// OpenGL ES 2.0 C++ code
#include <jni.h>
#include <android/log.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <stdio.h>


#include "natRenderer.h"
#include "esUtils.h"


#define VERTEX_POS_INDX 0
#define VERTEX_POS_SIZE 3

#define INDEX_A_POSITION 	0
#define INDEX_A_COLOR 		1
#define INDEX_U_MVP 		0

// helper functions
void loadAttributes(AppCtx * c);
void viewValuesSetUp(AppCtx * c);



static const char gVertexShader[] =
    "uniform mat4 u_mvpMatrix;                  \n"
    "attribute vec4 a_position;					\n"
    "attribute vec4 a_color;					\n"
    "varying vec4   v_color;					\n"
    "void main() {						        \n"
	"  v_color = a_color;				        \n"
	"  gl_Position =  a_position;  \n"
//	"  gl_Position = u_mvpMatrix * a_position;  \n"
    "}								            \n";

static const char gFragmentShader[] = 
    "precision mediump float;		            \n"
	"varying vec4 v_color;			            \n"
    "void main() {					            \n"
//    "  gl_FragColor = v_color;                  \n"
    "  gl_FragColor = vec4( 1.0, 0.0, 0.0, 1.0 );                  \n"
    "}                                          \n";

/////////// renderFrame ////////////
void renderFrame(AppCtx * c) {
    checkGlError("Before renderFrame");
    // TODO load colors
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    checkGlError("glClear");

    LOGI("renderFrame 1");
    glUniformMatrix4fv(INDEX_U_MVP , 1, GL_FALSE, (GLfloat*) &c->mvpMatrix.m[0][0]);
	checkGlError("glUniformMatrix4fv");

    LOGI("renderFrame 2");
    for(int i=0; i < c->parts_number; ++i) {
        glDrawElements(GL_TRIANGLES, c->parts_sizes[i], GL_UNSIGNED_BYTE, c->faces[i]);
        checkGlError("glDrawElements");
    }
    // TODO buffering
    LOGI("renderFrame end");
}


void renderTestFrame(AppCtx *c) {
	GLfloat gTriangleVertices[] = { 0.0f, 0.5f, -0.5f, -0.5f,
        0.5f, -0.5f };
    float grey = 0.5;

    glClearColor(grey, grey, grey, 1.0f);
    checkGlError("glClearColor");
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    checkGlError("glClear");

    glUseProgram(c->glProgram);
    checkGlError("glUseProgram");

    GLuint gvPositionHandle = glGetAttribLocation(c->glProgram, "a_position");
    checkGlError("glGetAttribLocation");

    glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, gTriangleVertices);
    checkGlError("glVertexAttribPointer");
    glEnableVertexAttribArray(gvPositionHandle);
    checkGlError("glEnableVertexAttribArray");
    glDrawArrays(GL_TRIANGLES, 0, 3);
    checkGlError("glDrawArrays");
}


void viewValuesSetUp(AppCtx *c) {
    glViewport(0, 0, c->width, c->height);
    checkGlError("glViewport");

    float aspect = (GLfloat) c->width / c->height;
    ESMatrix perspective;
    ESMatrix modelView;
    esMatrixLoadIdentity(&perspective);
    esPerspective(&perspective, 40.0f, aspect,1.0f, 200.0f);
    esMatrixLoadIdentity(&modelView);
    esTranslate(&modelView, 0.0f, 0.0f, -20.0f);
    esMatrixMultiply(&c->mvpMatrix, &modelView, &perspective);

    LOGI("viewValueSetUp end");
}

/////////// setupGraphics /////////
bool setupGraphics(AppCtx * c) {
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

//    glEnable(GL_CULL_FACE);
//    checkGlError("glEnable(GL_CULL_FACE)");

    c->glProgram = createProgram(gVertexShader, gFragmentShader);
    glUseProgram(c->glProgram);
    checkGlError("glUseProgram");
    if (!c->glProgram) {
        LOGE("Could not create program.");
        return false;
    }

	viewValuesSetUp(c);
    loadAttributes(c);

    LOGI("setupGraphics(%d, %d) end", c->width, c->height);
    return true;
}

/////// loadAttributes
void loadAttributes(AppCtx * c) {
    glBindAttribLocation(c->glProgram, INDEX_U_MVP,"u_mvpMatrix");
    checkGlError("glBindAttribLocation .. u_mvpMatrix");
    glBindAttribLocation(c->glProgram, INDEX_A_POSITION, "a_position");
    checkGlError("glBindAttribLocation .. a_position");
    glBindAttribLocation(c->glProgram, INDEX_A_COLOR, "a_color");
    checkGlError("glBindAttribLocation .. a_color");

    // TODO reinitialize the colors
//    glVertexAttribPointer(INDEX_A_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(SVertex)+offset TODO, c->vertices);

    GLfloat red[4] = {1.0f,0.0f,0.0f,1.0f};
    glVertexAttrib4fv(INDEX_A_COLOR,red);
    checkGlError("glVertexAttrib4fv");

    glUniformMatrix4fv(INDEX_U_MVP , 1, GL_FALSE, (GLfloat*) &c->mvpMatrix.m[0][0] );
    checkGlError("glUniformMatrix4fv .. mvp");

    glVertexAttribPointer(INDEX_A_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(SVertex), c->vertices);
    checkGlError("glVertexAttribPointer");
    glEnableVertexAttribArray(INDEX_A_POSITION);
    checkGlError("glEnableVertexAttribArray");

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    LOGI("loadAttributes end");
}

void zoom(AppCtx * c, float z) {
    esTranslate(&c->mvpMatrix, 0.0, 0.0, -z);
}

void rotateAnchor(AppCtx * c, float dx, float dy) {
    esRotate(&c->mvpMatrix, dx, 1.0, 0.0, 0.0);
    esRotate(&c->mvpMatrix, dy, 0.0, 0.1, 0.0);
}

//////////////////// not really opengl functions ///////////

/////////// AppCtx::AppCtx ////////////
AppCtx::AppCtx() {
	this->glProgram; this->numVertices = this->parts_number = 0;
	this->parts_sizes = NULL;
	this->faces = NULL;
	this->vertices = NULL;
}
/////////// AppCtx::~AppCtx ////////////
AppCtx::~AppCtx() {
    releaseResources(this);
}

/////////// releaseResources ////////////
void releaseResources(AppCtx * c) {
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
}


/*
mat4 LookAt(const vec3& eye, const vec3& target, const vec3& up)
{
    vec3 z = (eye - target).Normalized();
    vec3 x = up.Cross(z).Normalized();
    vec3 y = z.Cross(x).Normalized();

    mat4 m;
    m.x = vec4(x, 0);
    m.y = vec4(y, 0);
    m.z = vec4(z, 0);
    m.w = vec4(0, 0, 0, 1);

    vec4 eyePrime = m * -eye;
    m = m.Transposed();
    m.w = eyePrime;

    return m;
}

*/




