#include "vtkCMLParser.h"
#include "vtkObjectFactory.h"

//
// vtkCMLParser Methods
//
#include <string>

vtkStandardNewMacro(vtkCMLParser);

vtkCMLParser::vtkCMLParser()
  : vtkXMLParser(),
    Target(0)
{
}

vtkCMLParser::~vtkCMLParser()
{
  this->SetTarget(NULL);
}

void vtkCMLParser::StartElement(const char *name, const char **attr)
{
  if (strcmp(name, "atom") == 0)
  {
    this->NewAtom(attr);
  }
  else if (strcmp(name, "bond") == 0)
  {
    this->NewBond(attr);
  }
  else if (strcmp(name, "molecule") == 0)
  {
    this->NewMolecule(attr);
  }
  else if (this->GetDebug())
  {
    std::string desc;
    desc += "Unhandled CML Element. Name: ";
    desc += name;
    desc += "\n\tAttributes:";
    int attrIndex = 0;
    while (const char * cur = attr[attrIndex])
    {
      if (attrIndex > 0)
      {
        desc.push_back(' ');
      }
      desc += cur;
      ++attrIndex;
    }
    vtkDebugMacro(<<desc);
  }

  return;
}

void vtkCMLParser::EndElement(const char *)
{
}

void vtkCMLParser::NewMolecule(const char **)
{
  this->Target->Initialize();
}

void vtkCMLParser::NewAtom(const char **attr)
{
  vtkAtom atom = this->Target->AppendAtom();
  int attrInd = 0;
  unsigned short atomicNum = 0;
  float pos[3];
  const char * id = NULL;
  while (const char * cur = attr[attrInd])
  {
    // Get atomic number
    if (strcmp(cur, "elementType") == 0)
    {
      const char *symbol = attr[++attrInd];
      atomicNum = pTab->GetAtomicNumber(symbol);
      printf("atomicNum = %d\n",atomicNum);
    }

    // Get position
    else if (strcmp(cur, "x3") == 0)
      pos[0] = atof(attr[++attrInd]);
    else if (strcmp(cur, "y3") == 0)
      pos[1] = atof(attr[++attrInd]);
    else if (strcmp(cur, "z3") == 0)
      pos[2] = atof(attr[++attrInd]);

    // string id / names
    else if (strcmp(cur, "id") == 0)
      id = attr[++attrInd];

    else
    {
      vtkDebugMacro(<< "Unhandled atom attribute: " << cur);
    }

    ++attrInd;
  }

  atom.SetAtomicNumber(atomicNum);
  atom.SetPosition(pos);

  // Store name for lookups
  size_t atomId = static_cast<size_t>(atom.GetId());
  if (atomId >= this->AtomNames.size())
  {
    this->AtomNames.resize(atomId + 1);
  }

  this->AtomNames[atomId] = std::string(id);

  vtkDebugMacro(<< "Added atom #" << atomId << " ('" << id << "') ");

}

void vtkCMLParser::NewBond(const char **attr)
{
  int attrInd = 0;
  vtkIdType atomId1 = -1;
  vtkIdType atomId2 = -1;
  unsigned short order = 0;

  while (const char * cur = attr[attrInd])
  {
    // Get names of bonded atoms
    if (strcmp(cur, "atomRefs2") == 0)
    {
      char atomRefs[128];
      strncpy(atomRefs, attr[++attrInd], 128);
      // Parse out atom names:
      const char *nameChar = strtok(atomRefs, " ");
      while (nameChar != NULL)
      {
        vtkIdType currentAtomId;
        bool found = false;
        for (currentAtomId = 0;
             currentAtomId < static_cast<vtkIdType>(this->AtomNames.size());
             ++currentAtomId)
        {
          if (this->AtomNames[currentAtomId].compare(nameChar) == 0)
          {
            found = true;
            break;
          }
        }
        if (!found)
        {
          // Create list of known atom names:
          std::string allAtomNames ("");
          for (size_t i = 0; i < this->AtomNames.size(); ++i)
          {
            allAtomNames += this->AtomNames[i];
            allAtomNames.push_back(' ');
          }
          vtkWarningMacro(<< "NewBond(): unknown atom name '"
                          << nameChar << "'. Known atoms:\n"
                          << allAtomNames.c_str());

          nameChar = strtok(NULL, " ");
          continue;
        }
        else if (atomId1 == -1)
        {
          atomId1 = currentAtomId;
        }
        else if (atomId2 == -1)
        {
          atomId2 = currentAtomId;
        }
        else
        {
          vtkWarningMacro(<< "NewBond(): atomRef2 string has >2 atom names: "
                          << atomRefs);
        }

        nameChar = strtok(NULL, " ");
      }
    }

    // Get bond order
    else if (strcmp(cur, "order") == 0)
    {
      order = static_cast<unsigned short>(atoi(attr[++attrInd]));
    }

    else
    {
      vtkDebugMacro(<< "Unhandled bond attribute: " << cur);
    }

    ++attrInd;
  }

  if (atomId1 < 0 || atomId2 < 0)
  {
    vtkWarningMacro(<< "NewBond(): Invalid atom ids: " << atomId1
                    << " " << atomId2);
    return;
  }

  vtkDebugMacro(<< "Adding bond between atomids " << atomId1 << " "
                << atomId2);

  this->Target->AppendBond(atomId1, atomId2, order);
}
