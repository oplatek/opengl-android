// NpenGL ES 2.0 C++ code
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <stdio.h>

#include "natRenderer.h"
#include "esUtils.h"
#include "shaders.h"

#define VERTEX_POS_INDX 0
#define VERTEX_POS_SIZE 3

#define INDEX_A_POSITION     0
#define INDEX_A_COLOR         1
#define INDEX_U_MVP         0

#define Z_NEAR              1.0f
#define Z_FAR               100.0f
#define ANGLE               45.0f
#define ROTATION_ANGLE      35.0f
#define TANGENS             0.414f 
#define FRUS_COEF           0.2f

//// DO NOT FORGET UPDATE ATTRIBUTES AT ////
void bindShaderAttr(AppCtx *c) {
    c->shaderIdx_a_position=  glGetAttribLocation(c->glProgram, "a_position");
    LOGI("pointer shaderIdx_a_position = %d", c->shaderIdx_a_position);
    checkGlError("glGetAttribLocation a_position");
    c->shaderIdx_a_color =  glGetAttribLocation(c->glProgram, "a_color");
    LOGI("pointer shaderIdx_a_color = %d", c->shaderIdx_a_color);
    checkGlError("glGetAttribLocation a_color");
    c->shaderIdx_a_normals[0] =  glGetAttribLocation(c->glProgram, "a_normal");
    LOGI("pointer shaderIdx_a_normals[0] = %d", c->shaderIdx_a_normals[0]);
    checkGlError("glGetAttribLocation a_normal");
    c->shaderIdx_c_Perspective = glGetUniformLocation(c->glProgram, "c_Perspective");
    checkGlError("glGetAttribLocation c_Perspective");
    c->shaderIdx_u_C = glGetUniformLocation(c->glProgram, "u_C");
    checkGlError("glGetAttribLocation u_C");
    c->shaderIdx_u_R = glGetUniformLocation(c->glProgram, "u_R");
    checkGlError("glGetAttribLocation u_R");
    c->shaderIdx_u_S = glGetUniformLocation(c->glProgram, "u_S");
    checkGlError("glGetAttribLocation u_S");
    c->shaderIdx_u_P = glGetUniformLocation(c->glProgram, "u_P");
    checkGlError("glGetAttribLocation u_P");
    c->shaderIdx_u_dirToLight = glGetUniformLocation(c->glProgram, "u_dirToLight");
    checkGlError("glGetAttribLocation dirToLight");
}
//// DO NOT FORGET UPDATE ATTRIBUTES AT ////

// HELPER FUNCTIONS declarations
void loadAttributes(AppCtx * c);
void modelViewBoundaries(SVertex * verArr, int sizeArr, GLfloat * rxmin, GLfloat * rxmax, GLfloat  * rymin, GLfloat * rymax, GLfloat  * rzmin, GLfloat * rzmax);
GLfloat diameter(SVertex * verArr, int sizeArr, GLfloat xcenter, GLfloat ycenter, GLfloat zcenter);
// functions IMPLEMENTATION

void renderFrame(AppCtx * c) {
    glClearColor(0.5f,0.5f,0.5f, 1.0f);
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    checkGlError("glClear");

    // todo c_Perspective matrix should not be updated every time uniform->constant
    glUniformMatrix4fv(c->shaderIdx_c_Perspective, 1, GL_FALSE, (GLfloat*) &c->c_Perspective.m[0][0]);
    checkGlError("glUniformMatrix4fv c_Perspective");
    glUniformMatrix4fv(c->shaderIdx_u_C, 1, GL_FALSE, (GLfloat*) &c->u_C.m[0][0]);
    checkGlError("glUniformMatrix4fv u_C");
    glUniformMatrix4fv(c->shaderIdx_u_R, 1, GL_FALSE, (GLfloat*) &c->u_R.m[0][0]);
    checkGlError("glUniformMatrix4fv u_R");
    glUniformMatrix4fv(c->shaderIdx_u_S, 1, GL_FALSE, (GLfloat*) &c->u_S.m[0][0]);
    checkGlError("glUniformMatrix4fv u_S");
    glUniformMatrix4fv(c->shaderIdx_u_P, 1, GL_FALSE, (GLfloat*) &c->u_P.m[0][0]);
    checkGlError("glUniformMatrix4fv u_P");
    glUniform3fv(c->shaderIdx_u_dirToLight,1, &c->u_dirToLight.v[0]);
    checkGlError("glUniform4fv u_dirToLight");

    for(int i=0; i < c->parts_number; ++i) {
        glDrawElements(GL_TRIANGLES, c->parts_sizes[i], GL_UNSIGNED_INT, c->faces[i]);
        checkGlError("glDrawElements");
    }
    // TODO buffering
}

