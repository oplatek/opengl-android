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
    GLfloat nx,ny,nz;
//    GLfloat t0_s,t0_t; // 1. texture coordinates
//    GLfloat t1_s,t1_t; // 2. texture coordinates
    SVertex() {
        w = 1.0f; x = y = z = 0.0f; r = g = b = 0.5f; // grey color
    }
    SVertex(GLfloat x_, GLfloat y_, GLfloat z_, GLfloat w_ ,
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
    GLint width;
    GLint height;
    int * parts_sizes;
    int glProgram;
    Normal ** normals; // first dim[pointer]: part_index, second dim[GLfloat]: normals per part
    GLuint **  faces; //firrst dim[pointer]: part_index, second dim[GLubyte]: indeces to vertices per part
    SVertex * vertices;
    GLuint shaderIdx_a_position;
    GLuint shaderIdx_a_color;
    GLuint shaderIdx_a_normals[];
    GLuint shaderIdx_a_diffColor;
    GLuint shaderIdx_c_Perspective;
    GLuint shaderIdx_u_C;
    GLuint shaderIdx_u_R;
    GLuint shaderIdx_u_S;
    GLuint shaderIdx_u_P;
    GLuint shaderIdx_u_dirToLight;
    GLfloat xcenter;
    GLfloat ycenter;
    GLfloat zcenter;
    ESMatrix scaleOriginal;
    ESMatrix c_Perspective;
    ESMatrix u_C;
    ESMatrix u_R;
    ESMatrix u_S;
    ESMatrix u_P;
    GLfloat u_dirToLight[4];
    GLfloat scaleF;
    AppCtx();
    ~AppCtx();
};

/////// Functions ////////

void viewValuesSetUp(AppCtx * c);
void releaseVertexRes(AppCtx * c);
void renderFrame(AppCtx * c);
void renderTestFrame(AppCtx * c);
void renderTestFrame2(AppCtx * c);
void zoom(AppCtx * c, float z);
void rotateAnchor(AppCtx * c, float dx, float dy);
bool setupGraphics(AppCtx * c);
void LOGm(ESMatrix *esm, AppCtx *c);
SVertex* Mat_x_Vertex(ESMatrix * esm, SVertex * v);
void LogArrayF(const char * arrName, float * arr, int length);
void LogArrayGLui(const char * arrName, GLuint * arr, int length);
void LogVertices(AppCtx *c);

#endif
