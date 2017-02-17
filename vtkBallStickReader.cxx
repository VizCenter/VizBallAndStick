/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkCMLMoleculeReader.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notice for more information.

  =========================================================================*/
#include "vtkBallStickReader.h"
#include "vtkBallStickParser.h"

#include "vtkDataObject.h"
#include "vtkExecutive.h"
#include "vtkFieldData.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkMolecule.h"
#include "vtkNew.h"
#include "vtkObjectFactory.h"
#include "vtkPeriodicTable.h"
#include "vtkCMLParser.h"
#include "config.h"

#include <string>
#include <vector>

// Subclass of vtkXMLParser -- definitions at end of file
vtkStandardNewMacro(vtkBallStickReader);

//----------------------------------------------------------------------------
vtkBallStickReader::vtkBallStickReader()
  : FileName(NULL), BallFileName(NULL), StickFileName( NULL )
{
  this->SetNumberOfInputPorts(0);
}

//----------------------------------------------------------------------------
vtkBallStickReader::~vtkBallStickReader()
{
  this->SetFileName(NULL);
  this->SetBallFileName(NULL);
  this->SetStickFileName( NULL );
}

//----------------------------------------------------------------------------
vtkMolecule *vtkBallStickReader::GetOutput()
{
  return vtkMolecule::SafeDownCast(this->GetOutputDataObject(0));;
}

//----------------------------------------------------------------------------
void vtkBallStickReader::SetOutput(vtkMolecule *output)
{
  this->GetExecutive()->SetOutputData(0, output);
}

int vtkBallStickReader::RequestData(
  vtkInformation *,
  vtkInformationVector **,
  vtkInformationVector *outputVector)
{

  vtkMolecule *output = vtkMolecule::SafeDownCast
    (vtkDataObject::GetData(outputVector));

  if (!output)
  {
    vtkErrorMacro(<<"vtkCMLMoleculeReader does not have a vtkMolecule "
                  "as output.");
    return 1;
  }

#ifdef CML
  vtkCMLParser *parser = vtkCMLParser::New();
  parser->SetDebug(this->GetDebug());
  parser->SetFileName(this->FileName);
#else
  vtkBallStickParser *parser = vtkBallStickParser::New();
  parser->SetDebug(this->GetDebug());
  parser->SetBallFileName(this->BallFileName);
  parser->SetStickFileName(this->StickFileName);
#endif
  parser->SetTarget(output);

  if (!parser->Parse())
  {
#ifdef CML
    vtkWarningMacro(<<"Cannot parse file " << this->FileName << " as CML.");
#else
    vtkWarningMacro(<<"Cannot parse file " << this->BallFileName << " or " << this->StickFileName << ".");
#endif
    parser->Delete();
    return 1;
  }

  parser->Delete();

  return 1;
}

int vtkBallStickReader::FillOutputPortInformation(int, vtkInformation *info)
{
  info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkMolecule");
  return 1;
}

void vtkBallStickReader::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}

