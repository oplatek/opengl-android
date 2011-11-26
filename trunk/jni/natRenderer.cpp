// OpenGL ES 2.0 C++ code

#include <jni.h>
#include <android/log.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "natRenderer.h"

#define INDEX_A_POSITION 0
#define INDEX_A_COLOR 1

static const char gVertexShader[] = 
    "attribute vec4 a_position;		\n"
    "attribute vec4 a_color;		\n"
    "varying vec4   v_color;		\n"
    "void main() {					\n"
	"  v_color = a_color;			\n"
    "  gl_Position = a_position;	\n"
    "}								\n";

static const char gFragmentShader[] = 
    "precision mediump float;		\n"
	"varying vec4 v_color;			\n"
    "void main() {					\n"
    "  gl_FragColor = v_color;		\n"
    "}								\n";

/////////// renderFrame ////////////
void renderFrame() {
	checkGlError("Before renderFrame");
	// TODO load colors
    glClearColor(0.5, 0.5f, 0.5f,1.0f);
    checkGlError("glClearColor");
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    checkGlError("glClear");

    // TODO buffering

    glDrawArrays(GL_TRIANGLES, 0, numTriangle); // still drawing one triangle
    checkGlError("glDrawArrays");
}

void loadAttributes(float * raw_vertices, int raw_size, GLuint glProgram) {

    // loading vertices on the remaing part of function
    releaseResources(); // if there has been stored another Verteces clean them
    numTriangle = raw_size/3;
    Vertices = new SVertex[numTriangle]; // important to release before it

    int t=0;
    float sc = 0.2;
    for(int i=0; i < numTriangle; ++i) {
            t = 3*i; // tripple times to index i
            Vertices[i] = SVertex(raw_vertices[t]*sc, raw_vertices[t+1]*sc, raw_vertices[t+2]*sc);
            Vertices[i].LOG(i);
    }

    // TODO reinitialize the colors
    GLfloat red[4] = {1.0f,0.0f,0.0f,1.0f};
    glVertexAttrib4fv(INDEX_A_COLOR,red);
    checkGlError("glVertexAttrib4fv");

    glVertexAttribPointer(INDEX_A_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(SVertex), Vertices);
    checkGlError("glVertexAttribPointer");
    glEnableVertexAttribArray(INDEX_A_POSITION);
    checkGlError("glEnableVertexAttribArray");

    // todo str 101
    glBindAttribLocation(glProgram, INDEX_A_COLOR, "a_color");
    checkGlError("glBindAttribLocation .. a_color");
    glBindAttribLocation(glProgram, INDEX_A_POSITION, "a_position");
    checkGlError("glBindAttribLocation .. a_position");
}

/////////// setupGraphics /////////
bool setupGraphics(int w, int h, float * raw_vertices, int raw_size) {
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

    LOGI("setupGraphics(%d, %d)", w, h);
    GLuint gProgram = createProgram(gVertexShader, gFragmentShader, raw_vertices, raw_size);
    glUseProgram(gProgram);

    checkGlError("glUseProgram");
    if (!gProgram) {
        LOGE("Could not create program.");
        return false;
    }

    glViewport(0, 0, w, h);
    checkGlError("glViewport");

    // TODO lookAt function

    return true;
}

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
                    LOGE("Could not compile shader %d:\n%s\n",
                            shaderType, buf);
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
GLuint createProgram(const char* pVertexSource, const char* pFragmentSource,float * raw_vertices, int raw_size) {
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

        // Important
		loadAttributes(raw_vertices, raw_size, glProgram);

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
static void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}

/////////// checkGlError ////////////
static void checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error
            = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}

//////////////////// not really opengl functions ///////////

/////////// releaseResources ////////////
void releaseResources() {
	if(Vertices != NULL) {
		delete[] Vertices; // HEAP RELEASE
		Vertices = NULL; // IMPORTANT -> check allocation of Vertices
    }
}

/////////// JNICALL .._releaseCpp resources ////////////
JNIEXPORT void JNICALL Java_ondrej_platek_bind_NativeRenderer_releaseCppResources(JNIEnv * env, jobject) {
    releaseResources();
}

/////////// JNICALL .._init ////////////
JNIEXPORT void JNICALL Java_ondrej_platek_bind_NativeRenderer_init(JNIEnv * env, jobject mythis) {
    // Get the class associated with this object
    jclass cls = env->GetObjectClass(mythis);

    // Get the integer field associated with the size.
    // Data type signature ("I") is very important.
    // I for integers, D for double, [I for array of integers, [D for array of doubles...

    jfieldID fieldID_integer = env->GetFieldID(cls, "vertexes_size", "I");
    jint integer_container = env->GetIntField(mythis, fieldID_integer);
  int raw_size = integer_container;

    jfieldID fieldID_integer2 = env->GetFieldID(cls, "height", "I");
    jint integer_container2 = env->GetIntField(mythis, fieldID_integer2);
    int height = integer_container2;

    jfieldID fieldID_integer3 = env->GetFieldID(cls, "width", "I");
    jint integer_container3 = env->GetIntField(mythis, fieldID_integer3);
    int width = integer_container3;

    // Get the integer array field associated with the unsorted array. Data type signature "[I" is very important
    jfieldID fieldID_floatarray = env->GetFieldID(cls, "vertexes", "[F");
    // we need object
    jobject mvdata = env->GetObjectField(mythis, fieldID_floatarray);

    // Cast it to a jfloatarray
    jfloatArray * arr = reinterpret_cast<jfloatArray*>(&mvdata);

    // Get the elements (you probably have to fetch the length of the array as well
    float * raw_vertices = env->GetFloatArrayElements(*arr, NULL);

    setupGraphics(width, height, raw_vertices, raw_size); // called from BINDView.Renderer.onSurfaceChanged

    // Don't forget to release it
    env->ReleaseFloatArrayElements(*arr, raw_vertices, 0);
}

/////////// JNICALL .._step ////////////
JNIEXPORT void JNICALL Java_ondrej_platek_bind_NativeRenderer_step(JNIEnv * env, jobject obj) {
    renderFrame(); // called from BINDView.Renderer.onDrawFrame
}
