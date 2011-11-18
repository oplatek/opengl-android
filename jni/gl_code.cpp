// OpenGL ES 2.0 code

#include <jni.h>
#include <android/log.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define  LOG_TAG    "libbind"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

GLfloat * gTriangleVertices;

static void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}

static void checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error
            = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}

static const char gVertexShader[] = 
    "attribute vec4 vPosition;		\n"
    "void main() {					\n"
    "  gl_Position = vPosition;		\n"
    "}								\n";

static const char gFragmentShader[] = 
    "precision mediump float;						\n"
    "void main() {									\n"
    "  gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);		\n"
    "}												\n";

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

GLuint gProgram;
GLuint gvPositionHandle;

bool setupGraphics(int w, int h) {
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
    return true;
}

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

    glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, gTriangleVertices);
    checkGlError("glVertexAttribPointer");
    glEnableVertexAttribArray(gvPositionHandle);
    checkGlError("glEnableVertexAttribArray");
    glDrawArrays(GL_TRIANGLES, 0, 3);
    checkGlError("glDrawArrays");
}
void loadOBJ2GL(const char * filename) {
        LOGE("loadOBJ2GL obj filename is %s()\n", filename);
}

/* ---- do NOT FORGET to ADD new FUNCTIONS to EXTERN scope!! ----- */
extern "C" {
    JNIEXPORT void JNICALL Java_ondrej_platek_bind_BINDLib_init(JNIEnv * env, jobject obj,  jint width, jint height);
    JNIEXPORT void JNICALL Java_ondrej_platek_bind_BINDLib_step(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_ondrej_platek_bind_BINDLib_loadOBJ2GL(JNIEnv * env, jobject obj, jstring objfile);
};


JNIEXPORT void JNICALL Java_ondrej_platek_bind_BINDLib_init(JNIEnv * env, jobject obj,  jint width, jint height) {
    setupGraphics(width, height); // called from BINDView.Renderer.onSurfaceChanged
}

JNIEXPORT void JNICALL Java_ondrej_platek_bind_BINDLib_step(JNIEnv * env, jobject obj) {
    renderFrame(); // called from BINDView.Renderer.onDrawFrame
}

JNIEXPORT void JNICALL Java_ondrej_platek_bind_BINDLib_loadOBJ2GL(JNIEnv * env, jobject obj, jstring objfile) {
	// called from the onSurfaceCreated method(called only after creating glSurface context)

    //{ 0.0f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f };
	gTriangleVertices = new GLfloat[6]; // HEAP PUT
	gTriangleVertices[0] = 0.0f;
	gTriangleVertices[1] = 0.5f;
	gTriangleVertices[2] = -0.5f;
	gTriangleVertices[3] = -0.5f;
	gTriangleVertices[4] = 0.5f;
	gTriangleVertices[5] = -0.5f;

	const char* objfile_c = env->GetStringUTFChars(objfile,0); // HEAP PUT
    loadOBJ2GL(objfile_c);
    env->ReleaseStringUTFChars(objfile,objfile_c); //HEAP REALEASE

    delete[] gTriangleVertices; // HEAP RELEASE
}
