#ifndef _Included_ondrej_platek_bind_renderer_general_
#define _Included_ondrej_platek_bind_renderer_general_
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>


#include <stdio.h>
#include <stdlib.h>

#include "def.h"
#include "esUtils.h"

struct SVertex {
    GLfloat x,y,z,w;
    GLfloat r,g,b;
//    GLfloat t0_s,t0_t; // 1. texture coordinates
//    GLfloat t1_s,t1_t; // 2. texture coordinates
    SVertex() {
              x = y = z = w = 0.0f; r = g = b = 0.5f; // grey color
    }
    SVertex(GLfloat x_, GLfloat y_, GLfloat z_, GLfloat w_ = 1.0f,
            GLfloat r_ = 0.5f, GLfloat g_ = 0.5f, GLfloat b_ = 0.5f) {
	  x = x_; y = y_; z = z_; w = w_; r = r_; g = g_; b = b_;
    }
    void LOG(int index);
};

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
    ESMatrix T; // translate matrix
    ESMatrix S; // scale matrix
    ESMatrix R; // rotation matrix
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
void LOGm(ESMatrix *esm, AppCtx *c);
SVertex* Mat_x_Vertex(ESMatrix * esm, SVertex * v);

#endif
