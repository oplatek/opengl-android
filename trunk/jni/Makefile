# Makefile for compiling code for Linux with X11  
FLAGS=-Wall -DOS_LINUX
INCDIR=-I./
LIBS=-lGLESv2 -lEGL -lm -lX11

default: x11renderer 

ndk: ndk-build 

all: x11 ndk

clean:
	find . -type f -executable | xargs rm -f
	rm out/*.o


######## android2X11
out/logFunctions.o: logFunctions.cpp def.h natRenderer.h esUtils.h
	g++ -c ${FLAGS} logFunctions.cpp -o $@

out/esUtils.o: esUtils.cpp esUtils.h
	g++ -c ${FLAGS} esUtils.cpp -o $@

out/esX11Util.o: esX11Util.cpp esX11Util.h
	g++ -c ${FLAGS} esX11Util.cpp -o $@

out/x11renderer.o: x11renderer.cpp esUtils.h def.h natRenderer.h
	g++ -c ${FLAGS} x11renderer.cpp -o $@

out/genCube.o: genCube.cpp genCube.h natRenderer.h
	g++ -c ${FLAGS} genCube.cpp -o $@

out/AppCtx.o: AppCtx.cpp natRenderer.h
	g++ -c ${FLAGS} AppCtx.cpp -o $@

out/natRenderer.o: natRenderer.cpp natRenderer.h esUtils.h shader_vert.h shader_frag.h
	g++ -c ${FLAGS} natRenderer.cpp -o $@


x11renderer: out/logFunctions.o out/AppCtx.o out/esUtils.o out/esX11Util.o out/genCube.o out/x11renderer.o out/natRenderer.o
	g++ ${FLAGS} $^ -o $@ ${INCDIR} ${LIBS}   
