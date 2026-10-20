# CS 450 – Intro to Computer Graphics (Fall 2026) — Project Log

## Course basics
- Tech: C++, OpenGL (4.1 core on Mac), GLFW, GLEW, GLM, SOIL2
- Grading: total points earned / total points possible. this stuff is so easy, aim always to get a final 10/10

## Submission format (every homework)
- Video: hosted on a external platform so everytime you also create a small file to tell me how to start my build and run locally without using any IDEs, which I have to submit myself so u have to prepare me that and a human reviewer will asses the narration script that I use for it with captions on, so give me a very small public speaking draft for this as a normal text output., **under 5 minutes**
- **Build and run must be shown**
- Turn in: PDF + ALL project files (.cpp, .vert, .frag) + I upload the videos

## My machine
- MacBook Air, Apple Silicon (M3), macOS, zsh
- Visual Studio is Windows-only → using the approved Mac setup
  (Homebrew + Premake + Makefile)
- Homebrew installed at /opt/homebrew (PATH added to ~/.zprofile)

## Mac setup (done once)
Libraries:
  brew install glfw glew glm premake (install anyother if needed)

SOIL2 built with Premake:
  ~/GL/SOIL2 → premake5 gmake → make soil2-static-lib config=debug_$(uname -m)

Template folder (Mac equivalent of C:\GL\OpenGL_Template):
  ~/GL/
  ├── SOIL2/                     (source + bin/img_mars.jpg)
  └── OpenGL_Template/
      ├── include/  GL/ GLFW/ glm/ SOIL2/
      ├── lib/      libGLEW.dylib  libglfw3.a  libsoil2-debug.a
      └── project/  Makefile

Makefile: compiles EVERY .cpp in the folder with clang++ (C++17),
links glfw3, GLEW, soil2-debug and the macOS frameworks.
  make        → build
  make run    → build + run
  make clean  → remove the binary

## New project recipe (Mac version of "New Project → OpenGL Template")
  mkdir -p ~/CS450/<project>
  cd ~/CS450/<project>
  cp ~/GL/OpenGL_Template/project/Makefile .
  cp ~/Downloads/<starter>.cpp ./<name>.cpp
  touch shader.vert shader.frag
  make && make run
Rules:
- One .cpp with main() per folder (the Makefile builds all .cpp files)
- Always run from the project folder (relative paths for shaders/images)
- Windows paths like "C:\\GL\\..." in starter code must be changed
  (e.g. copy the image into the folder and use "img_mars.jpg")

## Lessons learned / gotchas
- Don't `rm -rf` the folder Terminal is currently in → "getcwd" errors;
  run `cd ~` first
- If ~/GL goes missing, check ~/CS450/GL (it was once moved there by accident)
- Ed formatting adds backticks — copy only the command text
- macOS caps OpenGL at 4.1 (shaders use `#version 410 core`)
- Cursor may show red squiggles on includes → editor-only; `make` still works
  (optional: add ~/GL/OpenGL_Template/include to the C/C++ include path)
- GitHub "Add a README" creates `origin/main` with a commit. You cannot
  `git pull --rebase` onto a local branch that has never been committed
  ("unborn branch with changes added to the index"). Commit locally first,
  then pull --rebase, then push.
- README.md is part of the repo. Before each push, update it if layout,
  build steps, or homework status changed — do not leave GitHub's placeholder.

## Homework 1 — Tool Setup Verification ✅ (submitted)
- test_install.cpp ran: orange "OpenGL Test" window,
  "GLM vector: 1", "SOIL2 image loaded: 512x512"
- Project: ~/CS450/Test Project (leave as is; don't reuse)


## Homework 2 — Loading, Interpolating, and Animating ✅ (code done locally)
Modules: Getting Data to a Shader, Interpolation, Simple Animation
Folder: ~/CS450/homework2
Files to turn in: homework2.cpp, shader.vert, shader.frag, plus a PDF with the Kaltura/OSU video link

What it draws:
- Original sailboat made of 2 triangles (NOT the course triangle):
  sail (tall triangle) + hull (wide triangle)
- 6 vertices in one VBO, interleaved as x,y,z,triangleId
- glDrawArrays(GL_TRIANGLES, 0, 6)
- Color from vertex position in the vertex shader; rasterizer interpolates vColor
- Shared uniform uTime (glfwGetTime). Different motion via aTriId:
  sail rotates around the mast; hull bobs/sways at a different frequency

Build / run (no IDE) — show this at the START of the video:
  cd ~/CS450/homework2
  make clean
  make
  make run
  (or: ./program after make)
Window title: "Homework 2 - Sailboat". Close the window to quit.
make clean → remove the binary

Video still on you:
- Record with Kaltura (auto captions + media.oregonstate.edu host)
- Voice it yourself, < 5 min, build+run FIRST
- Chat should give a short spoken script in the reply (do not skip this)
- Then PDF with the link + the 3 source files

Status:
- [x] Stage 1: own vertices in a VBO → both triangles visible
- [x] Stage 2: color from position (interpolated vColor)
- [x] Stage 3: two different animations from one uTime uniform + aTriId
- [x] Explanations ready (vertex data / interpolation / animation) — see video script
- [ ] Record, upload, submit (human; agent cannot record)

## Key concepts (until now)
- VBO = GPU memory holding vertex data; VAO = saved layout of how to read it
- Order in init(): uniform locations → gen VAO/VBO → bind VAO FIRST →
  bind VBO → glBufferData → glVertexAttribPointer → enable → unbind VAO → unbind VBO
- Order in display(): useProgram → set uniforms → bind VAO → draw → unbind
- `in` = per-vertex data; `uniform` = shared across vertices (matrices, time)
- Uniform location -1 = not found (silently ignored, no crash)
- Rasterizer interpolates per-vertex values (e.g. color) across a triangle
- Animation: glfwGetTime() → float t → sin/cos offsets → glm::translate on the model matrix