#include <jni.h> 
#include "def.h"
#include "natRenderer.h"

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
JNIEXPORT void JNICALL Java_ondrej_platek_bind_NativeRenderer_initV(JNIEnv * env, jobject mythis,jobjectArray npo, jobjectArray Faces)  {
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

    int vert_raw_size = extractInt(env, mythis, "vertices_size");
    int norm_raw_size = extractInt(env, mythis, "normals_size");
    c->parts_number = extractInt(env, mythis, "parts_number");

    // jArrays -> I have to release them afterwards
    jobject mvdata = objForArray(env, mythis, "vertices", "[F");
    jfloatArray * arr = reinterpret_cast<jfloatArray*>(&mvdata);
    float * raw_vertices = env->GetFloatArrayElements(*arr, NULL);

    mvdata = objForArray(env, mythis, "normals", "[F");
    jfloatArray * arr1 = reinterpret_cast<jfloatArray*>(&mvdata);
    float * raw_normals = env->GetFloatArrayElements(*arr1, NULL);


    mvdata = objForArray(env, mythis, "parts_sizes", "[I");
    jintArray * arr2 = reinterpret_cast<jintArray*>(&mvdata);
    int * raw_parts_sizes = env->GetIntArrayElements(*arr2, NULL);

    c->parts_sizes = new int[c->parts_number];
    // TODO memcpy
    for(int i = 0; i < c->parts_number; i++) {
        c->parts_sizes[i] = raw_parts_sizes[i];
    }

    c->faces = new GLuint*[c->parts_number];
    GLuint ** np = new GLuint*[c->parts_number];
    for(int i = 0; i < c->parts_number; i++) {
         jshortArray oneDimFaces = (jshortArray) env->GetObjectArrayElement(Faces, i);
         jshortArray oneDimNormP = (jshortArray)env->GetObjectArrayElement(npo, i);
         jshort * arrface =env->GetShortArrayElements(oneDimFaces, 0);
         jshort * arrnorm =env->GetShortArrayElements(oneDimNormP, 0);
         c->faces[i] = new GLuint[c->parts_sizes[i]];
         np[i] = new GLuint[c->parts_sizes[i]];
         for(int j = 0; j < c->parts_sizes[i]; j++) { // each part could have different number of vertices
            c->faces[i][j] = arrface[j];
            np[i][j]= arrnorm[j];
         }
    }

    // delete np, fill vertices, change faces
    separateVertices(& c->numVertices, c->vertices,
            raw_vertices, raw_normals, c->faces, np, c->parts_sizes);


    // Don't forget to release it
    env->ReleaseFloatArrayElements(*arr, raw_vertices, JNI_ABORT);
    env->ReleaseFloatArrayElements(*arr1, raw_normals, JNI_ABORT);
    env->ReleaseIntArrayElements(*arr2, raw_parts_sizes, JNI_ABORT);

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

