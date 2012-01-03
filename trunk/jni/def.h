#ifndef _Included_ondrej_platek_bind_def_
#define _Included_ondrej_platek_bind_def_

#define OS_ANDROID // alternative #define OS_LINUX

#ifdef OS_ANDROID
    #include <android/log.h>
    #define LOG_TAG "natRenderer"
    #define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
    #define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#else
#ifdef OS_LINUX
    #include <stdio.h> 
    #include <stdarg.h>
    void LOGI(const char * format, ...) {
        va_list argptr;
        va_start(argptr, format);
        vfprintf(stdout, format, argptr);
        va_end(argptr);
    }

    void LOGE(const char* format, ...) {
        va_list argptr;
        va_start(argptr, format);
        vfprintf(stderr, format, argptr);
        va_end(argptr);
    }
#endif
#endif

#endif
