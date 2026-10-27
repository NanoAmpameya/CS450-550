GL       ?= $(HOME)/GL/OpenGL_Template
CXXFLAGS := -std=c++17 -Wall -g -DGL_SILENCE_DEPRECATION -I$(GL)/include
LDLIBS   := -L$(GL)/lib -lglfw3 -lGLEW -lsoil2-debug -framework OpenGL \
            -framework Cocoa -framework IOKit -framework CoreVideo \
            -framework CoreFoundation -framework QuartzCore -Wl,-rpath,$(GL)/lib
program: $(wildcard *.cpp) ; clang++ $(CXXFLAGS) $^ -o program $(LDLIBS)
run: program ; ./program
clean: ; rm -rf program program.dSYM
