#ifndef _Included_ondrej_platek_bind_renderer_
#define _Included_ondrej_platek_bind_renderer_

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <android/log.h>
#include <android/log.h>

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <android/log.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <jni.h> //TODO SEPARATE JNI LAYER

#define LOG_TAG "natRenderer"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#define PI 3.1415926535897932384626433832795f

//static const char *classPathName = "ondrej/platek/bind/NativeRenderer";
// TODO JNI_OnLoad?

void printGLString(const char *name, GLenum s);
void checkGlError(const char* op);
GLuint loadShader(GLenum shaderType, const char* pSource);
GLuint createProgram(const char* pVertexSource, const char* pFragmentSource);


typedef struct {
	GLfloat m[4][4];
} ESMatrix;

/*
void esTranslate(ESMatrix *result, GLfloat tx, GLfloat ty, GLfloat tz);

void esMatrixMultiply(ESMatrix *result, ESMatrix *srcA, ESMatrix *srcB);

void esRotate(ESMatrix *result, GLfloat angle, GLfloat x, GLfloat y, GLfloat z);

void esMatrixLoadIdentity(ESMatrix *result);

void esFrustum(ESMatrix *result, float left, float right, float bottom, float top, float nearZ, float farZ);

void esPerspective(ESMatrix *result, float fovy, float aspect, float nearZ, float farZ);
*/

void printGLString(const char *name, GLenum s);

void checkGlError(const char* op);

GLuint loadShader(GLenum shaderType, const char* pSource);

GLuint createProgram(const char* pVertexSource, const char* pFragmentSource);

//////////////////// end of esUtils.h //////

struct AppCtx;

/////// Functions ////////

void releaseResources(AppCtx * c);
void renderFrame(AppCtx * c);
void renderTestFrame(AppCtx * c);
void zoom(AppCtx * c, float z);
void rotateAnchor(AppCtx * c, float dx, float dy);
bool setupGraphics(AppCtx * c);

#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     ondrej_platek_bind_NativeRenderer
 * Method:    init
 * Signature: ([[F[[S)V
 */
JNIEXPORT void JNICALL Java_ondrej_platek_bind_NativeRenderer_init
  (JNIEnv *, jobject, jobjectArray, jobjectArray);

/*
 * Class:     ondrej_platek_bind_NativeRenderer
 * Method:    step
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_ondrej_platek_bind_NativeRenderer_step
  (JNIEnv *, jobject);

/*
 * Class:     ondrej_platek_bind_NativeRenderer
 * Method:    Zoom
 * Signature: (F)V
 */
JNIEXPORT void JNICALL Java_ondrej_platek_bind_NativeRenderer_Zoom
  (JNIEnv *, jobject, jfloat);

/*
 * Class:     ondrej_platek_bind_NativeRenderer
 * Method:    RotateAnchor
 * Signature: (FF)V
 */
JNIEXPORT void JNICALL Java_ondrej_platek_bind_NativeRenderer_RotateAnchor
  (JNIEnv *, jobject, jfloat, jfloat);

/*
 * Class:     ondrej_platek_bind_NativeRenderer
 * Method:    releaseCppResources
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_ondrej_platek_bind_NativeRenderer_releaseCppResources
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif //#ifndef _Included_ondrej_platek_bind_renderer_
