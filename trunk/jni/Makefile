# Straight forward Makefile to compile all examples in a row
FLAGS=-Wall
INCDIR=-I./
LIBS=-lGLESv2 -lEGL -lm -lX11

# COMMONSRC=./Common/esShader.c    \
#           ./Common/esTransform.c \
#           ./Common/esShapes.c    \
#           ./Common/esUtil.c
# COMMONHRD=esUtil.h

SRC=x11renderer.c esUtils.cpp logFunctions.cpp esX11Util.c
HRD=esUtils.h def.h esX11Util.h

default: x11 

x11: x11renderer

ndk: ndk-build 

all: x11 ndk

clean:
	find . -executable | xargs rm -f
	ndk-build clean

logFunctions.o: logFunctions.cpp def.h natRenderer.h esUtils.h
	g++ -c ${FLAGS} ogFunctions.cpp

esUtils.o: esUtils.cpp esUtils.h
	g++ -c ${FLAGS} esUtils.cpp

esX11Util.o: esX11Util.c esX11Util.h
	g++ -c ${FLAGS} esX11Util.cpp

x11renderer.o: x11renderer.c esUtils.h def.h natRenderer.h
	g++ -c ${FLAGS} x11renderer.c 

x11renderer: logFunctions.o esUtils.o esX11Util.o x11renderer.o
	g++ -o ./$@ ${INCDIR} ${LIBS} 
# DO NOT DELETE

