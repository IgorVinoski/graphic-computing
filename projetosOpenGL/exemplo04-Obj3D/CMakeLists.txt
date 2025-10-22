cmake_minimum_required(VERSION 3.10)
project(Proj_OpenGL_Exemplo_Obj3D)

set(CMAKE_CXX_STANDARD 17)


# Definições de compilação
add_compile_definitions(
    STB_IMAGE_IMPLEMENTATION
    STB_IMAGE_WRITE_IMPLEMENTATION
    TINYOBJLOADER_IMPLEMENTATION
    TINYPLY_IMPLEMENTATION
)


# Diretórios de inclusão
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    # ${CMAKE_CURRENT_SOURCE_DIR}/../include/imgui
)

# Procura pela biblioteca libglfw3.a
find_library(GLFW3_LIBRARY
    NAMES glfw3
    PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../lib
    NO_DEFAULT_PATH
)

#Arquivos fonte do projeto
set(SOURCES
    main.cpp
    CGMath.cpp
    objeto3d.cpp
    opengl_func.cpp
    shader.cpp
    textura.cpp
    ../src/glad.c
    # ../src/imgui.cpp
    # ../src/imgui_draw.cpp
    # ../src/imgui_impl_glfw.cpp
    # ../src/imgui_impl_opengl3.cpp
    # ../src/imgui_tables.cpp
    # ../src/imgui_widgets.cpp
)


# Adiciona o executável
add_executable(${PROJECT_NAME} ${SOURCES})


# Diretórios de bibliotecas
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib)


# Vincula as bibliotecas necessárias
target_link_libraries(${PROJECT_NAME}
    ${GLFW3_LIBRARY}
    opengl32
    gdi32
)
