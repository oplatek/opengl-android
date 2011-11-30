#ifndef _Included_ondrej_platek_bind_renderer_
#define _Included_ondrej_platek_bind_renderer_

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <android/log.h>
#include <android/log.h>


#include <stdio.h>
#include <stdlib.h>
#include <jni.h> 

#include "def.h"
#include "esUtils.h"


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
    ESMatrix mvpMatrix;
    AppCtx();
    ~AppCtx();
};


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



// helper functions
int extractInt(JNIEnv * env, jobject mythis,const char * memberName);
jobject objForArray(JNIEnv * env, jobject mythis, const char * memberName,const char * type);
void returnInt(JNIEnv * env, jobject mythis,const char * memberName,int v);

/////////// JNICALL .._init ////////////
JNIEXPORT void JNICALL Java_ondrej_platek_bind_NativeRenderer_init(JNIEnv * env, jobject mythis,jobjectArray Normals, jobjectArray Faces)  {

    const char * str ="pAppCtx";
    int ic = extractInt(env, mythis,"pAppCtx");
    LOGI("AppCtx pointer at beggining init: %d",ic);
    AppCtx * c =  reinterpret_cast<AppCtx*>(ic);
    if(c == NULL) { // create new AppCtx -> should be only 1 
        c = new AppCtx();
        LOGI("AppCtx pointer initialize in init: %d",c);
    }
    else {
    	releaseResources(c);
    }

    int raw_size = extractInt(env, mythis, "vertices_size");
    c->height = extractInt(env, mythis, "height");
    c->width = extractInt(env, mythis, "width");
    c->parts_number = extractInt(env, mythis, "parts_number");

    // jArrays -> I have to release them afterwards
    jobject mvdata = objForArray(env, mythis, "vertices", "[F");
    jfloatArray * arr = reinterpret_cast<jfloatArray*>(&mvdata);
    float * raw_vertices = env->GetFloatArrayElements(*arr, NULL);

    mvdata = objForArray(env, mythis, "parts_sizes", "[I");
    jintArray * arr2 = reinterpret_cast<jintArray*>(&mvdata);
    int * raw_parts_sizes = env->GetIntArrayElements(*arr2, NULL);

    c->numVertices = raw_size/3;
    c->vertices = new SVertex[c->numVertices]; // important to release before it

    int t;
    for(int i=0; i < c->numVertices; ++i) {
        t = 3*i; // tripple times to index i
        c->vertices[i] = SVertex(raw_vertices[t], raw_vertices[t+1], raw_vertices[t+2]);
        c->vertices[i].LOG(i);
    }

    c->parts_sizes = new int[c->parts_number];
    for(int i = 0; i < c->parts_number; i++) {
    	c->parts_sizes[i] = raw_parts_sizes[i];
    }

    c->faces = new GLubyte*[c->parts_number];
    c->normals = new Normal*[c->parts_number];
    for(int i = 0; i < c->parts_number; i++) {
         jshortArray oneDimFaces = (jshortArray) env->GetObjectArrayElement(Faces, i);
         LOGI("Loaded c->faces step 1 parts number %d",c->parts_number);
         jfloatArray oneDimNormals = (jfloatArray)env->GetObjectArrayElement(Normals, i);
         jshort * arrshort =env->GetShortArrayElements(oneDimFaces, 0);
         LOGI("Loaded c->faces step 2 parts number %d", c->parts_number);
         jfloat * arrfloat =env->GetFloatArrayElements(oneDimNormals, 0);
         LOGI("c->parts_sizes %d",c->parts_sizes[i]);
         c->faces[i] = new GLubyte[c->parts_sizes[i]];
         LOGI("Loaded c->faces step 3 parts number %d", c->parts_number);
         c->normals[i] = new Normal[c->parts_sizes[i]];
         for(int j = 0; j < c->parts_sizes[i]; j++) { // each part could have different number of vertices
        	// copy to local arrays
        	LOGI("Loaded c->faces step 4 parts number %d forloop j %d",c->parts_number,j);
            c->faces[i][j] = arrshort[j];
            c->normals[i][j]= Normal( arrfloat[j], arrfloat[j+1], arrfloat[j+2] );
            LOGI("Part %d, Vertex %d: indc->vertices %d",i,j,c->faces[i][j]);
            LOGI("Part %d, Vertex %d: normal %f %f %f",i,j,c->faces[i][j],c->normals[i][j].x, c->normals[i][j].y, c->normals[i][j].z);
         }
    }

    setupGraphics(c);

    // Don't forget to release it
    env->ReleaseFloatArrayElements(*arr, raw_vertices, 0);
    env->ReleaseIntArrayElements(*arr2, raw_parts_sizes, 0);

    // return AppCtx c
    int retValue = reinterpret_cast<int>(c);
    LOGI("AppCtx pointer at the end of init: %d",retValue);
    returnInt(env, mythis,"pAppCtx", retValue);
} 
// end of Java_ondrej_platek_bind_NativeRenderer_init


