# GLCraft

OpenGL/GLFW basic voxel game written in C++ 

This project is a learning experience and I'm always open to suggestions and critisicm

## Currently Implemented

* **Config System**: Adjustable settings through settings.json
* **Block Data Map**: Dynamic block data loading system
* **Input Handling**: Keyboard and mouse, supports raw mouse movment and calculating mouse deltas
* **OpenGL Renderer**: Rendering managed through OpenGL
* **Cached Texture Data**: Precalculated texture coordinates for faster mesh generation times
* **Greedy Meshing**: Removes invisible faces from mesh, reduces generation & render time

## Project Building

Currently only built/tested on Windows with MSVC

Before building the project, I recommend the following enviornment:

* 64 bit Windows 10+
* Visual Studio 2026
* OpenGL 3.3+ compatible hardware
* Newest/Platform Compatible version of dependancies(will eventually be compiled from source)

Build systems are managed through premake

For each build, the resource folder and required .dlls will be automatically pasted into the executable path

### 1. Windows 64bit
```
git clone https://github.com/soldacat4ket/GLCraft.git
cd GLCraft
premake5 vs2026
```

### 2. Literally everything else

Getting this project to work on other platforms will require some work

Premake may be able to generate project files for other platforms, but dependancies and some code(particularly wWinMain for release mode) may need updating

I plan to solve these issues through wider platform testing and compiling dependancies from source through seperate projects

If you run into any issues building for other platforms, feel free to let me know. I aim to make this project fully compatible with Windows, MacOS, and Linux

## Project File Structure
```.
├── res/                        # Assets & settings
│   ├── logs/                   # Game logs
│   ├── shaders/                # GLSL source files
│   ├── textures/               # Texture packs
│   ├── blockdata.json          # Structured block definitions
│   └── settings.json           # User configuration & preferences
│
├── src/
│   ├── Main.cpp                # Application entry point
│   └── OpenGLMinecraft/
│       ├── MultiMedia/
│       │   ├── Graphics/
│       │   │   ├── Objects/    # OpenGL wrappers (VAO, VBO, IBO)
│       │   │   ├── Renderer/   # Render pipelines & draw calls
│       │   │   ├── Shaders/    # Shader compilation logic
│       │   │   └── Texture/    # Texture resources & atlas calculations
│       │   └── Input/          # GLFW-based input handling
│       ├── World/
│       │   ├── Block/          # ID exchange logic (std::string ↔ uint16_t)
│       │   └── Chunk/          # Editable chunks & mesh building
│       ├── Physics/            # [WIP] Collision
│       └── Utility/            # Abstract structures & math helpers
│
├── vendor/                     # Third-party dependencies
│   ├── glad/                   # OpenGL loader
│   ├── glfw-3.4/               # Windowing & context
│   ├── glm/                    # OpenGL math
│   ├── nlohmann-json/          # JSON parsing support
│   ├── spdlog-1.x/             # Fast C++ logging
│   └── stb_image/              # Image loading utilities
└── premake5.lua                # Project build script```