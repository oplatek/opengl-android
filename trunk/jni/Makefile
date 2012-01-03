# Straight forward Makefile to compile all examples in a row

INCDIR=-I./Common
LIBS=-lGLESv2 -lEGL -lm -lX11

# COMMONSRC=./Common/esShader.c    \
#           ./Common/esTransform.c \
#           ./Common/esShapes.c    \
#           ./Common/esUtil.c
# COMMONHRD=esUtil.h

SRC=x11renderer.c esUtils.cpp esUtils.h

default: x11 

x11: ./x11renderer

ndk: ndk-build 

all: x11 ndk

clean:
	find . -executable | xargs rm -f
	ndk-build clean

./x11renderer: ${SRC} 
	gcc ${SRC} -o ./$@ ${INCDIR} ${LIBS}
