#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <android/log.h>
#include "renderer.h"
#include "jniInterface.h"

int extractInt(JNIEnv * env, jobject mythis,const char * memberName);

jobject objForArray(JNIEnv * env, jobject mythis, const char * memberName,const char * type);

/////////// JNICALL .._init ////////////
JNIEXPORT int JNICALL Java_ondrej_platek_bind_NativeRenderer_init(JNIEnv * env, jobject mythis,jobjectArray Normals, jobjectArray Faces)  {
	const char * str ="pAppCtx";
    AppCtx * c =  reinterpret_cast<AppCtx*>(extractInt(env, mythis,"pAppCtx"));
    if(c == NULL) { // create new AppCtx -> should be only 1 
        AppCtx * c = new AppCtx();
    }
    else {
    	releaseResources(c);
    }

    int raw_size = extractInt(env, mythis, "vertexes_size"); 
    c->height = extractInt(env, mythis, "height");
    c->width = extractInt(env, mythis, "width");
    c->parts_number = extractInt(env, mythis, "parts_number");

    // jArrays -> I have to release them afterwards
    jobject mvdata = objForArray(env, mythis, "vertexes", "[F");
    jfloatArray * arr = reinterpret_cast<jfloatArray*>(&mvdata);
    float * raw_vertices = env->GetFloatArrayElements(*arr, NULL);

    mvdata = objForArray(env, mythis, "parts_sizes", "[I");
    jintArray * arr2 = reinterpret_cast<jintArray*>(&mvdata);
    jint * raw_parts_sizes = env->GetIntArrayElements(*arr2, NULL);

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
//         LOGI("Loaded c->faces step 1 parts number %d",c->parts_number);
         jfloatArray oneDimNormals = (jfloatArray)env->GetObjectArrayElement(Normals, i);
         jshort * arrshort =env->GetShortArrayElements(oneDimFaces, 0);
//         LOGI("Loaded c->faces step 2 parts number %d", c->parts_number);
         jfloat * arrfloat =env->GetFloatArrayElements(oneDimNormals, 0);
//         LOGI("c->parts_sizes %d",c->parts_sizes[i]);
         c->faces[i] = new GLubyte[c->parts_sizes[i]];
//         LOGI("Loaded c->faces step 3 parts number %d", c->parts_number);
         c->normals[i] = new Normal[c->parts_sizes[i]];
         for(int j = 0; j < c->parts_sizes[i]; j++) { // each part could have different number of vertices
        	// copy to local arrays
//        	LOGI("Loaded c->faces step 4 parts number %d forloop j %d",c->parts_number,j);
            c->faces[i][j] = arrshort[j];
            c->normals[i][j]= Normal( arrfloat[j], arrfloat[j+1], arrfloat[j+2] );
//            LOGI("Part %d, Vertex %d: indc->vertices %d",i,j,c->faces[i][j]);
//            LOGI("Part %d, Vertex %d: normal %f %f %f",i,j,c->faces[i][j],normals[i][j].x, normals[i][j].y, normals[i][j].z);
         }
      }
    setupGraphics(c);


    // Don't forget to release it
    env->ReleaseFloatArrayElements(*arr, raw_vertices, 0);
    env->ReleaseIntArrayElements(*arr2, raw_parts_sizes, 0);

    // return AppCtx c
    return reinterpret_cast<int>(c);
}


/////////// JNICALL .._step ////////////
JNIEXPORT void JNICALL Java_ondrej_platek_bind_NativeRenderer_step(JNIEnv * env, jobject mythis) {
    AppCtx * c =  reinterpret_cast<AppCtx*>(extractInt(env, mythis, "pAppCtx"));
    if(c == NULL) {
      LOGE("NativeRender_step context is NULL");
    }
    renderTestFrame(c);
    renderFrame(c);
}
  
/////////// JNICALL .._Zoom ////////////
JNIEXPORT void JNICALL Java_ondrej_platek_bind_NativeRenderer_Zoom(JNIEnv * env, jobject mythis, float z) {
    AppCtx * c =  reinterpret_cast<AppCtx*>(extractInt(env, mythis, "pAppCtx"));
    if(c == NULL) {
      LOGE("NativeRender_Zoom context is NULL");
    }
    zoom(c,z);
}

/////////// JNICALL .._RotateAnchor ////////////
JNIEXPORT void JNICALL Java_ondrej_platek_bind_NativeRenderer_RotateAnchor(JNIEnv * env, jobject mythis, float dx, float dy) { 
    // LOGI("rotating with dx, dy by %f %f", dx,dy);
    AppCtx * c =  reinterpret_cast<AppCtx*>(extractInt(env, mythis, "pAppCtx"));
    if(c == NULL) {
      LOGE("NativeRender_RotateAnchor context is NULL");
    }
    rotateAnchor(c, dx, dy);
}

/////////// JNICALL .._releaseCpp resources ////////////
JNIEXPORT void JNICALL Java_ondrej_platek_bind_BINDView_releaseCppResources(JNIEnv * env, jobject mythis) {
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

jobject objForArray(JNIEnv * env, jobject mythis, const char * memberName,const char * type) {
	jclass cls = env->GetObjectClass(mythis);
    jfieldID fieldID_raw_vertices = env->GetFieldID(cls, memberName, type);
    env->GetFieldID(cls,"asdf","asdf");
    return env->GetObjectField(mythis, fieldID_raw_vertices);
}

