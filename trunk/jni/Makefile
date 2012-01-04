# Makefile for compiling code for Linux with X11  
FLAGS=-Wall -DOS_LINUX
INCDIR=-I./
LIBS=-lGLESv2 -lEGL -lm -lX11

default: x11renderer 

ndk: ndk-build 

all: x11 ndk

clean:
	find . -type f -executable | xargs rm -f
	rm obj/*.o


######## android2X11
obj/logFunctions.o: logFunctions.cpp def.h natRenderer.h esUtils.h
	g++ -c ${FLAGS} logFunctions.cpp -o $@

obj/esUtils.o: esUtils.cpp esUtils.h
	g++ -c ${FLAGS} esUtils.cpp -o $@

obj/esX11Util.o: esX11Util.cpp esX11Util.h
	g++ -c ${FLAGS} esX11Util.cpp -o $@

obj/x11renderer.o: x11renderer.cpp esUtils.h def.h natRenderer.h
	g++ -c ${FLAGS} x11renderer.cpp -o $@

obj/genCube.o: genCube.cpp genCube.h natRenderer.h
	g++ -c ${FLAGS} genCube.cpp -o $@

obj/AppCtx.o: AppCtx.cpp natRenderer.h
	g++ -c ${FLAGS} AppCtx.cpp -o $@

obj/natRenderer.o: natRenderer.cpp natRenderer.h esUtils.h
	g++ -c ${FLAGS} natRenderer.cpp -o $@


x11renderer: obj/logFunctions.o obj/AppCtx.o obj/esUtils.o obj/esX11Util.o obj/genCube.o obj/x11renderer.o obj/natRenderer.o
	g++ ${FLAGS} $^ -o $@ ${INCDIR} ${LIBS}   
