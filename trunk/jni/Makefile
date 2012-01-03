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

esX11Util.o: esX11Util.c esX11Util.h
	gcc -c ${FLAGS} esX11Util.c

x11renderer.o: x11renderer.cpp esUtils.h def.h natRenderer.h
	g++ -c ${FLAGS} x11renderer.cpp

x11renderer: logFunctions.o esUtils.o esX11Util.o x11renderer.o
	g++ -o ./$@ ${INCDIR} ${LIBS} 
# DO NOT DELETE

