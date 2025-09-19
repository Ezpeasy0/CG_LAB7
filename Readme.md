# OpenGL Starter (GLEW + GLFW + GLM)

Cross-platform OpenGL starter for **Windows (MSVC)**, **macOS (Intel & Apple Silicon)**, and **Linux** using **CMake + vcpkg** in **VS Code**.  
First run shows a small red triangle. 

---

## Quick Start

1) Install tools (see your OS below)  
2) Clone the repo and open it in **VS Code**  
3) **CMake: Select Configure Preset** → build (**F7**) → run (**F5**)

Repo: https://github.com/momotheoxy/OpenGLStarter

---

## VS Code Extensions (all OS)

- **C/C++** (by Microsoft)  
- **CMake Tools** (by Microsoft)

---

## Windows (MSVC) — Setup

1. Install **Visual Studio 2022 (Community)** with **Desktop development with C++**  
   https://visualstudio.microsoft.com/downloads/

2. Open **PowerShell** and install:
   ```powershell
   winget install Git.Git
   winget install Kitware.CMake

3. Install vcpkg:
   ```powershell
   git clone https://github.com/microsoft/vcpkg $env:USERPROFILE\vcpkg
   & $env:USERPROFILE\vcpkg\bootstrap-vcpkg.bat
   setx VCPKG_ROOT "$env:USERPROFILE\vcpkg"

4. Verify the toolchain file exists (should print True):
   ```powershell
   Test-Path "$env:USERPROFILE\vcpkg\scripts\buildsystems\vcpkg.cmake"

**We use MSVC on Windows (Visual Studio generator), not MinGW.**

---

## macOS (Intel & Apple Silicon) — Setup

1. Command Line Tools:
   ```bash
   xcode-select --install

2. Homebrew (if you don’t have it):
   ```bash
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

3. Tools:
   ```bash
   brew install git cmake ninja

4. vcpkg:
   ```bash
   git clone https://github.com/microsoft/vcpkg ~/vcpkg
   ~/vcpkg/bootstrap-vcpkg.sh
   echo 'export VCPKG_ROOT="$HOME/vcpkg"' >> ~/.bashrc   # or ~/.zshrc
   export VCPKG_ROOT="$HOME/vcpkg"

**macOS uses an OpenGL core, forward-compatible context (OpenGL 4.1 max).**

---

## Linux (Ubuntu/Debian) — Setup

   ```bash
   sudo apt update
   sudo apt install -y build-essential git cmake ninja-build
   git clone https://github.com/microsoft/vcpkg ~/vcpkg
   ~/vcpkg/bootstrap-vcpkg.sh
   echo 'export VCPKG_ROOT="$HOME/vcpkg"' >> ~/.bashrc
   export VCPKG_ROOT="$HOME/vcpkg"

---

## Get the Code

   ```bash
   git clone https://github.com/momotheoxy/OpenGLStarter
   cd OpenGLStarter

**Open the folder root in VS Code (the same folder that contains CMakeLists.txt).**

---

## Configure, Build, Run (VS Code)

1. **Select a Configure Preset**  
   Press **Ctrl/Cmd + Shift + P** → **CMake: Select Configure Preset**:
   - **Windows** → *Windows MSVC (VS2022)*
   - **macOS (Intel)** → *macOS Intel (x64, Ninja)*
   - **macOS (Apple Silicon)** → *macOS Apple Silicon (arm64, Ninja)*
   - **Linux** → *Linux x64 (Ninja)*

2. **Build**  
   Press **F7** (or *Terminal → Run Build Task… → Build (CMake preset)*).  
   The first build will install `glew`, `glfw3`, `glm` via vcpkg.

3. **Run / Debug**  
   Press **F5** (choose the matching debug config the first time).

---

## Important notes (Windows)

- **Open the correct folder**  
  Always open **the cloned repo folder** as your VS Code workspace (the folder that contains `CMakeLists.txt`).  
  VS Code will then use the `.vscode` files **from the repo**.  
  If you open a different folder, VS Code may create new `.vscode` files and **won’t use** the repo settings.

- **Avoid Unicode paths**  
  Do **not** put the project in a path with non-ASCII characters (e.g., Thai letters).  
  Use a simple path like: `C:\Dev\OpenGLStarter`.

- **Avoid OneDrive locks**  
  Building inside OneDrive can fail due to file locks. Prefer a local path, e.g., `C:\Dev\OpenGLStarter`.

---

## Troubleshooting (Windows)

- **“Could not find toolchain file … vcpkg.cmake”**  
  Make sure `VCPKG_ROOT` is set and the file exists at  
  `C:\Users\<you>\vcpkg\scripts\buildsystems\vcpkg.cmake`.  
  Then run **CMake: Delete Cache and Reconfigure**.

- **CMake picked GNU/MinGW instead of MSVC**  
  You used the wrong preset or an old cache.  
  Select preset **Windows MSVC (VS2022)** → **CMake: Delete Cache and Reconfigure**.  
  The log should say: `The CXX compiler identification is MSVC …`.

- **Ninja not found (Windows)**  
  You are on a Ninja preset by mistake. Use **Windows MSVC (VS2022)** (MSBuild).  
  Ninja is only used on macOS/Linux here.

- **glfw/glew config not accepted**  
  Don’t mix toolchains. On Windows use MSVC with the `x64-windows` triplet.  
  If you changed toolchains, delete `build/` and reconfigure.

---

## Folder Layout

OpenGLStarter/
  src/
    main.cpp
    Libs/
      Mesh.{h,cpp}
      Shader.{h,cpp}
      Window.{h,cpp}
  Shaders/
    shader.vert
    shader.frag
  .vscode/
    settings.json
    launch.json
    tasks.json
  CMakeLists.txt
  CMakePresets.json
  vcpkg.json
  README.md

---

## License / Credits

- Uses **GLEW**, **GLFW**, **GLM** via **vcpkg**  
- For educational use in computer graphics labs