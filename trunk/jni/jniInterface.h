#ifndef _Included_ondrej_platek_bind_jniInterface_
#define _Included_ondrej_platek_bind_jniInterface_

#include <jni.h>
#include <android/log.h>

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

#endif // _Included_ondrej_platek_bind_jniInterface_
