/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkCMLMoleculeReader.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkCMLMoleculeReader
 * @brief   Read a CML file and output a
 * vtkMolecule object
 *
*/

#ifndef vtkCMLMoleculeReader_h
#define vtkCMLMoleculeReader_h

#include "vtkDomainsChemistryModule.h" // For export macro
#include "vtkMoleculeAlgorithm.h"
#include "config.h"

class vtkMolecule;

class VTKDOMAINSCHEMISTRY_EXPORT vtkBallStickReader : public vtkMoleculeAlgorithm
{
public:
  static vtkBallStickReader *New();
  vtkTypeMacro(vtkBallStickReader,vtkMoleculeAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) VTK_OVERRIDE;

  //@{
  /**
   * Get/Set the output (vtkMolecule) that the reader will fill
   */
  vtkMolecule *GetOutput();
  void SetOutput(vtkMolecule *) VTK_OVERRIDE;
  //@}

  //@{
  /**
   * Get/Set the name of the CML file
   */
  vtkSetStringMacro(FileName);
  vtkGetStringMacro(FileName);
  //@}

protected:
  vtkBallStickReader();
  ~vtkBallStickReader() VTK_OVERRIDE;

  int RequestData(vtkInformation *, vtkInformationVector **,
                  vtkInformationVector *) VTK_OVERRIDE;
  int FillOutputPortInformation(int, vtkInformation*) VTK_OVERRIDE;

  char *FileName;

private:
  vtkBallStickReader(const vtkBallStickReader&) VTK_DELETE_FUNCTION;
  void operator=(const vtkBallStickReader&) VTK_DELETE_FUNCTION;
};

#endif
