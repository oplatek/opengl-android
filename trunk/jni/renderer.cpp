// OpenGL ES 2.0 C++ code
//#include <jni.h>
//#include <android/log.h>
//
//#include <GLES2/gl2.h>
//#include <GLES2/gl2ext.h>
//
//#include <stdio.h>
//#include <stdlib.h>
//#include <math.h>

#include "renderer.h"
#include "esTransform.h"
#include "esUtils.h"

#define VERTEX_POS_INDX 0
#define VERTEX_POS_SIZE 3

#define INDEX_A_POSITION 	0
#define INDEX_A_COLOR 		1

void loadAttributes(AppCtx * c);

//#define INDEX_U_MVP 		0
//    "uniform mat4 u_mvpMatrix;                   \n"
//	"   gl_Position = u_mvpMatrix * a_position;  \n"
static const char gVertexShader[] = 
    "attribute vec4 a_position;					 \n"
    "attribute vec4 a_color;					 \n"
    "varying vec4   v_color;					 \n"
    "void main() {								 \n"
	"  v_color = a_color;					  	 \n"
	"   gl_Position = a_position;  				 \n"
    "}											 \n";

static const char gFragmentShader[] = 
    "precision mediump float;		\n"
	"varying vec4 v_color;			\n"
    "void main() {					\n"
    "  gl_FragColor = v_color;		\n"
    "}								\n";

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
        // TODO wtf?	gl.glDisableClientState(GL10.GL_NORMAL_ARRAY);
    }
    // TODO buffering

}

void renderTestFrame(AppCtx * c){
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
//        LOGE("Could not create program.");
        return false;
    }

    loadAttributes(c);

    glViewport(0, 0, c->width, c->height);
    checkGlError("glViewport");

    float aspect = (GLfloat) c->width / c->height;
    ESMatrix perspective;
    ESMatrix modelView;
    esMatrixLoadIdentity(&perspective);
    esPerspective(&perspective, 40.0f, aspect,1.0f, 200.0f);
    esMatrixLoadIdentity(&modelView);
    esTranslate(&modelView, 0.0f, 0.0f, -20.0f);
    esMatrixMultiply(c->mvpMatrix, &modelView, &perspective);
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
    //	gl.glTranslatef(0.0f, -1.2f, -z);	//Move down 1.2 Unit And Into The Screen 6.0
	esTranslate(c->mvpMatrix, 0.0, 0.0, -z);
}

