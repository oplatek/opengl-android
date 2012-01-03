# Straight forward Makefile to compile all examples in a row

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

logFunctions.o: logFunctions.cpp def.h natRenderer.h
	g++ -c logFunctions.cpp

x11renderer: ${SRC} ${HDR}
	gcc ${SRC} -o ./$@ ${INCDIR} ${LIBS}
