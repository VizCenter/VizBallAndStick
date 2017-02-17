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

  //@{
  /**
   * Get/Set the name of the Ball file
   */
  vtkSetStringMacro(BallFileName);
  vtkGetStringMacro(BallFileName);
  //@}

    //@{
  /**
   * Get/Set the name of the Ball file
   */
  vtkSetStringMacro(StickFileName);
  vtkGetStringMacro(StickFileName);
  //@}

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
  char *BallFileName;
  char *StickFileName;

private:
  vtkBallStickParser(const vtkBallStickParser&) VTK_DELETE_FUNCTION;
  void operator=(const vtkBallStickParser&) VTK_DELETE_FUNCTION;
};

#endif // VTKCMLPARSER_H
