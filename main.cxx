#include "vtkActor.h"
#include "vtkBallStickReader.h"
#include "vtkCamera.h"
#include "vtkMolecule.h"
#include "vtkOpenGLMoleculeMapper.h"
//#include "vtkMoleculeMapper.h"
#include "vtkNew.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkOpenGLRenderer.h"
#include "vtkSphereSource.h"
#include "config.h"

/**
 * this is the main class
 */
int main(int, char *[]) {
  vtkNew<vtkBallStickReader>      source;
  vtkNew<vtkOpenGLMoleculeMapper>         mapper;
  vtkNew<vtkActor>                  actor;
  vtkNew<vtkOpenGLRenderer>               renderer;
  vtkNew<vtkRenderWindow>           renderWindow;
  vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;

  // Set the source and mapper
#ifdef CML
  source->SetFileName("porphyrin.cml");
#else
  source->SetBallFileName("MasaData/ball-Nikhil0.dat");
  source->SetStickFileName("MasaData/throat-Nikhil0.dat");
#endif
  mapper->SetInputConnection(source->GetOutputPort());
  mapper->UseBallAndStickSettings();
  mapper->SetAtomicRadiusTypeToCustomArrayRadius();
  mapper->SetColorModeToDirectScalars();
  mapper->SetColorModeToDefault();
  mapper->SetColorModeToMapScalars();
  mapper->ScalarVisibilityOn();
  mapper->SetScalarModeToUsePointFieldData();
  mapper->SetBondRadius(.15);
  mapper->SelectColorArray("colors");

  // Put the data set into a coordinate system
  actor->SetMapper(mapper.GetPointer());

  // Create a renderer, render window, and interactor
  renderWindow->AddRenderer(renderer.GetPointer());
  renderWindowInteractor->SetRenderWindow(renderWindow.GetPointer());
  // Add the actor to the scene
  renderer->AddActor(actor.GetPointer());
  renderer->SetBackground(.3, .3, .3); // Background color green
  // Render and interact
  renderWindow->Render();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}
