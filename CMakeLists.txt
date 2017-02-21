
cmake_minimum_required(VERSION 2.8)

PROJECT(BallStick)
find_package(HDF5 REQUIRED)
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

include_directories(${HDF5_INCLUDE_DIR})
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


target_link_libraries(BallStick ${VTK_LIBRARIES} ${HDF5_LIBRARIES})

