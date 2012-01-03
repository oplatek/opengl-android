#ifndef _Included_ondrej_platek_bind_def_
#define _Included_ondrej_platek_bind_def_

//#define OS_ANDROID // alternative #define OS_LINUX
#define OS_LINUX // alternative #define OS_ANDROID

#ifdef OS_ANDROID
    #include <android/log.h>
    #define LOG_TAG "natRenderer"
    #define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
    #define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#else
#ifdef OS_LINUX
    #include <stdio.h> 
    #include <stdarg.h>
    void LOGI(const char * format, ...) ;
    void LOGE(const char* format, ...) ;
#endif
#endif

#endif
