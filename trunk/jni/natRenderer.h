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
      LOGI("v[%d].xyz=(%f, %f, %f)",index,x,y,z);
      LOGI("v[%d].rgb=(%f, %f, %f)",index,r,g,b);
    }
};

SVertex* Mat_x_Vertex(ESMatrix * esm, SVertex * v) {
	SVertex tmp;
	tmp.x = (esm->m[0][0] * v->x) +(esm->m[0][1] * v->y) +(esm->m[0][2] * v->z) +(esm->m[0][3] * 1) ;
	tmp.y = (esm->m[1][0] * v->x) +(esm->m[1][1] * v->y) +(esm->m[1][2] * v->z) +(esm->m[1][3] * 1) ;
	tmp.z = (esm->m[2][0] * v->x) +(esm->m[2][1] * v->y) +(esm->m[2][2] * v->z) +(esm->m[2][3] * 1) ;
	memcpy(v,&tmp,sizeof(tmp));
	return v;
}

void LOGm(ESMatrix *esm, SVertex v) {
    v.LOG(666);
    Mat_x_Vertex(esm,&v);
    v.LOG(777);
}

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
    GLuint shaderIdx_a_position;
    GLuint shaderIdx_a_color;
    GLuint shaderIdx_u_mvpMatrix;
    ESMatrix mvpMatrix;
    AppCtx();
    ~AppCtx();
};

/////// Functions ////////

void viewValuesSetUp(AppCtx * c);
void releaseVertexRes(AppCtx * c);
void renderFrame(AppCtx * c);
void renderTestFrame(AppCtx * c);
void zoom(AppCtx * c, float z);
void rotateAnchor(AppCtx * c, float dx, float dy);
bool setupGraphics(AppCtx * c);
void modelViewBoundaries(SVertex * verArr, int sizeArr, GLfloat * rxmin, GLfloat * rxmax, GLfloat  * rymin, GLfloat * rymax, GLfloat  * rzmin, GLfloat * rzmax);

#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     ondrej_platek_bind_NativeRenderer
 * Method:    initV
 * Signature: ([[F[[S)V
 */
JNIEXPORT void JNICALL Java_ondrej_platek_bind_NativeRenderer_initV
  (JNIEnv *, jobject, jobjectArray, jobjectArray);

/*
 * Class:     ondrej_platek_bind_NativeRenderer
 * Method:    initG
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_ondrej_platek_bind_NativeRenderer_initG
  (JNIEnv *, jobject);

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
  (JNIEnv *, jobject,float,float);

/*
 * Class:     ondrej_platek_bind_NativeRenderer
 * Method:    RotateAnchor
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_ondrej_platek_bind_NativeRenderer_changeWH
  (JNIEnv *, jobject);

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
JNIEXPORT void JNICALL Java_ondrej_platek_bind_NativeRenderer_initV(JNIEnv * env, jobject mythis,jobjectArray Normals, jobjectArray Faces)  {
    int ic = extractInt(env, mythis,"pAppCtx");
    LOGI("AppCtx pointer at beggining init: %d",ic);
    AppCtx * c =  reinterpret_cast<AppCtx*>(ic);
    if(c == NULL) { // create new AppCtx -> should be only 1
        c = new AppCtx();
        LOGI("AppCtx pointer initialize in init: %d",c);
    }
    else {
    	releaseVertexRes(c);
    }

	int raw_size = extractInt(env, mythis, "vertices_size");
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
	}

	c->parts_sizes = new int[c->parts_number];
	for(int i = 0; i < c->parts_number; i++) {
		c->parts_sizes[i] = raw_parts_sizes[i];
	}

	c->faces = new GLubyte*[c->parts_number];
	c->normals = new Normal*[c->parts_number];
	for(int i = 0; i < c->parts_number; i++) {
		 jshortArray oneDimFaces = (jshortArray) env->GetObjectArrayElement(Faces, i);
		 jfloatArray oneDimNormals = (jfloatArray)env->GetObjectArrayElement(Normals, i);
		 jshort * arrshort =env->GetShortArrayElements(oneDimFaces, 0);
		 jfloat * arrfloat =env->GetFloatArrayElements(oneDimNormals, 0);
		 c->faces[i] = new GLubyte[c->parts_sizes[i]];
		 c->normals[i] = new Normal[c->parts_sizes[i]];
		 for(int j = 0; j < c->parts_sizes[i]; j++) { // each part could have different number of vertices
			c->faces[i][j] = arrshort[j];
			c->normals[i][j]= Normal( arrfloat[j], arrfloat[j+1], arrfloat[j+2] );
		 }
	}

//	for(int i=0; i < c->parts_number; ++i) {
//		LOGI("parts_sizes %d", c->parts_sizes[i]);
//		for(int j =0; j< c->parts_sizes[i]; ++j) {
//			LOGI("faces[%d] = %d",j,((int)c->faces[i][j]));
//		}
//	}

	// Don't forget to release it
	env->ReleaseFloatArrayElements(*arr, raw_vertices, 0);
	env->ReleaseIntArrayElements(*arr2, raw_parts_sizes, 0);

    // return AppCtx c
    int retValue = reinterpret_cast<int>(c);
    LOGI("AppCtx pointer at the end of init: %d",retValue);
    returnInt(env, mythis,"pAppCtx", retValue);
} 
// end of Java_ondrej_platek_bind_NativeRenderer_init

JNIEXPORT void JNICALL Java_ondrej_platek_bind_NativeRenderer_initG(JNIEnv * env, jobject mythis)  {
    AppCtx * c =  reinterpret_cast<AppCtx*>(extractInt(env, mythis, "pAppCtx"));
    if(c == NULL) {
      LOGE("NativeRender_init context is NULL");
    }
    else {
    	setupGraphics(c);
    }
}

JNIEXPORT void JNICALL Java_ondrej_platek_bind_NativeRenderer_changeWH(JNIEnv *env, jobject mythis) {
    AppCtx * c =  reinterpret_cast<AppCtx*>(extractInt(env, mythis, "pAppCtx"));
    if(c == NULL) {
      LOGE("NativeRender_changeWH context is NULL");
    }
    else {
		c->height = extractInt(env, mythis, "height");
		c->width = extractInt(env, mythis, "width");
		viewValuesSetUp(c);
    }
}

/////////// JNICALL .._step ////////////
JNIEXPORT void JNICALL Java_ondrej_platek_bind_NativeRenderer_step(JNIEnv * env, jobject mythis) {
    AppCtx * c =  reinterpret_cast<AppCtx*>(extractInt(env, mythis, "pAppCtx"));
    if(c == NULL) {
      LOGE("NativeRender_step context is NULL");
    }
    else {
//    	renderTestFrame(c);
    	renderFrame(c);
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
    releaseVertexRes(c);
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