/////////// setupGraphics /////////
bool setupGraphics(AppCtx * c) {
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

    c->glProgram = createProgram(gVertexShader, gFragmentShader);
    if (!c->glProgram) {
        LOGE("Could not create program.");
        return false;
    }
    glUseProgram(c->glProgram);
    checkGlError("glUseProgram");

    loadAttributes(c);

    glEnable(GL_CULL_FACE);
    checkGlError("glEnable(GL_CULL_FACE)");

    LOGI("setupGraphics end");
    return true;
}


void viewValuesSetUp(AppCtx *c) {
    glViewport(0, 0, c->width, c->height);
    checkGlError("glViewport");

    float aspect = (GLfloat) c->width / c->height;
//    LOGI("aspect %f",aspect);

    esMatrixLoadIdentity(&c->c_Perspective);
    esPerspective(&c->c_Perspective, ANGLE, aspect, Z_NEAR, Z_FAR);
//    LOGI("perspective");
//    logMatrix(&c->c_Perspective);

    GLfloat xmin, ymin, zmin, xmax, ymax, zmax;
    modelViewBoundaries(c->vertices,c->numVertices,&xmin,&xmax,&ymin,&ymax,&zmin,&zmax);
//    LOGI("xmin: %f\n, xmax: %f\n, ymin: %f\n, ymax: %f\n, zmin: %f\n, zmax: %f",xmin,xmax,ymin,ymax,zmin,zmax);

    c->xcenter = (xmax + xmin) / 2;
    c->ycenter = (ymax + ymin) / 2;
    c->zcenter = (zmax + zmin) / 2;
    GLfloat diam = diameter(c->vertices, c->numVertices, c->xcenter,c->ycenter,c->zcenter);

    // Centering
    esMatrixLoadIdentity(&c->u_C);
    esTranslate(&c->u_C, -c->xcenter, -c->ycenter, -c->zcenter);
//    LOGI("Centering");
//    logMatrix(&c->u_C);

    // Rotating
    esMatrixLoadIdentity(&c->u_R);
//    esRotate( &c->u_R, ROTATION_ANGLE, 1.0, 0.0, 1.0 );
//    LOGI("rotate");
//    logMatrix(&c->u_R);

    // Scaling
    c->scaleF = 1.0f; // 100% of scaling -default value
    esMatrixLoadIdentity(&c->u_S);
    float scale = 0.5f *FRUS_COEF *TANGENS * Z_FAR / diam;
//    LOGI("diameter: %f scale: %f",diam, scale);
    esScale(&c->u_S, scale, scale, scale);
    c->scaleOriginal = c->u_S; // u_S is gone change based on scaleOriginal
//    LOGI("scale");
//    logMatrix(&c->u_S);

    // Positioning 
    esMatrixLoadIdentity(&c->u_P);
    esTranslate(&c->u_P, 0, 0, -FRUS_COEF * Z_FAR );
//    LOGI("translate");
//    logMatrix(&c->u_P);

//    LogArrayGLui("indeces", c->faces[0],c->parts_sizes[0]);

//    LogVertices(c);

    // light
//    esVectorLoad(&c->u_dirToLight, 0.866f, 0.5f, 0.0f, 0.0f);
    esVectorLoad(&c->u_dirToLight, 5.0f, 5.0f, 0.0f, 0.0f);
    LOGI("viewValueSetUp end");
}


