cmake_minimum_required(VERSION 4.3)
project(global_maritime)

set(CMAKE_CXX_STANDARD 20)

find_package(Qt6 REQUIRED COMPONENTS
    Widgets
    OpenGL
    OpenGLWidgets
)

qt_add_executable(global_maritime main.cpp)

target_link_libraries(global_maritime PRIVATE
    Qt6::Widgets
    Qt6::OpenGL
    Qt6::OpenGLWidgets
)