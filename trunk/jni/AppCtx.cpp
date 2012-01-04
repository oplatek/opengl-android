#include "natRenderer.h"

/////////// AppCtx::AppCtx ////////////
AppCtx::AppCtx() {
	this->glProgram = 0; 
    this->numVertices = this->parts_number = 0;
	this->parts_sizes = NULL;
	this->faces = NULL;
	this->vertices = NULL;
}
/////////// AppCtx::~AppCtx ////////////
AppCtx::~AppCtx() {
    releaseVertexRes(this);
}

/////////// releaseResources ////////////
void releaseVertexRes(AppCtx * c) {
    if(c->vertices != 0) {
        delete[] c->vertices; 
        c->vertices = NULL; // IMPORTANT for checking if allocated
    }
    if(c->normals != 0) {
        for (int i = 0; i < c->parts_number; ++i){
                delete [] c->normals[i];
        }
        delete[] c->normals;
    }
    if(c->faces != 0) {
        for (int i = 0; i < c->parts_number; ++i){
                delete [] c->faces[i];
        }
        delete[] c->faces;
    }

    if(c->parts_sizes != NULL) {
        delete [] c->parts_sizes;
    }

    glDeleteProgram(c->glProgram);
}
