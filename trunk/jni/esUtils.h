#ifndef _Included_ondrej_platek_bind_esutils_
#define _Included_ondrej_platek_bind_esutils_
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "def.h"

#define PI 3.1415926535897932384626433832795f

typedef struct {
	GLfloat m[4][4];
} ESMatrix;

void test(const char * t);

void printGLString(const char *name, GLenum s);

void checkGlError(const char* op);

GLuint loadShader(GLenum shaderType, const char* pSource);

GLuint createProgram(const char* pVertexSource, const char* pFragmentSource);

void esTranslate(ESMatrix *result, GLfloat tx, GLfloat ty, GLfloat tz);

void esMatrixMultiply(ESMatrix *result, ESMatrix *srcA, ESMatrix *srcB);

void esRotate(ESMatrix *result, GLfloat angle, GLfloat x, GLfloat y, GLfloat z);

void esScale(ESMatrix *result, GLfloat sx, GLfloat sy, GLfloat sz);

void esMatrixLoadIdentity(ESMatrix *result);

void esFrustum(ESMatrix *result, float left, float right, float bottom, float top, float nearZ, float farZ);

void esPerspective(ESMatrix *result, float fovy, float aspect, float nearZ, float farZ);

void printGLString(const char *name, GLenum s);

void checkGlError(const char* op);

void logMatrix(ESMatrix * m) ;
#endif
//////////////////// end of esUtils.h //////
