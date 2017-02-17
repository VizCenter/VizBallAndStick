#include "vtkActor.h"
#include "vtkBallStickReader.h"
#include "vtkCamera.h"
#include "vtkMolecule.h"
#include "vtkMoleculeMapper.h"
#include "vtkNew.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSphereSource.h"
#include "config.h"

int main(int, char *[]) {
  vtkNew<vtkBallStickReader>      source;
  vtkNew<vtkMoleculeMapper>         mapper;
  vtkNew<vtkActor>                  actor;
  vtkNew<vtkRenderer>               renderer;
  vtkNew<vtkRenderWindow>           renderWindow;
  vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;

  // Set the source and mapper
  source->SetFileName("porphyrin.cml");
  mapper->SetInputConnection(source->GetOutputPort());
  mapper->UseBallAndStickSettings();
  // Put the data set into a coordinate system
  actor->SetMapper(mapper.GetPointer());

  // Create a renderer, render window, and interactor
  renderWindow->AddRenderer(renderer.GetPointer());
  renderWindowInteractor->SetRenderWindow(renderWindow.GetPointer());
  // Add the actor to the scene
  renderer->AddActor(actor.GetPointer());
  renderer->SetBackground(.3, .6, .3); // Background color green
  // Render and interact
  renderWindow->Render();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}