void rotateAnchor(AppCtx * c, float dx, float dy) {
//	gl.glRotatef(xrot, 1.0f, 0.0f, 0.0f);	//X
//	gl.glRotatef(yrot, 0.0f, 1.0f, 0.0f);	//Y

	esRotate(c->mvpMatrix, dx, 1.0, 0.0, 0.0);
	esRotate(c->mvpMatrix, dy, 0.0, 0.1, 0.0);
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


int extractInt(JNIEnv * env, jobject mythis,const char * memberName) {
    jclass cls = env->GetObjectClass(mythis);
    jfieldID fieldID_integer = env->GetFieldID(cls, memberName, "I");
    // integer we do not have to extract from container env->GetIntField..
    return env->GetIntField(mythis, fieldID_integer);
}

jobject objForArray(JNIEnv * env, jobject mythis, const char * memberName,const char * type) {
	jclass cls = env->GetObjectClass(mythis);
    jfieldID fieldID_raw_vertices = env->GetFieldID(cls, memberName, type);
    env->GetFieldID(cls,"asdf","asdf");
    return env->GetObjectField(mythis, fieldID_raw_vertices);
}

/////////// JNICALL .._init ////////////
JNIEXPORT void JNICALL Java_ondrej_platek_bind_NativeRenderer_init(JNIEnv * env, jobject mythis,jobjectArray Normals, jobjectArray Faces)  {
	const char * str ="pAppCtx";
//    AppCtx * c =  reinterpret_cast<AppCtx*>(extractInt(env, mythis,str));
    AppCtx * c =  reinterpret_cast<AppCtx*>(extractInt(env, mythis,"pAppCtx"));
    if(c == NULL) { // create new AppCtx -> should be only 1
        AppCtx * c = new AppCtx();
    }
    else {
    	releaseResources(c);
    }

    int raw_size = extractInt(env, mythis, "vertexes_size");
    c->height = extractInt(env, mythis, "height");
    c->width = extractInt(env, mythis, "width");
    c->parts_number = extractInt(env, mythis, "parts_number");

    jobject mvdata = objForArray(env, mythis, "vertexes", "[F");
    jfloatArray * arr = reinterpret_cast<jfloatArray*>(&mvdata);
    float * raw_vertices = env->GetFloatArrayElements(*arr, NULL);

    mvdata = objForArray(env, mythis, "parts_sizes", "[I");
    jintArray * arr2 = reinterpret_cast<jintArray*>(&mvdata);
    jint * raw_parts_sizes = env->GetIntArrayElements(*arr2, NULL);

    c->numVertices = raw_size/3;
    c->vertices = new SVertex[c->numVertices]; // important to release before it

    int t;
    for(int i=0; i < c->numVertices; ++i) {
        t = 3*i; // tripple times to index i
        c->vertices[i] = SVertex(raw_vertices[t], raw_vertices[t+1], raw_vertices[t+2]);
        c->vertices[i].LOG(i);
    }

    c->parts_sizes = new int[c->parts_number];
    for(int i = 0; i < c->parts_number; i++) {
    	c->parts_sizes[i] = raw_parts_sizes[i];
    }

    c->faces = new GLubyte*[c->parts_number];
    c->normals = new Normal*[c->parts_number];
    for(int i = 0; i < c->parts_number; i++) {
         jshortArray oneDimFaces = (jshortArray) env->GetObjectArrayElement(Faces, i);
//         LOGI("Loaded c->faces step 1 parts number %d",c->parts_number);
         jfloatArray oneDimNormals = (jfloatArray)env->GetObjectArrayElement(Normals, i);
         jshort * arrshort =env->GetShortArrayElements(oneDimFaces, 0);
//         LOGI("Loaded c->faces step 2 parts number %d", c->parts_number);
         jfloat * arrfloat =env->GetFloatArrayElements(oneDimNormals, 0);
//         LOGI("c->parts_sizes %d",c->parts_sizes[i]);
         c->faces[i] = new GLubyte[c->parts_sizes[i]];
//         LOGI("Loaded c->faces step 3 parts number %d", c->parts_number);
         c->normals[i] = new Normal[c->parts_sizes[i]];
         for(int j = 0; j < c->parts_sizes[i]; j++) { // each part could have different number of vertices
        	// copy to local arrays
//        	LOGI("Loaded c->faces step 4 parts number %d forloop j %d",c->parts_number,j);
            c->faces[i][j] = arrshort[j];
            c->normals[i][j]= Normal( arrfloat[j], arrfloat[j+1], arrfloat[j+2] );
//            LOGI("Part %d, Vertex %d: indc->vertices %d",i,j,c->faces[i][j]);
//            LOGI("Part %d, Vertex %d: normal %f %f %f",i,j,c->faces[i][j],normals[i][j].x, normals[i][j].y, normals[i][j].z);
         }
      }
    setupGraphics(c);

    // TODO return AppCtx c

    // Don't forget to release it
    env->ReleaseFloatArrayElements(*arr, raw_vertices, 0);
    env->ReleaseIntArrayElements(*arr2, raw_parts_sizes, 0);
}


/////////// JNICALL .._step ////////////
JNIEXPORT void JNICALL Java_ondrej_platek_bind_NativeRenderer_step(JNIEnv * env, jobject mythis) {
    AppCtx * c =  reinterpret_cast<AppCtx*>(extractInt(env, mythis, "pAppCtx"));
    if(c == NULL) {
      LOGE("NativeRender_step context is NULL");
    }
    renderTestFrame(c);
    renderFrame(c);
}

/////////// JNICALL .._Zoom ////////////
JNIEXPORT void JNICALL Java_ondrej_platek_bind_NativeRenderer_Zoom(JNIEnv * env, jobject mythis, float z) {
    AppCtx * c =  reinterpret_cast<AppCtx*>(extractInt(env, mythis, "pAppCtx"));
    if(c == NULL) {
      LOGE("NativeRender_Zoom context is NULL");
    }
    zoom(c,z);
}

/////////// JNICALL .._RotateAnchor ////////////
JNIEXPORT void JNICALL Java_ondrej_platek_bind_NativeRenderer_RotateAnchor(JNIEnv * env, jobject mythis, float dx, float dy) {
    // LOGI("rotating with dx, dy by %f %f", dx,dy);
    AppCtx * c =  reinterpret_cast<AppCtx*>(extractInt(env, mythis, "pAppCtx"));
    if(c == NULL) {
      LOGE("NativeRender_RotateAnchor context is NULL");
    }
    rotateAnchor(c, dx, dy);
}

/////////// JNICALL .._releaseCpp resources ////////////
JNIEXPORT void JNICALL Java_ondrej_platek_bind_BINDView_releaseCppResources(JNIEnv * env, jobject mythis) {
    AppCtx * c =  reinterpret_cast<AppCtx*>(extractInt(env, mythis, "pAppCtx"));
    if(c == NULL) {
      LOGE("NativeRender_releaseCppResources context is NULL");
    }
    releaseResources(c);
}
