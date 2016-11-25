cmake_minimum_required(VERSION 2.8.11)
 
if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
endif()
 
PROJECT(fatigueProject)
 
 
# root of your msvc14 x64 prebuild, there should be a `/lib/cmake` dir
set( CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} $ENV{QT5_DIR} )
set( CMAKE_INCLUDE_CURRENT_DIR ON )
set( CMAKE_AUTOMOC ON )


find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
   MESSAGE(CMAKE_BUILD_TYPE)
if(${VTK_VERSION} VERSION_GREATER "6" )
  # Instruct CMake to run moc automatically when needed.
  MESSAGE("VERSION_GREATER 6")
  set(CMAKE_AUTOMOC ON)
  find_package(Qt5Widgets REQUIRED QUIET)
else()
  MESSAGE("VERSION_GREATER 4")
  find_package(Qt4 REQUIRED)
  include(${QT_USE_FILE})
endif()
 
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
include_directories("C:/Python27x64/include")
include_directories("E:/workspace/vki/devtools4.3.5/src")
include_directories("E:/workspace/QtitanRibbon3.2.5_msvc2012_source/installpack001p/include")
 
file(GLOB UI_FILES *.ui)
file(GLOB QT_WRAP *.h)
file(GLOB CXX_FILES *.cxx)
 
if(${VTK_VERSION} VERSION_GREATER "6")
  QT5_ADD_RESOURCES(QRC_SOURCES   res/fatigueRc.qrc)
  qt5_wrap_ui(UISrcs ${UI_FILES} )
  # CMAKE_AUTOMOC in ON so the MOC headers will be automatically wrapped.
  add_executable(fatigueProject 
  ${QRC_SOURCES} 
  main.cpp 
  mainwindow.cpp 
  #widget3d.cpp 
  ribbonwindow.cpp
  mesh.cpp
  meshviewer.cpp
  )

 
  SOURCE_GROUP("Resources" FILES
  ${UI_FILES}
  ${QRC_SOURCES}
  )
  qt5_use_modules(fatigueProject Core Gui Widgets)
  target_link_libraries(fatigueProject ${VTK_LIBRARIES})

 
  if(VTK_LIBRARIES)
    if(${VTK_VERSION} VERSION_LESS "6")
      target_link_libraries(fatigueProject ${VTK_LIBRARIES} QVTK)
    else()
      target_link_libraries(fatigueProject ${VTK_LIBRARIES})
    endif()
  else()
    target_link_libraries(fatigueProject vtkHybrid QVTK vtkViews ${QT_LIBRARIES})
  endif()
endif()

TARGET_LINK_LIBRARIES(fatigueProject "C:/Python27x64/libs/python27.lib")
TARGET_LINK_LIBRARIES(fatigueProject "E:/workspace/vki/devtools4.3.5/lib/WIN64/devtools.lib")
TARGET_LINK_LIBRARIES(fatigueProject "E:/workspace/QtitanRibbon3.2.5_msvc2012_source/installpack001p/bin/qtnribbond3.lib")
#qt vtk vki python qt ribbion 