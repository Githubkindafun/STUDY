# ==================================================
# Portable makefile for compilation on platforms: 
#     Linux   /   Windows with MSYS2 using gcc
# install required packages like:
#       e.g. gcc, make,   libglfw3-dev, libglew-dev 
# ==== TARGET ARCHITECTURE =========================
UNAMES := $(shell uname -o)
ifeq      (${UNAMES},GNU/Linux) #==== Linux ========
COPTS = 
CLIBS = -lGL -lglfw -lGLEW -ldl
EXE = 
else ifeq (${UNAMES},Msys)  #==== Windows/Msys2 ====
COPTS = -I /msys64/mingw64/include/
CLIBS = -L /msys64/mingw64/lib/ -lopengl32 -lglfw3 -lglew32
EXE = .exe
COPTS += -DARCH_MSYS=${UNAMES}
else
TARGET = unknown-target     #==== UNKNOWN ==========
defaulterror:
	echo Unknown target architecture: uname -s 
endif                       #=======================
#===================================================
default: tutorial02$(EXE)

%$(EXE): %.cpp
	g++ -I. $(COPTS)  $< -o $@ common/shader.cpp $(CLIBS) 
clean:
	rm a.out *.o *~ tutorial02$(EXE)
