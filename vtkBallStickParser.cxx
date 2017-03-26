#include "vtkBallStickParser.h"
#include "vtkObjectFactory.h"

//
// vtkCMLParser Methods
//
#include <string>
#include <H5Cpp.h>

vtkStandardNewMacro(vtkBallStickParser);
//using namespace H5;

vtkBallStickParser::vtkBallStickParser()
    : vtkXMLParser(),
      Target(0)
{
}

vtkBallStickParser::~vtkBallStickParser()
{
  this->SetTarget(NULL);
}
// ----------------------------------------------------------------------------
int vtkBallStickParser::Parse()
{
  // const H5std_string h5BallFileName( "ball.h5" );
  // const H5std_string BALLS_DATASET_NAME( "Balls" );
  // const H5std_string B_ID( "id" );
  // const H5std_string B_X( "x" );
  // const H5std_string B_Y( "y" );
  // const H5std_string B_Z( "z" );
  // const H5std_string B_R( "r" );

  // const H5std_string h5StickFileName( "Stick.h5" );
  // const H5std_string SITCKS_DATASET_NAME( "Sticks" );
  // const H5std_string S_ID( "id" );
  // const H5std_string S_IDSide1( "idSide1" );
  // const H5std_string S_IDSide2( "idSide2" );
  // const H5std_string S_R( "r" );

  //This function only opens the file
  FILE *ballFile = fopen(this->BallFileName, "r");
  // H5File* h5Ball = new H5File(h5BallFileName, H5F_ACC_TRUNC);

  typedef struct {
    unsigned long int id;
    double x,y,z,r;
  } Ball;

//  CompType mtype1( sizeof(Ball) );
//  mtype1.insertMember( B_ID, HOFFSET(Ball, id), PredType::NATIVE_UINT64);
//  mtype1.insertMember( B_X , HOFFSET(Ball, x),  PredType::NATIVE_DOUBLE);
//  mtype1.insertMember( B_Y , HOFFSET(Ball, y),  PredType::NATIVE_DOUBLE);
//  mtype1.insertMember( B_Z , HOFFSET(Ball, z),  PredType::NATIVE_DOUBLE);
//  mtype1.insertMember( B_R , HOFFSET(Ball, r),  PredType::NATIVE_DOUBLE);

  if(ballFile == NULL)
  {
    vtkErrorMacro(<<"Cannot open BallFile for reading.");
    return 1;
  }

  Ball ball;
  int conditionBall = 1;
  while(conditionBall)
  int ballFileLength;
  fseek(ballFile,0,SEEK_END);
  ballFileLength = ftell(ballFile);
  fseek(ballFile,0,SEEK_SET);
  {
    conditionBall = fscanf(ballFile,"%lu %lE %lE %lE %lE\n",
                           &ball.id,
                           &ball.x,
                           &ball.y,
                           &ball.z,
                           &ball.r);
    //printf("%lu %lE %lE %lE %lE\n",idBall,x,y,z,rBall);
  }

  FILE *stickFile = fopen(this->StickFileName, "r");
  if(stickFile == NULL)
  {
    vtkErrorMacro(<<"Cannot open StickFile for reading.");
    return 1;
  }

  typedef struct{
    unsigned long int id, idSide1, idSide2;
    double r;
  } Stick;
  Stick stick;
  int conditionStick = 1;
  while (conditionStick){
      conditionStick = fscanf(stickFile,"%lu %lu %lu %lE\n",

//  CompType stype( sizeof(Stick) );
//  stype.insertMember( S_ID      , HOFFSET(Ball, id), PredType::NATIVE_UINT64);
//  stype.insertMember( S_IDSide1 , HOFFSET(Ball, x),  PredType::NATIVE_UINT64);
//  stype.insertMember( S_IDSide2 , HOFFSET(Ball, y),  PredType::NATIVE_UINT64);
//  stype.insertMember( S_R       , HOFFSET(Ball, r),  PredType::NATIVE_DOUBLE);

                              &stick.id,
                              &stick.idSide1,
                              &stick.idSide2,
                              &stick.radius);
      //printf("%lu %lu %lu %lE\n",idStick,idSide1,idSide2,rStick);
  }
  return 1;
}

void vtkBallStickParser::StartElement(const char *name, const char **attr)
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

void vtkBallStickParser::EndElement(const char *)
{
}

void vtkBallStickParser::NewMolecule(const char **)
{
  this->Target->Initialize();
}

void vtkBallStickParser::NewAtom(const char **attr)
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

void vtkBallStickParser::NewBond(const char **attr)
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
