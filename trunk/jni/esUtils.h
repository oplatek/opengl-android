#ifndef _Included_ondrej_platek_bind_esUtils_
#define _Included_ondrej_platek_bind_esUtils_

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <android/log.h>

#ifndef LOG_TAG
#define LOG_TAG "esUtils"
#endif
#ifndef LOGI
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#endif
#ifndef LOGE
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#endif

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

void printGLString(const char *name, GLenum s);
void checkGlError(const char* op);
GLuint loadShader(GLenum shaderType, const char* pSource);
GLuint createProgram(const char* pVertexSource, const char* pFragmentSource);

#endif
