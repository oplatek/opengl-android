#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "esUtils.h"

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


void logMatrix(ESMatrix * m, const char * c) {
    LOGI(c);
	for (int i = 0; i < 4; ++i) {
		LOGI("( %f %f %f %f )",m->m[i][0],m->m[i][1],m->m[i][2],m->m[i][3]);
	}
}

void esMatrixInverse(ESMatrix *m) {
    ESMatrix tmp;
    memcpy(&tmp, m, sizeof(ESMatrix));
    esMatrixInversion(&tmp, m);
}


void esMatrixTranspose(ESMatrix *m) {
    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j< 4; ++j) {
            if(i != j) { // diagonal remains the same
                float tr = m->m[j][i]; 
                m->m[j][i] = m->m[i][j];
                m->m[i][j] = tr;
            }
        }
    }
}

void esTranslate(ESMatrix *result, GLfloat tx, GLfloat ty, GLfloat tz)
{
    result->m[3][0] += (result->m[0][0] * tx + result->m[1][0] * ty + result->m[2][0] * tz);
    result->m[3][1] += (result->m[0][1] * tx + result->m[1][1] * ty + result->m[2][1] * tz);
    result->m[3][2] += (result->m[0][2] * tx + result->m[1][2] * ty + result->m[2][2] * tz);
    result->m[3][3] += (result->m[0][3] * tx + result->m[1][3] * ty + result->m[2][3] * tz);
}

void esScale(ESMatrix *result, GLfloat sx, GLfloat sy, GLfloat sz)
{
    result->m[0][0] *= sx;
    result->m[0][1] *= sx;
    result->m[0][2] *= sx;
    result->m[0][3] *= sx;

    result->m[1][0] *= sy;
    result->m[1][1] *= sy;
    result->m[1][2] *= sy;
    result->m[1][3] *= sy;

    result->m[2][0] *= sz;
    result->m[2][1] *= sz;
    result->m[2][2] *= sz;
    result->m[2][3] *= sz;
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


// the result is put in Y
void esMatrixInversion(ESMatrix *A, ESMatrix *Inv) {
    double det = 1.0/CalcDeterminant4(A);

    for(int j = 0; j < 4; ++j) {
        for(int i = 0; i < 4; ++i) {
            Inv->m[i][j] = det * CalcDeterminant3(A, i, j);
            if( (i+j)%2 == 1 ) {
                Inv->m[i][j] = -Inv->m[i][j];
            }
        }
    }
}

double CalcDeterminant3(ESMatrix *m, int skip_i, int skip_j) {
    int i[3];
    int j[3];
    if(skip_i == 0) {
        i[0] = 1; i[1] = 2; i[2] = 3;
    } else if(skip_i == 1) {
        i[0] = 0; i[1] = 2; i[2] = 3;
    } else if(skip_i == 2) {
        i[0] = 0; i[1] = 1; i[2] = 3;
    } else if(skip_i == 3) {
        i[0] = 0; i[1] = 1; i[2] = 2;
    } 

    if(skip_j == 0) {
        j[0] = 1; j[1] = 2; j[2] = 3;
    } else if(skip_j == 1) {
        j[0] = 0; j[1] = 2; j[2] = 3;
    } else if(skip_j == 2) {
        j[0] = 0; j[1] = 1; j[2] = 3;
    } else if(skip_j == 3) {
        j[0] = 0; j[1] = 1; j[2] = 2;
    }
    return  
          m->m[i[0]][j[0]]*m->m[i[1]][j[1]]*m->m[i[2]][j[2]] 
        + m->m[i[0]][j[1]]*m->m[i[1]][j[2]]*m->m[i[2]][j[0]] 
        + m->m[i[1]][j[0]]*m->m[i[2]][j[1]]*m->m[i[0]][j[2]] 
        - m->m[i[0]][j[2]]*m->m[i[1]][j[1]]*m->m[i[2]][j[0]]
        - m->m[i[1]][j[0]]*m->m[i[0]][j[1]]*m->m[i[2]][j[2]]
        - m->m[i[0]][j[0]]*m->m[i[2]][j[1]]*m->m[i[1]][j[2]]
        ;
}

double CalcDeterminant4( ESMatrix *m) {
    double first = m->m[0][0] * 
            CalcDeterminant3(m,0,0);
    double second = m->m[0][1] *
            CalcDeterminant3(m,0,1);
    double third = m->m[0][2] *
            CalcDeterminant3(m,0,2);
    double four = m->m[0][3] *
            CalcDeterminant3(m,0,3);
    printf("%f %f %f %f\n", first, second, third, four);
    return first + second + third + four;
}

void esVectorLoad(ESVector * result, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
    memset(result, 0x0, sizeof(ESVector));
    result->v[0] = x;
    result->v[1] = y;
    result->v[2] = z;
    result->v[3] = w;
}
