#include <stdlib.h>
#include "esUtils.h"
#include "esX11Util.h"
#include "natRenderer.h"
#include "def.h"

typedef struct
{
   // Handle to a program object
   GLuint programObject;

   // Attribute locations
   GLint  positionLoc;

   // Uniform locations
   GLint  mvpLoc;
   
   // Vertex daata
   GLfloat  *vertices;
   GLuint *indices;
   int       numIndices;

   // Rotation angle
   GLfloat   angle;

   // MVP matrix
   ESMatrix  mvpMatrix;
} UserData;

///
// Initialize the shader and program object
//
int Init ( ESContext *esContext ) {

    esContext->c = new AppCtx();

    AppCtx *c = esContext->c;
    const char vShaderStr[] =  
      "uniform mat4 u_mvpMatrix;                   \n"
      "attribute vec4 a_position;                  \n"
      "void main()                                 \n"
      "{                                           \n"
      "   gl_Position = u_mvpMatrix * a_position;  \n"
      "}                                           \n";

    const char fShaderStr[] =  
      "precision mediump float;                            \n"
      "void main()                                         \n"
      "{                                                   \n"
      "  gl_FragColor = vec4( 1.0, 0.0, 0.0, 1.0 );        \n"
      "}                                                   \n";

    test(fShaderStr);
    // Load the shaders and get a linked program object
    c->glProgram = createProgram ( vShaderStr, fShaderStr );

    // Get the attribute locations
    c->shaderIdx_a_position = glGetAttribLocation ( c->glProgram, "a_position" );

    // Get the uniform locations
    c->shaderIdx_u_mvpMatrix = glGetUniformLocation( c->glProgram, "u_mvpMatrix" );

    // Generate the vertex data
    todo
    c->numIndices = esGenCube( 1.0, &userData->vertices,

    glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );
    return GL_TRUE;
}


///
// Update MVP matrix based on time
//
void Update ( ESContext *esContext, float deltaTime ) {
   AppCtx *c = esContext->c;
   ESMatrix perspective;
   ESMatrix modelview;
   float    aspect;
   
   // Compute a rotation angle based on time to rotate the cube
//   userData->angle += ( deltaTime * 40.0f );
   float angle = 66.0f;
//   if( userData->angle >= 360.0f )
//      userData->angle -= 360.0f;

   // Compute the window aspect ratio
   aspect = (GLfloat) esContext->width() / (GLfloat) esContext->height();
   
   // Generate a perspective matrix with a 60 degree FOV
   esMatrixLoadIdentity( &perspective );
   esPerspective( &perspective, 60.0f, aspect, 1.0f, 20.0f );

   // Generate a model view matrix to rotate/translate the cube
   esMatrixLoadIdentity( &modelview );

   // Translate away from the viewer
   esTranslate( &modelview, 0.0, 0.0, -2.0 );

   // Rotate the cube
//   esRotate( &modelview, userData->angle, 1.0, 0.0, 1.0 );
   esRotate( &modelview, angle, 1.0, 0.0, 1.0 );
   
   // Compute the final MVP by multiplying the 
   // modevleiw and perspective matrices together
   esMatrixMultiply( &c->mvpMatrix, &modelview, &perspective );
}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw ( ESContext *esContext )
{
   AppCtx *c = esContext->c;
   
   // Set the viewport
   glViewport ( 0, 0, esContext->width(), esContext->height() );
   
   
   // Clear the color buffer
   glClear ( GL_COLOR_BUFFER_BIT );

   // Use the program object
   glUseProgram ( c->glProgram );

   // Load the vertex position
   glVertexAttribPointer ( c->shaderIdx_a_position, 3, GL_FLOAT, 
                           GL_FALSE, sizeof(SVertex), c->vertices );
   
   glEnableVertexAttribArray ( c->shaderIdx_a_position );
   
   
   // Load the MVP matrix
   glUniformMatrix4fv( c->shaderIdx_u_mvpMatrix, 1, GL_FALSE, (GLfloat*) &c->mvpMatrix.m[0][0] );
   
    for(int i=0; i < c->parts_number; ++i) {
        glDrawElements(GL_TRIANGLES, c->parts_sizes[i], GL_UNSIGNED_BYTE, c->faces[i]);
        checkGlError("glDrawElements");
    }
}

int main ( int argc, char *argv[] )
{
   ESContext esContext;
   struct AppCtx c;

   esInitContext ( &esContext );
   esContext.c = &c;

   esCreateWindow ( &esContext, "Simple Texture 2D", 320, 240, ES_WINDOW_RGB );

   if ( !Init ( &esContext ) )
      return 0;

   esRegisterDrawFunc ( &esContext, Draw );
   esRegisterUpdateFunc ( &esContext, Update );

   esMainLoop ( &esContext );

}

