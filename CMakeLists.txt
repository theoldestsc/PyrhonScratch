cmake_minimum_required(VERSION 3.0.0)
project(PythonScratch VERSION 0.1.0)
enable_testing()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(Qt6 COMPONENTS OpenGLWidgets REQUIRED)
find_package(Boost REQUIRED)
find_package(assimp REQUIRED)

add_executable(main 
               src/main.cpp 
               src/openGLWidget.cpp
               src/camera.cpp
               src/mesh.cpp
               src/model.cpp
               resources.qrc)

include_directories(${Boost_INCLUDE_DIR}
                    ${QT6_INCLUDE_DIR}
                    "D:\\MSYS\\mingw64\\include\\stb")
target_link_libraries(main ${Boost_LIB} Qt6::Widgets Qt6::OpenGLWidgets assimp)