/////// loadAttributes //////////
void loadAttributes(AppCtx * c) {
    bindShaderAttr(c);

    // TODO reinitialize the colors
    GLfloat red[4] = {1.0f,0.0f,0.0f,1.0f};
    glVertexAttrib4fv(c->shaderIdx_a_color,red);
    checkGlError("glVertexAttrib4fv");

    // TODO check correctness
//    for(int i = 0; i < c->parts_number; ++i){  // I do not know how to render more normals in vertex shaders -> I do not want to dynamicly change vertex shaders according number of parts_number 
    glVertexAttribPointer(c->shaderIdx_a_normals[0], 3, GL_FLOAT, GL_FALSE, sizeof(Normal), c->normals[0]);
    checkGlError("glVertexAttribPointer normals");
    glEnableVertexAttribArray(c->shaderIdx_a_normals[0]);
    checkGlError("glEnableVertexAttribArray a_normals");

    glVertexAttribPointer(c->shaderIdx_a_position, 4, GL_FLOAT, GL_FALSE, sizeof(SVertex), c->vertices);
    checkGlError("glVertexAttribPointer vertices");
    glEnableVertexAttribArray(c->shaderIdx_a_position);
    checkGlError("glEnableVertexAttribArray a_position");

    LOGI("loadAttributes end");
}

void modelViewBoundaries(SVertex * verArr, int sizeArr, GLfloat * rxmin, GLfloat * rxmax, GLfloat  * rymin, GLfloat * rymax, GLfloat  * rzmin, GLfloat * rzmax) {
    if (sizeArr < 1) {
        return;
    }
    GLfloat xmin, xmax, ymin, ymax, zmin, zmax;
    xmin = xmax= verArr[0].x;
    ymin = ymax= verArr[0].y;
    zmin = zmax= verArr[0].z;
    for (int i = 1; i < sizeArr; ++i) {
        if(verArr[i].x < xmin ) {
            xmin = verArr[i].x;
        }
        if(verArr[i].x > xmax ) {
            xmax = verArr[i].x;
        }
        if(verArr[i].y < ymin ) {
            ymin = verArr[i].y;
        }
        if(verArr[i].y > ymax ) {
            ymax = verArr[i].y;
        }
        if(verArr[i].z < zmin ) {
            zmin = verArr[i].z;
        }
        if(verArr[i].z > zmax ) {
            zmax = verArr[i].z;
        }
    }
    *rxmax = xmax;
    *rxmin = xmin;
    *rymax = ymax;
    *rymin = ymin;
    *rzmax = zmax;
    *rzmin = zmin;
}

GLfloat diameter(SVertex * v, int sizeArr, float xcenter, GLfloat ycenter, GLfloat zcenter) {
    GLfloat diameter = 0;
    GLfloat aktd, x,y,z;
    for (int i = 1; i < sizeArr; ++i) {
        x = v[i].x - xcenter; 
        y = v[i].y- ycenter; 
        z = v[i].z- zcenter; 
        aktd = (x * x) + (y * y) + (z * z);  
        if(aktd > diameter) {
            diameter = aktd; 
        }
    }
    diameter = sqrt(diameter);
    return diameter;
}

void zoom(AppCtx * c, float z) {
    c->scaleF =  c->scaleF + (0.1*z);
    c->scaleF = c->scaleF < 0.5f ? 0.5f : c->scaleF;
    c->scaleF = c->scaleF > 2.0f ? 2.0f : c->scaleF;
//    LOGI("c->scaleF = %f", c->scaleF);
    c->u_S = c->scaleOriginal;
    esScale(&c->u_S, c->scaleF, c->scaleF, c->scaleF);
}

void rotateAnchor(AppCtx * c, float dx, float dy) {
    esRotate(&c->u_R, -dx, 1.0, 0.0, 0.0);
    esRotate(&c->u_R, -dy, 0.0, 0.1, 0.0);
}

//void renderTestFrame(AppCtx *c) {
//    glClearColor(0.0f,0.0f,0.0f, 1.0f);
//    checkGlError("glClearColor");
//    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
//    checkGlError("glClear");
//
//    esMatrixLoadIdentity(&c->mvpMatrix);
//    glUniformMatrix4fv(c->shaderIdx_u_mvpMatrix , 1, GL_FALSE, (GLfloat*) &c->mvpMatrix.m[0][0]);
//    checkGlError("glUniformMatrix4fv");
//
//    for(int i=0; i < c->parts_number; ++i) {
//        glDrawElements(GL_TRIANGLES, c->parts_sizes[i], GL_UNSIGNED_INT, c->faces[i]);
//        checkGlError("glDrawElements");
//    }
//}

