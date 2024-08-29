# Dominoes-Physics-OpenGL
The project "Dominoes-Physics-OpenGL" is a school project designed to simulate falling dominoes using OpenGL for rendering. The primary goal is to learn and apply concepts of physics and computer graphics.

## Project Structure
### .vscode/: Contains Visual Studio Code settings.
  settings.json: Configuration for the VS Code workspace.
### bin/: Contains compiled binaries.
  Debug/: Directory for debug binaries.
  sdl2-config: Configuration script for SDL2.
### include/: Contains header files.
  animation.h: Header for animation-related functions.
  forms.h: Header for form-related functions.
  geometry.h: Header for geometry-related functions.
  SDL2/: Directory for SDL2 headers.
### lib/: Contains libraries.
  cmake/: CMake configuration files for SDL2.
    sdl2-config.cmake: CMake project-config input for SDL2.
  libSDL2_image.a, libSDL2_image.dll.a, libSDL2_image.la: Libraries for SDL2 image handling.
  libSDL2_test.a: Library for SDL2 testing.
  libSDL2main.a: Main library for SDL2.
  libSDL2.la: Main SDL2 library.
### obj/: Contains object files and project files.
  Projet_Support_CodeBlocks.cbp: Code::Blocks project file.
  Projet_Support_CodeBlocks.depend: Dependency file for the project.
  Projet_Support_CodeBlocks.layout: Layout file for the project.
### resources/: Contains resource files.
  SDL2_image.dll, SDL2.dll: SDL2 dynamic link libraries.
### share/: Contains shared files.
### src/: Contains source files.
  animation.cpp: Source file for animation-related functions.
  first_prog.cpp: Source file for the first program.
  forms.cpp: Source file for form-related functions.
  geometry.cpp: Source file for geometry-related functions.
### libjpeg-9.dll, libpng16-16.dll, libtiff-5.dll, libwebp-4.dll, libwebp-7.dll, zlib1.dll: Dynamic link libraries for various image formats.

## Build Instructions
To build the project, you can use the provided Code::Blocks project file.
  Open Projet_Support_CodeBlocks.cbp in Code::Blocks.
  Select the desired build target (Debug or Release).
  Build the project.

## Dependencies
SDL2
SDL2_image
OpenGL
GLU

