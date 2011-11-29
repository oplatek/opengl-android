#ifndef _Included_ondrej_platek_bind_esTransform_
#define _Included_ondrej_platek_bind_esTransform_

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <android/log.h>

#define  LOG_TAG    "esTransform"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#define PI 3.1415926535897932384626433832795f

typedef struct {
	GLfloat m[4][4];
} ESMatrix;


void esTranslate(ESMatrix *result, GLfloat tx, GLfloat ty, GLfloat tz);

void esMatrixMultiply(ESMatrix *result, ESMatrix *srcA, ESMatrix *srcB);

void esRotate(ESMatrix *result, GLfloat angle, GLfloat x, GLfloat y, GLfloat z);

void esMatrixLoadIdentity(ESMatrix *result);

void esFrustum(ESMatrix *result, float left, float right, float bottom, float top, float nearZ, float farZ);

void esPerspective(ESMatrix *result, float fovy, float aspect, float nearZ, float farZ);

#endif //_Included_ondrej_platek_bind_esTransform_

