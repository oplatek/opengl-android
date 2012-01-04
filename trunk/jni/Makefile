# Makefile for compiling code for Linux with X11  
FLAGS=-Wall -DOS_LINUX
INCDIR=-I./
LIBS=-lGLESv2 -lEGL -lm -lX11

default: x11renderer 

ndk: ndk-build 

all: x11 ndk

clean:
	find . -type f -executable | xargs rm -f
	rm *.o


######## android2X11
logFunctions.o: logFunctions.cpp def.h natRenderer.h esUtils.h
	g++ -c ${FLAGS} logFunctions.cpp

esUtils.o: esUtils.cpp esUtils.h
	g++ -c ${FLAGS} esUtils.cpp

esX11Util.o: esX11Util.cpp esX11Util.h
	g++ -c ${FLAGS} esX11Util.cpp

x11renderer.o: x11renderer.cpp esUtils.h def.h natRenderer.h
	g++ -c ${FLAGS} x11renderer.cpp

genCube.o: genCube.cpp genCube.h natRenderer.h
	g++ -c ${FLAGS} genCube.cpp

AppCtx.o: AppCtx.cpp natRenderer.h
	g++ -c ${FLAGS} AppCtx.cpp

natRenderer.o: natRenderer.cpp natRenderer.h esUtils.h
	g++ -c ${FLAGS} natRenderer.cpp


x11renderer: logFunctions.o AppCtx.o esUtils.o esX11Util.o genCube.o x11renderer.o natRenderer.o
	g++ ${FLAGS} $^ -o $@ ${INCDIR} ${LIBS}   
