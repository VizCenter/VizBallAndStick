
cmake_minimum_required(VERSION 2.8)

PROJECT(Sphere)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(Sphere MACOSX_BUNDLE
  main.cxx
  vtkBallStickParser.cxx
  vtkBallStickParser.h
  # vtkCMLParser.cpp
  # vtkCMLParser.h
  vtkBallStickReader.cxx
  vtkBallStickReader.h
 )


if(VTK_LIBRARIES)
  target_link_libraries(Sphere ${VTK_LIBRARIES})
else()
  target_link_libraries(Sphere vtkHybrid vtkWidgets)
endif()
