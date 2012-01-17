#include "natRenderer.h"

/////////// AppCtx::AppCtx ////////////
AppCtx::AppCtx() {
	this->glProgram = 0; 
    this->numVertices = this->parts_number = 0;
	this->parts_sizes = 0;
	this->faces = 0;
	this->vertices = 0;
    this->shaderIdx_a_normals = 0;
}
/////////// AppCtx::~AppCtx ////////////
AppCtx::~AppCtx() {
    releaseVertexRes(this);
}

/////////// releaseResources ////////////
void releaseVertexRes(AppCtx * c) {
    if(c->vertices != 0) {
        delete[] c->vertices; 
        c->vertices = 0; // IMPORTANT for checking if allocated
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
