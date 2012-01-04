# Straight forward Makefile to compile all examples in a row
FLAGS=-Wall
INCDIR=-I./
LIBS=-lGLESv2 -lEGL -lm -lX11

# COMMONSRC=./Common/esShader.c    \
#           ./Common/esTransform.c \
#           ./Common/esShapes.c    \
#           ./Common/esUtil.c
# COMMONHRD=esUtil.h

default: x11 

x11: x11renderer

ndk: ndk-build 

all: x11 ndk

clean:
	find . -type f -executable | xargs rm -f
	rm *.o
	ndk-build clean

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

x11renderer: logFunctions.o AppCtx.o esUtils.o esX11Util.o genCube.o x11renderer.o
	g++ ${FLAGS} $? -o $@ ${INCDIR} ${LIBS}   
