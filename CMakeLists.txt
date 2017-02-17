
cmake_minimum_required(VERSION 2.8)

PROJECT(Sphere)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(BallStick MACOSX_BUNDLE
  main.cxx
  config.h
  vtkBallStickParser.cxx
  vtkBallStickParser.h
  vtkCMLParser.cpp
  vtkCMLParser.h
  vtkBallStickReader.cxx
  vtkBallStickReader.h
 )


if(VTK_LIBRARIES)
  target_link_libraries(BallStick ${VTK_LIBRARIES})
else()
  target_link_libraries(BallStick vtkHybrid vtkWidgets)
endif()
