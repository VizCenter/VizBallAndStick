#ifndef VTKCMLPARSER_H
#define VTKCMLPARSER_H
#include "vtkXMLParser.h"
#include "vtkMolecule.h"
#include "vtkNew.h"
#include "vtkPeriodicTable.h"
#include <vector>

class vtkBallStickParser : public vtkXMLParser
{
public:
  vtkTypeMacro(vtkBallStickParser, vtkXMLParser);
  static vtkBallStickParser * New();

  vtkSetObjectMacro(Target, vtkMolecule);
  vtkGetObjectMacro(Target, vtkMolecule);

protected:
  vtkBallStickParser();
  ~vtkBallStickParser() VTK_OVERRIDE;
  void StartElement(const char *name, const char **attr) VTK_OVERRIDE;
  void EndElement(const char *name) VTK_OVERRIDE;

  std::vector<std::string> AtomNames;

  vtkMolecule *Target;

  void NewMolecule(const char **attr);
  void NewAtom(const char **attr);
  void NewBond(const char **attr);

  vtkNew<vtkPeriodicTable> pTab;

private:
  vtkBallStickParser(const vtkBallStickParser&) VTK_DELETE_FUNCTION;
  void operator=(const vtkBallStickParser&) VTK_DELETE_FUNCTION;
};

#endif // VTKCMLPARSER_H
