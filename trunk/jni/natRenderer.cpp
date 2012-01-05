// NpenGL ES 2.0 C++ code
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <stdio.h>

#include "natRenderer.h"
#include "esUtils.h"
#include "shaders.h"

#define VERTEX_POS_INDX 0
#define VERTEX_POS_SIZE 3

#define INDEX_A_POSITION 	0
#define INDEX_A_COLOR 		1
#define INDEX_U_MVP 		0

#define Z_NEAR              1.0f
#define Z_FAR               100.0f
#define ANGLE               45.0f
#define TANGENS             0.414f 
#define FRUS_COEF           0.2f

//// DO NOT FORGET UPDATE ATTRIBUTES AT ////
void bindShaderAttr(AppCtx *c) {
    c->shaderIdx_a_position=  glGetAttribLocation(c->glProgram, "a_position");
    c->shaderIdx_a_color =  glGetAttribLocation(c->glProgram, "a_color");
    c->shaderIdx_u_mvpMatrix = glGetUniformLocation(c->glProgram, "u_mvpMatrix");
}
//// DO NOT FORGET UPDATE ATTRIBUTES AT ////

// HELPER FUNCTIONS declarations
void loadAttributes(AppCtx * c);
void modelViewBoundaries(SVertex * verArr, int sizeArr, GLfloat * rxmin, GLfloat * rxmax, GLfloat  * rymin, GLfloat * rymax, GLfloat  * rzmin, GLfloat * rzmax);
GLfloat diameter(SVertex * verArr, int sizeArr, GLfloat xcenter, GLfloat ycenter, GLfloat zcenter);
// functions IMPLEMENTATION

void renderFrame(AppCtx * c) {
    checkGlError("Before renderFrame");
    // TODO load colors
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    checkGlError("glClear");

    glUniformMatrix4fv(c->shaderIdx_u_mvpMatrix, 1, GL_FALSE, (GLfloat*) &c->mvpMatrix.m[0][0]);
	checkGlError("glUniformMatrix4fv");

    for(int i=0; i < c->parts_number; ++i) {
        glDrawElements(GL_TRIANGLES, c->parts_sizes[i], GL_UNSIGNED_INT, c->faces[i]);
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
        glDrawElements(GL_TRIANGLES, c->parts_sizes[i], GL_UNSIGNED_INT, c->faces[i]);
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
    if (!c->glProgram) {
        LOGE("Could not create program.");
        return false;
    }
    glUseProgram(c->glProgram);
    checkGlError("glUseProgram");

    loadAttributes(c);

    LOGI("setupGraphics end");
    return true;
}


void viewValuesSetUp(AppCtx *c) {
    glViewport(0, 0, c->width, c->height);
    checkGlError("glViewport");

    float aspect = (GLfloat) c->width / c->height;
//    LOGI("aspect %f",aspect);

    ESMatrix perspective;
    esMatrixLoadIdentity(&perspective);
    esPerspective(&perspective, ANGLE, aspect, Z_NEAR, Z_FAR);
    
//    LOGI("perspective");
//    logMatrix(&perspective);

	GLfloat xmin, ymin, zmin, xmax, ymax, zmax;
	modelViewBoundaries(c->vertices,c->numVertices,&xmin,&xmax,&ymin,&ymax,&zmin,&zmax);
    LOGI("xmin: %f\n, xmax: %f\n, ymin: %f\n, ymax: %f\n, zmin: %f\n, zmax: %f",xmin,xmax,ymin,ymax,zmin,zmax);

	GLfloat xcenter = (xmax + xmin) / 2;
	GLfloat ycenter = (ymax + ymin) / 2;
	GLfloat zcenter = (zmax + zmin) / 2;
    GLfloat diam = diameter(c->vertices, c->numVertices, xcenter,ycenter,zcenter);

	ESMatrix C; // center 
	esMatrixLoadIdentity(&C);
	esTranslate(&C, -xcenter, -ycenter, -zcenter);
    LOGI("translate");
    logMatrix(&C);

	ESMatrix T; // translate
	esMatrixLoadIdentity(&T);
//	esTranslate(&T, -xcenter, -ycenter, ((Z_NEAR +Z_FAR)/-8.0f) - zmax);
	esTranslate(&T, 0, 0, -50 );
    LOGI("translate");
    logMatrix(&T);

//	ESMatrix S; // scale
//	esMatrixLoadIdentity(&S);
//	if((xdiff != 0) && (ydiff != 0) && (zdiff != 0) ) {
//		esScale(&S, (1.0f / xdiff), (1.0f / ydiff), (1.0f / zdiff));
//	}
//    LOGI("scale");
//    logMatrix(&S);

	ESMatrix R; // rotate
	esMatrixLoadIdentity(&R);
    esRotate( &R, 45, 1.0, 0.0, 1.0 );
    LOGI("rotate");
    logMatrix(&R);

	ESMatrix modelView;
	esMatrixLoadIdentity(&modelView);
//	esMatrixMultiply(&modelView, &T, &R);
//    esMatrixMultiply(&modelView, &modelView, &C);
//	esMatrixMultiply(&modelView, &S, &modelView);
//    LOGI("modelView");
//    logMatrix(&modelView);

	esTranslate(&modelView, 0, 0, -FRUS_COEF * Z_FAR );
    esRotate( &modelView, 30, 1.0, 0.0, 1.0 );
    float scale = 0.5f *FRUS_COEF *TANGENS * Z_FAR * diam;
    LOGI("diameter: %f scale: %f",diam, scale);
	esScale(&modelView, scale, scale, scale);
	esTranslate(&modelView, -xcenter, -ycenter, -zcenter);


    esMatrixMultiply(&c->mvpMatrix, &modelView, &perspective);
//    LOGI("result matrix");
//    logMatrix(&c->mvpMatrix);

//    LogArrayGLui("indeces", c->faces[0],c->parts_sizes[0]);

//    LogVertices(c);
//    LOGI("vertices after applying mvpMatrix");
//    LOGm(&c->mvpMatrix, c);

//    glEnable(GL_CULL_FACE);
//    checkGlError("glEnable(GL_CULL_FACE)");

    LOGI("viewValueSetUp end");
}


/////// loadAttributes //////////
void loadAttributes(AppCtx * c) {
    bindShaderAttr(c);

    // TODO reinitialize the colors
//    glVertexAttribPointer(INDEX_A_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(SVertex)+offset TODO, c->vertices);
    GLfloat red[4] = {1.0f,0.0f,0.0f,1.0f};
    glVertexAttrib4fv(c->shaderIdx_a_color,red);
    checkGlError("glVertexAttrib4fv");

    glVertexAttribPointer(c->shaderIdx_a_position, 4, GL_FLOAT, GL_FALSE, sizeof(SVertex), c->vertices);
    checkGlError("glVertexAttribPointer");

    glEnableVertexAttribArray(c->shaderIdx_a_position);
    checkGlError("glEnableVertexAttribArray");

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
