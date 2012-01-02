#include "natRenderer.h"

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
	tmp.x = (esm->m[0][0] * v->x) +(esm->m[0][1] * v->y) +(esm->m[0][2] * v->z) +(esm->m[0][3] * 1) ;
	tmp.y = (esm->m[1][0] * v->x) +(esm->m[1][1] * v->y) +(esm->m[1][2] * v->z) +(esm->m[1][3] * 1) ;
	tmp.z = (esm->m[2][0] * v->x) +(esm->m[2][1] * v->y) +(esm->m[2][2] * v->z) +(esm->m[2][3] * 1) ;
	memcpy(v,&tmp,sizeof(tmp));
	return v;
}
