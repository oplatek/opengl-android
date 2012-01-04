#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "natRenderer.h"




//int ESUTIL_API esGenCube ( float scale, GLfloat **vertices, GLfloat **normals,
//                           GLfloat **texCoords, GLuint **indices )
void  esGenCube ( float scale, AppCtx * c) {
    int i;
    int numVertices = 24;
    int numIndices = 36;

    GLfloat cubeVerts[] =
    {
      -0.5f, -0.5f, -0.5f,
      -0.5f, -0.5f,  0.5f,
      0.5f, -0.5f,  0.5f,
      0.5f, -0.5f, -0.5f,
      -0.5f,  0.5f, -0.5f,
      -0.5f,  0.5f,  0.5f,
      0.5f,  0.5f,  0.5f,
      0.5f,  0.5f, -0.5f,
      -0.5f, -0.5f, -0.5f,
      -0.5f,  0.5f, -0.5f,
      0.5f,  0.5f, -0.5f,
      0.5f, -0.5f, -0.5f,
      -0.5f, -0.5f, 0.5f,
      -0.5f,  0.5f, 0.5f,
      0.5f,  0.5f, 0.5f, 
      0.5f, -0.5f, 0.5f,
      -0.5f, -0.5f, -0.5f,
      -0.5f, -0.5f,  0.5f,
      -0.5f,  0.5f,  0.5f,
      -0.5f,  0.5f, -0.5f,
      0.5f, -0.5f, -0.5f,
      0.5f, -0.5f,  0.5f,
      0.5f,  0.5f,  0.5f,
      0.5f,  0.5f, -0.5f,
    };

    GLfloat cubeNormals[] =
    {
      0.0f, -1.0f, 0.0f,
      0.0f, -1.0f, 0.0f,
      0.0f, -1.0f, 0.0f,
      0.0f, -1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, -1.0f,
      0.0f, 0.0f, -1.0f,
      0.0f, 0.0f, -1.0f,
      0.0f, 0.0f, -1.0f,
      0.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 1.0f,
      -1.0f, 0.0f, 0.0f,
      -1.0f, 0.0f, 0.0f,
      -1.0f, 0.0f, 0.0f,
      -1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
    };

    GLfloat cubeTex[] =
    {
      0.0f, 0.0f,
      0.0f, 1.0f,
      1.0f, 1.0f,
      1.0f, 0.0f,
      1.0f, 0.0f,
      1.0f, 1.0f,
      0.0f, 1.0f,
      0.0f, 0.0f,
      0.0f, 0.0f,
      0.0f, 1.0f,
      1.0f, 1.0f,
      1.0f, 0.0f,
      0.0f, 0.0f,
      0.0f, 1.0f,
      1.0f, 1.0f,
      1.0f, 0.0f,
      0.0f, 0.0f,
      0.0f, 1.0f,
      1.0f, 1.0f,
      1.0f, 0.0f,
      0.0f, 0.0f,
      0.0f, 1.0f,
      1.0f, 1.0f,
      1.0f, 0.0f,
    };

    GLuint cubeIndices[] =
    {
     0, 2, 1,
     0, 3, 2, 
     4, 5, 6,
     4, 6, 7,
     8, 9, 10,
     8, 10, 11, 
     12, 15, 14,
     12, 14, 13, 
     16, 17, 18,
     16, 18, 19, 
     20, 23, 22,
     20, 22, 21
    };
   

	c->numVertices = numVertices/3;
	c->vertices = new SVertex[c->numVertices]; // important to release before it
	int t;

	for( i=0; i < c->numVertices; ++i) {
		t = 3*i; // tripple times to index i
		c->vertices[i] = 
                SVertex(cubeVerts[t]*scale, cubeVerts[t+1]*scale, cubeVerts[t+2]*scale,1.0f);
	}

    c->parts_number = 1;
    c->parts_sizes = new int [c->parts_number];
    c->parts_sizes[0] = c->numVertices;

	c->faces = new GLubyte*[c->parts_number];
	c->normals = new Normal*[c->parts_number];

	for(int i = 0; i < c->parts_number; i++) {
		 c->faces[i] = new GLubyte[c->parts_sizes[i]];
		 c->normals[i] = new Normal[c->parts_sizes[i]];
		 for(int j = 0; j < c->parts_sizes[i]; j++) { // each part could have different number of vertices
			c->faces[i][j] = cubeIndices[j];
            t = 3 * j;
			c->normals[i][j]= Normal(cubeNormals[t], cubeNormals[t+1], cubeNormals[t+2]);
		 }
    }

    // todo TEXTURES
//   if ( texCoords != NULL ) {
//      *texCoords = malloc ( sizeof(GLfloat) * 2 * numVertices );
//      memcpy( *texCoords, cubeTex, sizeof( cubeTex ) ) ;
//   }

}
