cmake_minimum_required(VERSION 4.3)
project(global_maritime)

set(CMAKE_CXX_STANDARD 20)

find_package(Qt6 REQUIRED COMPONENTS
    Widgets
    OpenGL
    OpenGLWidgets
)

qt_add_executable(global_maritime main.cpp
        src/widgets/GlobeViewer.cpp
        src/widgets/GlobeViewer.h
        src/core/shader/Shader.cpp
        src/core/shader/Shader.h
        src/core/shader/ShaderSources.cpp
        src/core/shader/ShaderSources.h
        src/core/texture/Texture.cpp
        src/core/texture/Texture.h
        src/core/objects/GlobeGenerator.cpp
        src/core/objects/GlobeGenerator.h
        src/core/objects/Camera.cpp
        src/core/objects/Camera.h)

target_link_libraries(global_maritime PRIVATE
    Qt6::Widgets
    Qt6::OpenGL
    Qt6::OpenGLWidgets
)