/////////// JNICALL .._step ////////////
JNIEXPORT void JNICALL Java_ondrej_platek_bind_NativeRenderer_step(JNIEnv * env, jobject mythis) {
    AppCtx * c =  reinterpret_cast<AppCtx*>(extractInt(env, mythis, "pAppCtx"));
    if(c == NULL) {
      LOGE("NativeRender_step context is NULL");
    }
    else {
    renderTestFrame(c);
//    renderFrame(c);
    }
}
  
/////////// JNICALL .._Zoom ////////////
JNIEXPORT void JNICALL Java_ondrej_platek_bind_NativeRenderer_Zoom(JNIEnv * env, jobject mythis, float z) {
    AppCtx * c =  reinterpret_cast<AppCtx*>(extractInt(env, mythis, "pAppCtx"));
    if(c == NULL) {
        LOGE("NativeRender_Zoom context is NULL");
    }
    else {
        zoom(c,z);
    }
}

/////////// JNICALL .._RotateAnchor ////////////
JNIEXPORT void JNICALL Java_ondrej_platek_bind_NativeRenderer_RotateAnchor(JNIEnv * env, jobject mythis, float dx, float dy) { 
    // LOGI("rotating with dx, dy by %f %f", dx,dy);
    AppCtx * c =  reinterpret_cast<AppCtx*>(extractInt(env, mythis, "pAppCtx"));
    if(c == NULL) {
        LOGE("NativeRender_RotateAnchor context is NULL");
    } 
    else {
        rotateAnchor(c, dx, dy);
    }
}

/////////// JNICALL .._releaseCpp resources ////////////
JNIEXPORT void JNICALL Java_ondrej_platek_bind_NativeRenderer_releaseCppResources(JNIEnv * env, jobject mythis) {
    AppCtx * c =  reinterpret_cast<AppCtx*>(extractInt(env, mythis, "pAppCtx"));
    if(c == NULL) {
      LOGE("NativeRender_releaseCppResources context is NULL");
    }
    releaseResources(c);
}



int extractInt(JNIEnv * env, jobject mythis,const char * memberName) {
    jclass cls = env->GetObjectClass(mythis);
    jfieldID fieldID_integer = env->GetFieldID(cls, memberName, "I");
    // integer we do not have to extract from container env->GetIntField..
    return env->GetIntField(mythis, fieldID_integer);
}

void returnInt(JNIEnv * env, jobject mythis,const char * memberName, int v) {
    jclass cls = env->GetObjectClass(mythis);
    jfieldID fieldID_integer = env->GetFieldID(cls, memberName, "I");
    env->SetIntField(mythis, fieldID_integer, v);
}

jobject objForArray(JNIEnv * env, jobject mythis, const char * memberName,const char * type) {
    jclass cls = env->GetObjectClass(mythis);
    jfieldID fieldID_raw_vertices = env->GetFieldID(cls, memberName, type);
    env->GetFieldID(cls,memberName,type);
    return env->GetObjectField(mythis, fieldID_raw_vertices);
}

#endif //#ifndef _Included_ondrej_platek_bind_renderer_
