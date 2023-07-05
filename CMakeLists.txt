cmake_minimum_required(VERSION 3.0.0)
project(LearnPython3D VERSION 0.1.0 LANGUAGES CXX)
enable_testing()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(Qt6 COMPONENTS OpenGLWidgets REQUIRED)
#find_package(Boost REQUIRED)
find_package(assimp REQUIRED)



list(APPEND flags "-fPIC" "-Wall")
if(NOT WIN32)  
    list(APPEND flags "-Wextra" "-Wpedantic")
endif()



add_executable(main 
               src/main.cpp 
               src/openGLWidget.cpp
               src/camera.cpp
               src/mesh.cpp
               src/model.cpp
               resources.qrc)
set_target_properties(main
                      PROPERTIES
                        OUTPUT_NAME "LearnPython3D")

include_directories(${Boost_INCLUDE_DIR}
                    ${QT6_INCLUDE_DIR})

target_link_libraries(main ${Boost_LIB} 
                      Qt6::Widgets 
                      Qt6::OpenGLWidgets 
                      assimp)

