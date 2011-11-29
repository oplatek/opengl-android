#ifndef _Included_ondrej_platek_bind_renderer_
#define _Included_ondrej_platek_bind_renderer_

#include <jni.h> //TODO SEPARATE JNI LAYER
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <android/log.h>
#include "esTransform.h"
#include <android/log.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#ifndef NULL
//#define NULL   ((void *) 0)
//#endif

#ifndef LOG_I
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#endif
#ifndef LOG_E
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#endif

struct SVertex {
    GLfloat x,y,z;
    GLfloat r,g,b;
//    GLfloat t0_s,t0_t; // 1. texture coordinates
//    GLfloat t1_s,t1_t; // 2. texture coordinates
    SVertex() {
              x = y = z = 0.0f; r = g = b = 0.5f; // grey color
    }
    SVertex(GLfloat x_, GLfloat y_, GLfloat z_, 
            GLfloat r_ = 0.5f, GLfloat g_ = 0.5f, GLfloat b_ = 0.5f) {
	  x = x_; y = y_; z = z_; r = r_; g = g_; b = b_;
    }
    void LOG(int index) {
      LOGI("Vertex[%d] {x=%f,y=%f,z=%f,\n,r=%f,g=%f,b=%f}",index,x,y,z,r,g,b);
    }
};

struct Normal{
    GLfloat x,y,z;
    Normal() {
    	x = y = z = 0.0f;
    }
    Normal(GLfloat x_, GLfloat y_, GLfloat z_) {
       x = x_; y = y_; z = z_;
    }
};

struct AppCtx {
    int numVertices; // sum(part_sizes) == numVertices
    int parts_number;
    int width;
    int height;
    int * parts_sizes;
    int glProgram;
    Normal ** normals; // first dim[pointer]: part_index, second dim[GLfloat]: normals per part
    GLubyte **  faces; //firrst dim[pointer]: part_index, second dim[GLubyte]: indeces to vertices per part
    SVertex * vertices;
    ESMatrix * mvpMatrix;
    AppCtx() {
    	this->glProgram; this->numVertices = this->parts_number = 0;
    	this->parts_sizes = NULL;
    	this->faces = NULL;
    	this->vertices = NULL;
    	this->mvpMatrix = NULL;
    }
    ~AppCtx();
};

/////// Functions ////////

void releaseResources(AppCtx * c);
void renderFrame(AppCtx * c);
void renderTestFrame(AppCtx * c);
void zoom(AppCtx * c, float z);
void rotateAnchor(AppCtx * c, float dx, float dy);
bool setupGraphics(AppCtx * c);


/// JNI interface /////
// do NOT FORGET to ADD new FUNCTIONS to EXTERN scope!!
extern "C" {
    JNIEXPORT void JNICALL Java_ondrej_platek_bind_NativeRenderer_init(
    		JNIEnv * env, jobject mythis,jobjectArray normals, jobjectArray faces);
    JNIEXPORT void JNICALL Java_ondrej_platek_bind_NativeRenderer_step(
    		JNIEnv * env, jobject mythis);
    JNIEXPORT void JNICALL Java_ondrej_platek_bind_NativeRenderer_Zoom(
    		JNIEnv * env, jobject mythis, float dz);
    JNIEXPORT void JNICALL Java_ondrej_platek_bind_NativeRenderer_RotateAnchor(
    		JNIEnv * env, jobject mythis, float dx, float dy);
    JNIEXPORT void JNICALL Java_ondrej_platek_bind_BINDView_releaseCppResources(
    		JNIEnv * env, jobject);
};

#endif //#ifndef _Included_ondrej_platek_bind_renderer_
