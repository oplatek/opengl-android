#include <stdio.h>
#include <iostream>

using std::cout;
using std::endl;

typedef struct {
	float m[4][4];
} ESMatrix;

void fillMatrix(ESMatrix *m);
double CalcDeterminant4(ESMatrix * mat);
double CalcDeterminant3(ESMatrix *m, int skip_i, int skip_j) ;
void MatrixInversion(ESMatrix *A, ESMatrix *Y);
void printMatrix(ESMatrix * m);

int main() {
    printf("Inverting matrix 4*4 by Cramer rule\n");
    ESMatrix m;
    ESMatrix res;
    
    fillMatrix(&m);

    CalcDeterminant4(&m);
    MatrixInversion(&m, &res);

    printMatrix(&m);
    printMatrix(&res);

}

// the result is put in Y
void MatrixInversion(ESMatrix *A, ESMatrix *Inv)
{
    // get the determinant of a
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
//    printf("index skip_i = %d, i = {%d, %d, %d}\n", skip_i, i[0],i[1],i[2]); 
//    printf("index skip_j = %d, j = {%d, %d, %d}\n", skip_j, j[0],j[1],j[2]); 
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


void fillMatrix(ESMatrix *m) {
    m->m[0][0] = 1;
    m->m[0][1] = 1;
    m->m[0][2] = 1;
    m->m[0][3] = 1;

    m->m[1][0] = 1;
    m->m[1][1] = 1;
    m->m[1][2] = 1;
    m->m[1][3] = 0;

    m->m[2][0] = 1;
    m->m[2][1] = 1;
    m->m[2][2] = 0;
    m->m[2][3] = 0;

    m->m[3][0] = 1;
    m->m[3][1] = 0;
    m->m[3][2] = 0;
    m->m[3][3] = 0;
}


void printMatrix(ESMatrix * m) {
    const char delim[] = "===================================================\n";
    printf(delim);
	for (int i = 0; i < 4; ++i) {
		printf("( %f %f %f %f )\n",m->m[i][0],m->m[i][1],m->m[i][2],m->m[i][3]);
	}
    printf(delim);
}
