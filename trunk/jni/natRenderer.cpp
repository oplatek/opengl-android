// NpenGL ES 2.0 C++ code
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


 void bindShaderAttr(AppCtx *c) {
	 c->shaderIdx_a_position=  glGetAttribLocation(c->glProgram, "a_position");
	 c->shaderIdx_a_color =  glGetAttribLocation(c->glProgram, "a_color");
	 c->shaderIdx_u_mvpMatrix = glGetUniformLocation(c->glProgram, "u_mvpMatrix");
 }

static const char gVertexShader[] =
    "uniform mat4 u_mvpMatrix;                  \n"
    "attribute vec4 a_position;					\n"
    "attribute vec4 a_color;					\n"
    "varying vec4   v_color;					\n"
    "void main() {						        \n"
	"  v_color = a_color;				        \n"
	"  gl_Position = u_mvpMatrix * a_position;  \n"
    "}								            \n";

static const char gFragmentShader[] = 
    "precision mediump float;		            \n"
	"varying vec4 v_color;			            \n"
    "void main() {					            \n"
    "  gl_FragColor = v_color;                  \n"
    "}                                          \n";


void renderFrame(AppCtx * c) {
    checkGlError("Before renderFrame");
    // TODO load colors
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    checkGlError("glClear");

    glUniformMatrix4fv(c->shaderIdx_u_mvpMatrix , 1, GL_FALSE, (GLfloat*) &c->mvpMatrix.m[0][0]);
	checkGlError("glUniformMatrix4fv");

    for(int i=0; i < c->parts_number; ++i) {
        glDrawElements(GL_TRIANGLES, c->parts_sizes[i], GL_UNSIGNED_BYTE, c->faces[i]);
        checkGlError("glDrawElements");
    }
    // TODO buffering
}

void renderTestFrame(AppCtx *c) {
    glClearColor(0.0f,0.0f,0.0f, 1.0f);
    checkGlError("glClearColor");
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    checkGlError("glClear");

    esMatrixLoadIdentity(&c->mvpMatrix);
    glUniformMatrix4fv(c->shaderIdx_u_mvpMatrix , 1, GL_FALSE, (GLfloat*) &c->mvpMatrix.m[0][0]);
	checkGlError("glUniformMatrix4fv");

    for(int i=0; i < c->parts_number; ++i) {
        glDrawElements(GL_TRIANGLES, c->parts_sizes[i], GL_UNSIGNED_BYTE, c->faces[i]);
        checkGlError("glDrawElements");
    }
}


/////////// setupGraphics /////////
bool setupGraphics(AppCtx * c) {
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);


    c->glProgram = createProgram(gVertexShader, gFragmentShader);
    glUseProgram(c->glProgram);
    checkGlError("glUseProgram");
    if (!c->glProgram) {
        LOGE("Could not create program.");
        return false;
    }

    loadAttributes(c);

    LOGI("setupGraphics end");
    return true;
}


void viewValuesSetUp(AppCtx *c) {
    glViewport(0, 0, c->width, c->height);
    checkGlError("glViewport");

    float aspect = (GLfloat) c->width / c->height;
    LOGI("aspect %f",aspect);

    ESMatrix perspective;
    esMatrixLoadIdentity(&perspective);
    esPerspective(&perspective, 45.0f, aspect,1.0f, 200.0f);
    LOGI("perspective");
    logMatrix(&perspective);

	GLfloat xmin, ymin, zmin, xmax, ymax, zmax;
	modelViewBoundaries(c->vertices,c->numVertices,&xmin,&xmax,&ymin,&ymax,&zmin,&zmax);

    LOGI("xmin: %f\n, xmax: %f\n, ymin: %f\n, ymax: %f\n, zmin: %f\n, zmax: %f",xmin,xmax,ymin,ymax,zmin,zmax);

	GLfloat xdiff = (xmax - xmin);
	GLfloat ydiff = (ymax - ymin);
	GLfloat zdiff = (zmax - zmin);
	GLfloat xcenter = xdiff / 2;
	GLfloat ycenter = ydiff / 2;
	GLfloat zcenter = zdiff / 2;

	ESMatrix T; // translate
	esMatrixLoadIdentity(&T);
	esTranslate(&T,-xcenter,-ycenter,-zcenter);
    LOGI("translate");
    logMatrix(&T);

	ESMatrix S; // scale
	esMatrixLoadIdentity(&S);
	if((xdiff != 0) && (ydiff != 0) && (zdiff != 0) ) {
		esScale(&S, (1.0f / xdiff), (1.0f / ydiff), (1.0f / zdiff));
	}
    LOGI("translate");
    logMatrix(&T);

	ESMatrix R; // rotate
	esMatrixLoadIdentity(&R);
	// todo start up rotation
    LOGI("rotate");
    logMatrix(&R);

	ESMatrix modelView;
	esMatrixMultiply(&modelView, &R, &T);
	esMatrixMultiply(&modelView, &S, &modelView);
//    LOGI("modelView");
//    logMatrix(&modelView);


    // todo put to translate
	esTranslate(&modelView, 0.0f, 0.0f, -20.0f);

    esMatrixMultiply(&c->mvpMatrix, &modelView, &perspective);
    LOGI("result matrix");
    logMatrix(&c->mvpMatrix);
    LOGm(&c->mvpMatrix, c);
    LOGI("viewValueSetUp end");
}


/////// loadAttributes
void loadAttributes(AppCtx * c) {
    bindShaderAttr(c);

    glEnable(GL_CULL_FACE);
    checkGlError("glEnable(GL_CULL_FACE)");

    // TODO reinitialize the colors
//    glVertexAttribPointer(INDEX_A_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(SVertex)+offset TODO, c->vertices);
    GLfloat red[4] = {1.0f,0.0f,0.0f,1.0f};
    glVertexAttrib4fv(c->shaderIdx_a_color,red);
    checkGlError("glVertexAttrib4fv");

    glVertexAttribPointer(c->shaderIdx_a_position, 4, GL_FLOAT, GL_FALSE, sizeof(SVertex), c->vertices);
    checkGlError("glVertexAttribPointer");

    glEnableVertexAttribArray(c->shaderIdx_a_position);
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
}

// should be only for debugging
SVertex* Mat_x_Vertex(ESMatrix * esm, SVertex * v) {
	SVertex tmp;
	tmp.x = (esm->m[0][0] * v->x) +(esm->m[0][1] * v->y) +(esm->m[0][2] * v->z) +(esm->m[0][3] * 1) ;
	tmp.y = (esm->m[1][0] * v->x) +(esm->m[1][1] * v->y) +(esm->m[1][2] * v->z) +(esm->m[1][3] * 1) ;
	tmp.z = (esm->m[2][0] * v->x) +(esm->m[2][1] * v->y) +(esm->m[2][2] * v->z) +(esm->m[2][3] * 1) ;
	memcpy(v,&tmp,sizeof(tmp));
	return v;
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



