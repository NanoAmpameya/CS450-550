# CS450-550

Oregon State CS 450 — Intro to Computer Graphics (Fall 2026).

C++, OpenGL 4.1 core (macOS), GLFW, GLEW, GLM, SOIL2. Libraries live in `~/GL/OpenGL_Template`, not in this repo.

## Layout

- `Test Project/` — Homework 1: tool setup check
- `homework2/` — Homework 2: two original triangles, VBO/VAO, position color, shader animation
- `homework3/` — Homework 3: two 3D objects (cube + pyramid), GLM transforms, app-side animation
- `project-brief.md` — course log, setup notes, and homework status

## Build (no IDE)

From any homework folder:

```bash
make        # build → ./program
make run    # build + run
make clean  # remove the binary
```

Homework 2:

```bash
cd homework2
make run
```

Homework 3:

```bash
cd homework3
make run
```

Close the window to quit.

## Git

Keep this README current before each push whenever the repo layout, build steps, or homework status changed.
