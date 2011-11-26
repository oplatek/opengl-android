// OpenGL ES 2.0 C++ code 
#include <jni.h>
#include <jni.h>
#include <android/log.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef _Included_ondrej_platek_bind_NativeRenderer_
#define _Included_ondrej_platek_bind_NativeRenderer_

#define  LOG_TAG    "libbind"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

/* ---- do NOT FORGET to ADD new FUNCTIONS to EXTERN scope!! ----- */
extern "C" { 
    JNIEXPORT void JNICALL Java_ondrej_platek_bind_NativeRenderer_init(JNIEnv * env, jobject mythis);
    JNIEXPORT void JNICALL Java_ondrej_platek_bind_NativeRenderer_step(JNIEnv * env, jobject mythis);
    JNIEXPORT void JNICALL Java_ondrej_platek_bind_NativeRenderer_Zoom(JNIEnv * env, jobject mythis, float dz);
    JNIEXPORT void JNICALL Java_ondrej_platek_bind_NativeRenderer_RotateAnchor(JNIEnv * env, jobject mythis, float dx, float dy);
    JNIEXPORT void JNICALL Java_ondrej_platek_bind_BINDView_releaseCppResources(JNIEnv * env, jobject);
};

#define VERTEX_POS_INDX 0
#define VERTEX_POS_SIZE 3;
struct SVertex {
      GLfloat x,y,z;
      GLfloat nx,ny,nz; //normals
//      GLfloat t0_s,t0_t; // 1. texture coordinates
//      GLfloat t1_s,t1_t; // 2. texture coordinates
      GLfloat r,g,b;
      SVertex() {
              x = y = z = 0.0f; r = g = b = 0.5f; // grey color
      }
  SVertex(GLfloat x_, GLfloat y_, GLfloat z_, GLfloat r_ = 0.5f, GLfloat g_ = 0.5f, GLfloat b_ = 0.5f){
              x = x_; y = y_; z = z_; r = r_; g = g_; b = b_;
      }
  void LOG(int index) {
              LOGI("Vertex[%d] {x=%f,y=%f,z=%f,\n,r=%f,g=%f,b=%f}",index,x,y,z,r,g,b);
      }
};

////////// Data //////////

int numTriangle = 0;
SVertex * Vertices = NULL;

// gVertexShader and gFragmentShader definition are at cpp file

/////// Functions ////////

void releaseResources();
static void printGLString(const char *name, GLenum s);
static void checkGlError(const char* op);
GLuint loadShader(GLenum shaderType, const char* pSource);
GLuint createProgram(const char* pVertexSource, const char* pFragmentSource, float * v,int v_size);
bool setupGraphics(int w, int h, float * raw_vertices, int raw_size);
void renderFrame();
void zoom(float z);
void rotateAnchor(float dx, float dy);

#endif //#ifndef _Included_ondrej_platek_bind_NativeRenderer_

/// sample data for copying do not comment them out
//    numTriangle = 3;
//    Vertices = new SVertex[numTriangle];
//    Vertices[0] = SVertex(0.5f, 0.5f, 0.0f);
//    Vertices[1] = SVertex(-0.5f, -0.5f, 0.0f);
//    Vertices[2] = SVertex(0.5f, -0.5f, 0.0f);
//    Vertices[3] = SVertex(-0.5f, -0.5f, 0.0f);
//    Vertices[4] = SVertex(-0.5f, 0.5f, 0.0f);
//    Vertices[5] = SVertex(0.5f, 0.5f, 0.0f);
