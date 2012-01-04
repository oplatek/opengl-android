#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "natRenderer.h"


void  loadAppCtx ( float scale, AppCtx * c, int numVertices, int numIndices, 
                   GLfloat * cubeVerts, GLfloat * cubeNormals, GLuint * cubeIndices);

void genCube1( float scale, AppCtx * c) {
    int numVertices = 8;
    int numIndices = 36;
    GLfloat cubeVerts[] ={ 
      0.0f,  0.0f,  0.0f,
      0.0f,  0.0f,  1.0f,
      0.0f,  1.0f,  0.0f,
      0.0f,  1.0f,  1.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  1.0f,
      1.0f,  1.0f,  0.0f,
      1.0f,  1.0f,  1.0f,
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
    GLuint cubeIndices[] = {
     0, 6, 4,
     0, 2, 6,
     0, 3, 2,
     0, 1, 3,
     2, 7, 6,
     2, 3, 7,
     4, 6, 7,
     4, 7, 5,
     0, 4, 5,
     0, 5, 1,
     1, 5, 7,
     1, 7, 3
    };
    loadAppCtx (scale, c, numVertices, numIndices, cubeVerts,  cubeNormals,  cubeIndices);
}

void genCube2(float scale, AppCtx *c){
    // multiple redefinition of vertices
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
//    GLfloat cubeTex[] = {
//      0.0f, 0.0f,
//      0.0f, 1.0f,
//      1.0f, 1.0f,
//      1.0f, 0.0f,
//      1.0f, 0.0f,
//      1.0f, 1.0f,
//      0.0f, 1.0f,
//      0.0f, 0.0f,
//      0.0f, 0.0f,
//      0.0f, 1.0f,
//      1.0f, 1.0f,
//      1.0f, 0.0f,
//      0.0f, 0.0f,
//      0.0f, 1.0f,
//      1.0f, 1.0f,
//      1.0f, 0.0f,
//      0.0f, 0.0f,
//      0.0f, 1.0f,
//      1.0f, 1.0f,
//      1.0f, 0.0f,
//      0.0f, 0.0f,
//      0.0f, 1.0f,
//      1.0f, 1.0f,
//      1.0f, 0.0f,
//    };
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
    loadAppCtx(scale, c, numVertices, numIndices, cubeVerts,  cubeNormals,  cubeIndices);
}

void  loadAppCtx ( float scale, AppCtx * c, int numVertices, int numIndices, GLfloat * cubeVerts, GLfloat * cubeNormals, GLuint * cubeIndices) {
    int i; // indexer in for loops
	int t; // indexer in for loops usually t = 3*i (tripple)

	c->numVertices = numVertices;
	c->vertices = new SVertex[c->numVertices]; // important to release before it

	for( i=0; i < c->numVertices; ++i) {
		t = 3*i; // tripple times to index i
		c->vertices[i] = 
                SVertex(cubeVerts[t]*scale, cubeVerts[t+1]*scale, cubeVerts[t+2]*scale,1.0f);
	}

    c->parts_number = 1;
    c->parts_sizes = new int [c->parts_number];
    c->parts_sizes[0] = numIndices;

	c->faces = new GLuint*[c->parts_number];
	c->normals = new Normal*[c->parts_number];

	for(int i = 0; i < c->parts_number; i++) {
		 c->faces[i] = new GLuint[c->parts_sizes[i]];
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

