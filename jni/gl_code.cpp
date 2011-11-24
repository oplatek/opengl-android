// OpenGL ES 2.0 C++ code

#include <jni.h>
#include <android/log.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gl_code.h"


static const char gVertexShader[] = 
    "attribute vec4 vPosition;		\n"
    "void main() {			\n"
    "  gl_Position = vPosition;		\n"
    "}					\n";

static const char gFragmentShader[] = 
    "precision mediump float;                           \n"
    "void main() {					\n"
    "  gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);		\n"
    "}							\n";

/////////// renderFrame ////////////
void renderFrame() {
    static float grey;
    grey += 0.01f;
    if (grey > 1.0f) {
        grey = 0.0f;
    }
    glClearColor(grey/3, grey/2, grey, 1.0f);
    checkGlError("glClearColor");
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    checkGlError("glClear");

    glUseProgram(gProgram);
    checkGlError("glUseProgram");

    glEnableVertexAttribArray(gvPositionHandle);
    checkGlError("glEnableVertexAttribArray");
    glDrawArrays(GL_TRIANGLES, 0, numTriangle);
    checkGlError("glDrawArrays");
}

/////////// setupGraphics /////////
bool setupGraphics(int w, int h, float * raw_vertices, int raw_size) {
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

    LOGI("setupGraphics(%d, %d)", w, h);
    gProgram = createProgram(gVertexShader, gFragmentShader);
    if (!gProgram) {
        LOGE("Could not create program.");
        return false;
    }
    gvPositionHandle = glGetAttribLocation(gProgram, "vPosition");
    checkGlError("glGetAttribLocation");
    LOGI("glGetAttribLocation(\"vPosition\") = %d\n",
            gvPositionHandle);

    glViewport(0, 0, w, h);
    checkGlError("glViewport");

    // loading vertices on the remaing part of function
    releaseResources(); // if there has been stored another Verteces clean them
    numTriangle = raw_size/3;
    Vertices = new SVertex[numTriangle]; // important to release before it

    int t=0;
    for(int i=0; i < numTriangle; ++i) {
            t = 3*i; // tripple times to index i
            Vertices[i] = SVertex(raw_vertices[t], raw_vertices[t+1], raw_vertices[t+2]);
            Vertices[i].LOG(i);
    }
    // reinitialize the vertexes after setting the opengl environment
    glVertexAttribPointer(gvPositionHandle, numTriangle, GL_FLOAT, GL_FALSE, sizeof(SVertex),Vertices );
    checkGlError("glVertexAttribPointer");

    numTriangle = 3; // TODO I am able to display only first 3 vertices..boo

    // TODO reinitialize the colors
    glVertexAttribPointer(gvPositionHandle, numTriangle, GL_FLOAT, GL_FALSE, sizeof(SVertex), Vertices);
    checkGlError("glVertexAttribPointer");

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

    GLuint program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        checkGlError("glAttachShader");
        glAttachShader(program, pixelShader);
        checkGlError("glAttachShader");
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char* buf = (char*) malloc(bufLength);
                if (buf) {
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    LOGE("Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}

/////////// releaseResources ////////////
void releaseResources() {
	if(Vertices != NULL) {
		delete[] Vertices; // HEAP RELEASE
		Vertices = NULL; // IMPORTANT -> check allocation of Vertices
    }
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

/////////// JNICALL .._releaseCpp resources ////////////
JNIEXPORT void JNICALL Java_ondrej_platek_bind_BINDLib_releaseCppResources(JNIEnv * env, jobject) {
    releaseResources();
}

/////////// JNICALL .._init ////////////
JNIEXPORT void JNICALL Java_ondrej_platek_bind_BINDLib_init(JNIEnv * env, jobject mythis) {
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
JNIEXPORT void JNICALL Java_ondrej_platek_bind_BINDLib_step(JNIEnv * env, jobject obj) {
    renderFrame(); // called from BINDView.Renderer.onDrawFrame
}
