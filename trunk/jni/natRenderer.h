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
//    GLfloat r,g,b;
//    GLfloat t0_s,t0_t; // 1. texture coordinates
//    GLfloat t1_s,t1_t; // 2. texture coordinates
    SVertex() {
        w = 1.0f; x = y = z = 0.0f;
//        r = g = b = 0.5f; // grey color
    }
    SVertex(GLfloat x_, GLfloat y_, GLfloat z_, GLfloat w_ 
//            ,GLfloat r_ = 0.5f, GLfloat g_ = 0.5f, GLfloat b_ = 0.5f
            ) {
	  x = x_; y = y_; z = z_; w = w_; 
//      r = r_; g = g_; b = b_;
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
    Normal * normals; // first dim[pointer]: part_index, second dim[GLfloat]: normals per part
    GLuint **  faces; //firrst dim[pointer]: part_index, second dim[GLubyte]: indeces to vertices per part
    GLuint ** normalsPointer;
    SVertex * vertices;
    GLuint shaderIdx_a_position;
    GLuint * shaderIdx_a_normals;
    GLuint shaderIdx_u_mvpMatrix;
    GLuint shaderIdx_u_normalMatrix;
    GLuint shaderIdx_u_eyePos;
    GLuint shaderIdx_u_lightPos;
    GLuint shaderIdx_u_lightColor;
    GLuint shaderIdx_u_matAmbient;
    GLuint shaderIdx_u_matDiffuse;
    GLuint shaderIdx_u_matSpecular;
    GLuint shaderIdx_u_matShininess;
    GLfloat xcenter;
    GLfloat ycenter;
    GLfloat zcenter;
    ESMatrix scaleOriginal;
    ESMatrix c_Perspective;
    ESMatrix u_C;
    ESMatrix u_R;
    ESMatrix u_S;
    ESMatrix u_P;
    ESMatrix mvp;
    ESVector u_eyePos;    // eye Position
    ESVector u_lightPos;  // light position
    ESVector u_lightColor; 
    ESVector u_matAmbient; // material ambient
    ESVector u_matDiffuse; // material diffuse
    ESVector u_matSpecular; // material Specular
    GLfloat u_matShininess; // material shininess
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
