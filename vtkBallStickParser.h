#ifndef VTKCMLPARSER_H
#define VTKCMLPARSER_H
#include "vtkXMLParser.h"
#include "vtkMolecule.h"
#include "vtkNew.h"
#include "vtkPeriodicTable.h"
#include <vector>

class vtkCMLParser : public vtkXMLParser
{
public:
  vtkTypeMacro(vtkCMLParser, vtkXMLParser);
  static vtkCMLParser * New();

  vtkSetObjectMacro(Target, vtkMolecule);
  vtkGetObjectMacro(Target, vtkMolecule);

protected:
  vtkCMLParser();
  ~vtkCMLParser() VTK_OVERRIDE;
  void StartElement(const char *name, const char **attr) VTK_OVERRIDE;
  void EndElement(const char *name) VTK_OVERRIDE;

  std::vector<std::string> AtomNames;

  vtkMolecule *Target;

  void NewMolecule(const char **attr);
  void NewAtom(const char **attr);
  void NewBond(const char **attr);

  vtkNew<vtkPeriodicTable> pTab;

private:
  vtkCMLParser(const vtkCMLParser&) VTK_DELETE_FUNCTION;
  void operator=(const vtkCMLParser&) VTK_DELETE_FUNCTION;
};

#endif // VTKCMLPARSER_H
