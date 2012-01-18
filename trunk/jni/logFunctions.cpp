#include "natRenderer.h"
#include "esUtils.h"
#include "def.h"

// LOGI and LOGE is defined in def.h
#ifdef OS_LINUX
    void LOGI(const char * format, ...) {
        va_list argptr;
        va_start(argptr, format);
        vfprintf(stdout, format, argptr);
        va_end(argptr);
        fprintf(stdout, "\n");
    }

    void LOGE(const char* format, ...) {
        va_list argptr;
        va_start(argptr, format);
        vfprintf(stderr, format, argptr);
        va_end(argptr);
        fprintf(stderr, "\n");
    }
#endif

void LogArrayGLui(const char * arrName, GLuint * arr, int length) {
    for(int i = 0; i < length; ++i ) {
        LOGI("%s[%d] = %d \n", arrName, i, arr[i]);
    }
}

void LogArrayF(const char * arrName, float * arr, int length) {
    for(int i = 0; i < length; ++i ) {
        LOGI("%s[%d] = %f \n", arrName, i, arr[i]);
    }
}

void SVertex::LOG(int index) const {
  LOGI("v[%d].xyz=(%f, %f, %f)",index,x,y,z);
  LOGI("v[%d].nx ny nz=(%f, %f, %f)",index,nx,ny,nz);
//  LOGI("v[%d].rgb=(%f, %f, %f)",index,r,g,b);
}

void LogVertices(AppCtx *c) {
    for(int i = 0; i < c->numVertices; ++i ) {
        c->vertices[i].LOG(i);
    }
}

void LOGm(ESMatrix *esm, AppCtx *c) {
    for(int i = 0; i < c->numVertices; ++i ) {
        SVertex v = c->vertices[i];
        Mat_x_Vertex(esm,&v);
        v.LOG(i);
    }
}

// should be only for debugging it is slow in CPU
SVertex* Mat_x_Vertex(ESMatrix * esm, SVertex * v) {
    SVertex tmp;
    tmp.x = (esm->m[0][0] * v->x) +(esm->m[0][1] * v->y) +(esm->m[0][2] * v->z) +(esm->m[0][3] * v->w) ;
    tmp.y = (esm->m[1][0] * v->x) +(esm->m[1][1] * v->y) +(esm->m[1][2] * v->z) +(esm->m[1][3] * v->w) ;
    tmp.z = (esm->m[2][0] * v->x) +(esm->m[2][1] * v->y) +(esm->m[2][2] * v->z) +(esm->m[2][3] * v->w) ;
    tmp.w = (esm->m[3][0] * v->x) +(esm->m[3][1] * v->y) +(esm->m[3][2] * v->z) +(esm->m[3][3] * v->w) ;
    *v = tmp;
//    memcpy(v,&tmp,sizeof(tmp));
    return v;
}
