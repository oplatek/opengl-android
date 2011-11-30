// OpenGL ES 2.0 C++ code
#include <jni.h>
#include <android/log.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <stdio.h>

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <android/log.h>
//#include "esUtils.h"

#include "natRenderer.h"
//#include "esUtils.h"

/////////// loadShader ////////////
GLuint loadShader(GLenum shaderType, const char* pSource) {
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
        glShaderSource(shader, 1, &pSource, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = (char*) malloc(infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    LOGE("Could not compile shader %d:\n%s\n", shaderType, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    checkGlError("loadShader end");
    return shader;
}


/////////// createProgram ////////////
GLuint createProgram(const char* pVertexSource, const char* pFragmentSource) {
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
    if (!vertexShader) {
        return 0;
    }

    GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
    if (!pixelShader) {
        return 0;
    }

    GLuint glProgram = glCreateProgram();
    if (glProgram) {
        glAttachShader(glProgram, vertexShader);
        checkGlError("glAttachShader");
        glAttachShader(glProgram, pixelShader);
        checkGlError("glAttachShader");

        glLinkProgram(glProgram);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(glProgram, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(glProgram, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char* buf = (char*) malloc(bufLength);
                if (buf) {
                    glGetProgramInfoLog(glProgram, bufLength, NULL, buf);
                    LOGE("Could not link glProgram:\n%s\n", buf);
                    free(buf);
                }
            }
            glDeleteProgram(glProgram);
            glProgram = 0;
        }
    }
    checkGlError("create Program end");
    return glProgram;
}

/////////// printGLString ////////////
void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}

/////////// checkGlError ////////////
void checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}

/*
///////////////////// openGL matrix function ///////////////
void esTranslate(ESMatrix *result, GLfloat tx, GLfloat ty, GLfloat tz)
{
    result->m[3][0] += (result->m[0][0] * tx + result->m[1][0] * ty + result->m[2][0] * tz);
    result->m[3][1] += (result->m[0][1] * tx + result->m[1][1] * ty + result->m[2][1] * tz);
    result->m[3][2] += (result->m[0][2] * tx + result->m[1][2] * ty + result->m[2][2] * tz);
    result->m[3][3] += (result->m[0][3] * tx + result->m[1][3] * ty + result->m[2][3] * tz);
}

void esMatrixMultiply(ESMatrix *result, ESMatrix *srcA, ESMatrix *srcB)
{
    ESMatrix    tmp;
    int         i;

	for (i=0; i<4; i++)
	{
		tmp.m[i][0] =	(srcA->m[i][0] * srcB->m[0][0]) +
						(srcA->m[i][1] * srcB->m[1][0]) +
						(srcA->m[i][2] * srcB->m[2][0]) +
						(srcA->m[i][3] * srcB->m[3][0]) ;

		tmp.m[i][1] =	(srcA->m[i][0] * srcB->m[0][1]) +
						(srcA->m[i][1] * srcB->m[1][1]) +
						(srcA->m[i][2] * srcB->m[2][1]) +
						(srcA->m[i][3] * srcB->m[3][1]) ;

		tmp.m[i][2] =	(srcA->m[i][0] * srcB->m[0][2]) +
						(srcA->m[i][1] * srcB->m[1][2]) +
						(srcA->m[i][2] * srcB->m[2][2]) +
						(srcA->m[i][3] * srcB->m[3][2]) ;

		tmp.m[i][3] =	(srcA->m[i][0] * srcB->m[0][3]) +
						(srcA->m[i][1] * srcB->m[1][3]) +
						(srcA->m[i][2] * srcB->m[2][3]) +
						(srcA->m[i][3] * srcB->m[3][3]) ;
	}
    memcpy(result, &tmp, sizeof(ESMatrix));
}

void esRotate(ESMatrix *result, GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
   GLfloat sinAngle, cosAngle;
   GLfloat mag = sqrtf(x * x + y * y + z * z);

   sinAngle = sinf ( angle * PI / 180.0f );
   cosAngle = cosf ( angle * PI / 180.0f );
   if ( mag > 0.0f )
   {
      GLfloat xx, yy, zz, xy, yz, zx, xs, ys, zs;
      GLfloat oneMinusCos;
      ESMatrix rotMat;

      x /= mag;
      y /= mag;
      z /= mag;

      xx = x * x;
      yy = y * y;
      zz = z * z;
      xy = x * y;
      yz = y * z;
      zx = z * x;
      xs = x * sinAngle;
      ys = y * sinAngle;
      zs = z * sinAngle;
      oneMinusCos = 1.0f - cosAngle;

      rotMat.m[0][0] = (oneMinusCos * xx) + cosAngle;
      rotMat.m[0][1] = (oneMinusCos * xy) - zs;
      rotMat.m[0][2] = (oneMinusCos * zx) + ys;
      rotMat.m[0][3] = 0.0F;

      rotMat.m[1][0] = (oneMinusCos * xy) + zs;
      rotMat.m[1][1] = (oneMinusCos * yy) + cosAngle;
      rotMat.m[1][2] = (oneMinusCos * yz) - xs;
      rotMat.m[1][3] = 0.0F;

      rotMat.m[2][0] = (oneMinusCos * zx) - ys;
      rotMat.m[2][1] = (oneMinusCos * yz) + xs;
      rotMat.m[2][2] = (oneMinusCos * zz) + cosAngle;
      rotMat.m[2][3] = 0.0F;

      rotMat.m[3][0] = 0.0F;
      rotMat.m[3][1] = 0.0F;
      rotMat.m[3][2] = 0.0F;
      rotMat.m[3][3] = 1.0F;

      esMatrixMultiply( result, &rotMat, result );
   }
}


void esFrustum(ESMatrix *result, float left, float right, float bottom, float top, float nearZ, float farZ) {
    float       deltaX = right - left;
    float       deltaY = top - bottom;
    float       deltaZ = farZ - nearZ;
    ESMatrix    frust;

    if ( (nearZ <= 0.0f) || (farZ <= 0.0f) ||
         (deltaX <= 0.0f) || (deltaY <= 0.0f) || (deltaZ <= 0.0f) )
         return;

    frust.m[0][0] = 2.0f * nearZ / deltaX;
    frust.m[0][1] = frust.m[0][2] = frust.m[0][3] = 0.0f;

    frust.m[1][1] = 2.0f * nearZ / deltaY;
    frust.m[1][0] = frust.m[1][2] = frust.m[1][3] = 0.0f;

    frust.m[2][0] = (right + left) / deltaX;
    frust.m[2][1] = (top + bottom) / deltaY;
    frust.m[2][2] = -(nearZ + farZ) / deltaZ;
    frust.m[2][3] = -1.0f;

    frust.m[3][2] = -2.0f * nearZ * farZ / deltaZ;
    frust.m[3][0] = frust.m[3][1] = frust.m[3][3] = 0.0f;

    esMatrixMultiply(result, &frust, result);
}

void esMatrixLoadIdentity(ESMatrix *result)
{
    memset(result, 0x0, sizeof(ESMatrix));
    result->m[0][0] = 1.0f;
    result->m[1][1] = 1.0f;
    result->m[2][2] = 1.0f;
    result->m[3][3] = 1.0f;
}
void esPerspective(ESMatrix *result, float fovy, float aspect, float nearZ, float farZ)
{
   GLfloat frustumW, frustumH;

   frustumH = tanf( fovy / 360.0f * PI ) * nearZ;
   frustumW = frustumH * aspect;

   esFrustum( result, -frustumW, frustumW, -frustumH, frustumH, nearZ, farZ );
}
*/
#define VERTEX_POS_INDX 0
#define VERTEX_POS_SIZE 3

#define INDEX_A_POSITION 	0
#define INDEX_A_COLOR 		1

void loadAttributes(AppCtx * c);


struct SVertex {
    GLfloat x,y,z;
    GLfloat r,g,b;
//    GLfloat t0_s,t0_t; // 1. texture coordinates
//    GLfloat t1_s,t1_t; // 2. texture coordinates
    SVertex() {
              x = y = z = 0.0f; r = g = b = 0.5f; // grey color
    }
    SVertex(GLfloat x_, GLfloat y_, GLfloat z_,
            GLfloat r_ = 0.5f, GLfloat g_ = 0.5f, GLfloat b_ = 0.5f) {
	  x = x_; y = y_; z = z_; r = r_; g = g_; b = b_;
    }
    void LOG(int index) {
      LOGI("Vertex[%d] {x=%f,y=%f,z=%f,\n,r=%f,g=%f,b=%f}",index,x,y,z,r,g,b);
    }
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
    ESMatrix * mvpMatrix;
    AppCtx() {
    	this->glProgram; this->numVertices = this->parts_number = 0;
    	this->parts_sizes = NULL;
    	this->faces = NULL;
    	this->vertices = NULL;
    	this->mvpMatrix = NULL;
    }
    ~AppCtx();
};



//#define INDEX_U_MVP 		0
//    "uniform mat4 u_mvpMatrix;                   \n"
//	"   gl_Position = u_mvpMatrix * a_position;  \n"
static const char gVertexShader[] = 
    "attribute vec4 a_position;					\n"
    "attribute vec4 a_color;					\n"
    "varying vec4   v_color;					\n"
    "void main() {						        \n"
	"  v_color = a_color;				        \n"
	"   gl_Position = a_position;  				\n"
    "}								            \n";

static const char gFragmentShader[] = 
    "precision mediump float;		            \n"
	"varying vec4 v_color;			            \n"
    "void main() {					            \n"
    "  gl_FragColor = v_color;                  \n"
    "}                                          \n";

/////////// renderFrame ////////////
void renderFrame(AppCtx * c) {
    checkGlError("Before renderFrame");
    // TODO load colors
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    checkGlError("glClear");

//    glUniformMatrix4fv(INDEX_U_MVP , 1, GL_FALSE, (GLfloat*) c->mvpMatrix.m[0][0] );
//	checkGlError("glUniformMatrix4fv");

    for(int i=0; i < c->parts_number; ++i) {
        glDrawElements(GL_TRIANGLES, c->parts_sizes[i], GL_UNSIGNED_BYTE, c->faces[i]);
        checkGlError("glDrawElements");
    }
    // TODO buffering

}

void renderTestFrame(AppCtx * c){
    LOGI("renderTestFrame.");
	checkGlError("Before renderFrame");

    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    checkGlError("glClear");

    GLfloat testV[] = { 0.0f, 0.5f, 0.0f,-0.5f,-0.5f,0.0f,0.5f,-0.5f,0.0f};
    glVertexAttribPointer(INDEX_A_POSITION, 3, GL_FLOAT, GL_FALSE, 0, testV);
    checkGlError("glVertexAttribPointer");
    checkGlError("glEnableVertexAttribArray");

    glEnableVertexAttribArray(INDEX_A_POSITION);
    glDrawArrays(GL_TRIANGLES, 0, 3); // still drawing one triangle
    checkGlError("glDrawArrays");
}


/////////// setupGraphics /////////
bool setupGraphics(AppCtx * c) {
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

//    glEnable(GL_CULL_FACE);
//    checkGlError("glEnable(GL_CULL_FACE)");

    LOGI("setupGraphics(%d, %d)", c->width, c->height);
    GLuint gProgram = createProgram(gVertexShader, gFragmentShader);
    glUseProgram(gProgram);
    checkGlError("glUseProgram");
    if (!gProgram) {
        LOGE("Could not create program.");
        return false;
    }

    loadAttributes(c);

    glViewport(0, 0, c->width, c->height);
    checkGlError("glViewport");

    float aspect = (GLfloat) c->width / c->height;
//    ESMatrix perspective;
//    ESMatrix modelView;
//    esMatrixLoadIdentity(&perspective);
//    esPerspective(&perspective, 40.0f, aspect,1.0f, 200.0f);
//    esMatrixLoadIdentity(&modelView);
//    esTranslate(&modelView, 0.0f, 0.0f, -20.0f);
//    esMatrixMultiply(c->mvpMatrix, &modelView, &perspective);
    checkGlError("Matrix setup");

    return true;
}

/////// loadAttributes
void loadAttributes(AppCtx * c) {
//    glBindAttribLocation(c->glProgram, INDEX_U_MVP,"u_mvpMatrix");
//    checkGlError("glBindAttribLocation .. u_mvpMatrix");
    glBindAttribLocation(c->glProgram, INDEX_A_POSITION, "a_position");
    checkGlError("glBindAttribLocation .. a_position");
    glBindAttribLocation(c->glProgram, INDEX_A_COLOR, "a_color");
    checkGlError("glBindAttribLocation .. a_color");

    // TODO reinitialize the colors
//    glVertexAttribPointer(INDEX_A_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(SVertex)+offset TODO, c->vertices);

    GLfloat red[4] = {1.0f,1.0f,0.0f,1.0f};
    glVertexAttrib4fv(INDEX_A_COLOR,red);
    checkGlError("glVertexAttrib4fv");

//    glUniformMatrix4fv(INDEX_U_MVP , 1, GL_FALSE, (GLfloat*) c->mvpMatrix.m[0][0] );
//    checkGlError("glUniformMatrix4fv .. mvp");


    glVertexAttribPointer(INDEX_A_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(SVertex), c->vertices);
    checkGlError("glVertexAttribPointer");
    glEnableVertexAttribArray(INDEX_A_POSITION);
    checkGlError("glEnableVertexAttribArray");

}

void zoom(AppCtx * c, float z) {
//    esTranslate(c->mvpMatrix, 0.0, 0.0, -z);
}

void rotateAnchor(AppCtx * c, float dx, float dy) {
//    esRotate(c->mvpMatrix, dx, 1.0, 0.0, 0.0);
//    esRotate(c->mvpMatrix, dy, 0.0, 0.1, 0.0);
}

//////////////////// not really opengl functions ///////////

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


/////////// JNICALL .._init ////////////

// helper functions
int extractInt(JNIEnv * env, jobject mythis,const char * memberName);
jobject objForArray(JNIEnv * env, jobject mythis, const char * memberName,const char * type);
void returnInt(JNIEnv * env, jobject mythis,const char * memberName,int v);

JNIEXPORT void JNICALL Java_ondrej_platek_bind_NativeRenderer_init(JNIEnv * env, jobject mythis,jobjectArray Normals, jobjectArray Faces)  {
    LOGI("start of all program init: %d");

    const char * str ="pAppCtx";
    int ic = extractInt(env, mythis,"pAppCtx");
    LOGI("AppCtx pointer at beggining init: %d",ic);
    AppCtx * c =  reinterpret_cast<AppCtx*>(ic);
    if(c == NULL) { // create new AppCtx -> should be only 1 
        c = new AppCtx();
        LOGI("AppCtx pointer initialize in init: %d",c);
    }
    else {
    	releaseResources(c);
    }

    LOGI("init 1");

    int raw_size = extractInt(env, mythis, "vertices_size");
    c->height = extractInt(env, mythis, "height");
    c->width = extractInt(env, mythis, "width");
    c->parts_number = extractInt(env, mythis, "parts_number");

    LOGI("init 2");
    // jArrays -> I have to release them afterwards
    jobject mvdata = objForArray(env, mythis, "vertices", "[F");
    jfloatArray * arr = reinterpret_cast<jfloatArray*>(&mvdata);
    float * raw_vertices = env->GetFloatArrayElements(*arr, NULL);

    LOGI("init 3");

    mvdata = objForArray(env, mythis, "parts_sizes", "[I");
    jintArray * arr2 = reinterpret_cast<jintArray*>(&mvdata);
    int * raw_parts_sizes = env->GetIntArrayElements(*arr2, NULL);

    LOGI("init 4");

    LOGI("before vertices init");
    c->numVertices = raw_size/3;
    c->vertices = new SVertex[c->numVertices]; // important to release before it

    LOGI("init 5");

    int t;
    for(int i=0; i < c->numVertices; ++i) {
        t = 3*i; // tripple times to index i
        c->vertices[i] = SVertex(raw_vertices[t], raw_vertices[t+1], raw_vertices[t+2]);
        c->vertices[i].LOG(i);
    }

    LOGI("before parts_sizes");

    c->parts_sizes = new int[c->parts_number];
    for(int i = 0; i < c->parts_number; i++) {
    	c->parts_sizes[i] = raw_parts_sizes[i];
    }

    c->faces = new GLubyte*[c->parts_number];
    c->normals = new Normal*[c->parts_number];
    for(int i = 0; i < c->parts_number; i++) {
         jshortArray oneDimFaces = (jshortArray) env->GetObjectArrayElement(Faces, i);
         LOGI("Loaded c->faces step 1 parts number %d",c->parts_number);
         jfloatArray oneDimNormals = (jfloatArray)env->GetObjectArrayElement(Normals, i);
         jshort * arrshort =env->GetShortArrayElements(oneDimFaces, 0);
         LOGI("Loaded c->faces step 2 parts number %d", c->parts_number);
         jfloat * arrfloat =env->GetFloatArrayElements(oneDimNormals, 0);
         LOGI("c->parts_sizes %d",c->parts_sizes[i]);
         c->faces[i] = new GLubyte[c->parts_sizes[i]];
         LOGI("Loaded c->faces step 3 parts number %d", c->parts_number);
         c->normals[i] = new Normal[c->parts_sizes[i]];
         for(int j = 0; j < c->parts_sizes[i]; j++) { // each part could have different number of vertices
        	// copy to local arrays
        	LOGI("Loaded c->faces step 4 parts number %d forloop j %d",c->parts_number,j);
            c->faces[i][j] = arrshort[j];
            c->normals[i][j]= Normal( arrfloat[j], arrfloat[j+1], arrfloat[j+2] );
            LOGI("Part %d, Vertex %d: indc->vertices %d",i,j,c->faces[i][j]);
            LOGI("Part %d, Vertex %d: normal %f %f %f",i,j,c->faces[i][j],c->normals[i][j].x, c->normals[i][j].y, c->normals[i][j].z);
         }
    }

    setupGraphics(c);

    // Don't forget to release it
    env->ReleaseFloatArrayElements(*arr, raw_vertices, 0);
    env->ReleaseIntArrayElements(*arr2, raw_parts_sizes, 0);

    // return AppCtx c
    int retValue = reinterpret_cast<int>(c);
    LOGI("AppCtx pointer at the end of init: %d",retValue);
    returnInt(env, mythis,"pAppCtx", retValue);
} 
// end of Java_ondrej_platek_bind_NativeRenderer_init


/////////// JNICALL .._step ////////////
JNIEXPORT void JNICALL Java_ondrej_platek_bind_NativeRenderer_step(JNIEnv * env, jobject mythis) {
    AppCtx * c =  reinterpret_cast<AppCtx*>(extractInt(env, mythis, "pAppCtx"));
    if(c == NULL) {
      LOGE("NativeRender_step context is NULL");
    }
    else {
    renderTestFrame(c);
//    renderFrame(c);
    }
}
  
/////////// JNICALL .._Zoom ////////////
JNIEXPORT void JNICALL Java_ondrej_platek_bind_NativeRenderer_Zoom(JNIEnv * env, jobject mythis, float z) {
    AppCtx * c =  reinterpret_cast<AppCtx*>(extractInt(env, mythis, "pAppCtx"));
    if(c == NULL) {
        LOGE("NativeRender_Zoom context is NULL");
    }
    else {
        zoom(c,z);
    }
}

/////////// JNICALL .._RotateAnchor ////////////
JNIEXPORT void JNICALL Java_ondrej_platek_bind_NativeRenderer_RotateAnchor(JNIEnv * env, jobject mythis, float dx, float dy) { 
    // LOGI("rotating with dx, dy by %f %f", dx,dy);
    AppCtx * c =  reinterpret_cast<AppCtx*>(extractInt(env, mythis, "pAppCtx"));
    if(c == NULL) {
        LOGE("NativeRender_RotateAnchor context is NULL");
    } 
    else {
        rotateAnchor(c, dx, dy);
    }
}

/////////// JNICALL .._releaseCpp resources ////////////
JNIEXPORT void JNICALL Java_ondrej_platek_bind_NativeRenderer_releaseCppResources(JNIEnv * env, jobject mythis) {
    AppCtx * c =  reinterpret_cast<AppCtx*>(extractInt(env, mythis, "pAppCtx"));
    if(c == NULL) {
      LOGE("NativeRender_releaseCppResources context is NULL");
    }
    releaseResources(c);
}

int extractInt(JNIEnv * env, jobject mythis,const char * memberName) {
    jclass cls = env->GetObjectClass(mythis);
    jfieldID fieldID_integer = env->GetFieldID(cls, memberName, "I");
    // integer we do not have to extract from container env->GetIntField..
    return env->GetIntField(mythis, fieldID_integer);
}

void returnInt(JNIEnv * env, jobject mythis,const char * memberName, int v) {
    jclass cls = env->GetObjectClass(mythis);
    jfieldID fieldID_integer = env->GetFieldID(cls, memberName, "I");
    env->SetIntField(mythis, fieldID_integer, v);
}

jobject objForArray(JNIEnv * env, jobject mythis, const char * memberName,const char * type) {
    jclass cls = env->GetObjectClass(mythis);
    jfieldID fieldID_raw_vertices = env->GetFieldID(cls, memberName, type);
    env->GetFieldID(cls,memberName,type);
    return env->GetObjectField(mythis, fieldID_raw_vertices);
}


//jint JNI_OnLoad(JavaVM* vm, void* reserved)
//{
//    JNIEnv *env;
//    LOGI("JNI_OnLoad called");
//    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
//        LOGE("Failed to get the environment using GetEnv()");
//        return -1;
//    }
//    return JNI_VERSION_1_4;
//}
