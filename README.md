# G-Man

A lightweight OpenGL model viewer and renderer.

## Features
- **Model Loading**: Drag & drop support (Assimp).
- **Camera**: Free-fly camera with mouse and keyboard controls.
- **UI**: Integrated ImGui for settings.
- **Rendering**: OpenGL 3.3 Core Profile.

## Controls
- **Right Click (Hold)**: Look around.
- **W/A/S/D**: Move.
- **Q/E**: Up/Down.
- **Drag & Drop**: Load 3D model file.

## Dependencies
This project uses the following libraries (included as submodules):
- **GLFW**: Windowing and input.
- **GLEW**: OpenGL extension loader.
- **GLM**: Mathematics.
- **Assimp**: 3D model import.
- **ImGui**: User interface.
- **spdlog**: Logging.
- **stb**: Image loading.

## Build
Clone with submodules:
```bash
git clone --recursive https://github.com/yourusername/G-Man.git
```

Build with CMake:
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Credits
- **Model**: "G-Man (Blacksuit)" (https://skfb.ly/pA8WJ) by VV is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).
