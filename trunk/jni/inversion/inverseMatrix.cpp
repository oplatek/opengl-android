#include <stdio.h>
#include <iostream>

using std::cout;
using std::endl;

typedef struct {
	float m[4][4];
} ESMatrix;

double CalcDeterminant( float **mat, int order);
int GetMinor(float **src, float **dest, int row, int col, int order);
void MatrixInversion(float **A, int order, float **Y);
void allocateMatrix(float **m, int x_size, int y_size);
void releaseMatrix(float **m, int x_size);

// the result is put in Y
void MatrixInversion(float **A, int order, float **Y)
{
    // get the determinant of a
    double det = 1.0/CalcDeterminant(A,order);

    // memory allocation
    float *temp = new float[(order-1)*(order-1)];
    float **minor = new float*[order-1];
    for(int i=0;i<order-1;i++)
        minor[i] = temp+(i*(order-1));

    for(int j=0;j<order;j++)
    {
        for(int i=0;i<order;i++)
        {
            // get the co-factor (matrix) of A(j,i)
            GetMinor(A,minor,j,i,order);
            Y[i][j] = det*CalcDeterminant(minor,order-1);
            if( (i+j)%2 == 1)
                Y[i][j] = -Y[i][j];
        }
    }

    // release memory
    //delete [] minor[0];
    delete [] temp;
    delete [] minor;
}

// calculate the cofactor of element (row,col)
int GetMinor(float **src, float **dest, int row, int col, int order)
{
    // indicate which col and row is being copied to dest
    int colCount=0,rowCount=0;

    for(int i = 0; i < order; i++ )
    {
        if( i != row )
        {
            colCount = 0;
            for(int j = 0; j < order; j++ )
            {
                // when j is not the element
                if( j != col )
                {
                    dest[rowCount][colCount] = src[i][j];
                    colCount++;
                }
            }
            rowCount++;
        }
    }

    return 1;
}

// Calculate the determinant recursively.
double CalcDeterminant( float **mat, int order)
{
    // order must be &gt;= 0
	// stop the recursion when matrix is a single element
    if( order == 1 )
        return mat[0][0];

    // the determinant value
    float det = 0;

    // allocate the cofactor matrix
    float **minor;
    minor = new float*[order-1];
    for(int i=0;i<order-1;i++)
        minor[i] = new float[order-1];

    for(int i = 0; i < order; i++ )
    {
        // get minor of element (0,i)
        GetMinor( mat, minor, 0, i , order);
        // the recusion is here!

        det += (i%2==1?-1.0:1.0) * mat[0][i] * CalcDeterminant(minor,order-1);
        //det += pow( -1.0, i ) * mat[0][i] * CalcDeterminant( minor,order-1 );
    }

    // release memory
    for(int i=0;i<order-1;i++)
        delete [] minor[i];
    delete [] minor;

    return det;
}

void allocateMatrix(float **m, int x_size, int y_size) {
    m = new float*[x_size];
    for(int i = 0 ; i < x_size; ++i) {
        m[i] = new float[y_size];
    }
}

void releaseMatrix(float **m, int x_size) {
    for(int i = 0 ; i < x_size; ++i) {
        delete [] m[i];
    }
    delete [] m;
}

void fillMatrix(float **m, int x_size, int y_size) {
    for( int i = 0; i < x_size; ++i ) {
        for ( int j = 0; j < y_size; ++j ) {
           m[i][j] = 10 * i + j; 
        }
    }
}

void printMatrix(float **m, int x_size, int y_size) {
    char delim[] = "===================================================\n";
    printf(delim);
    for( int i = 0; i < x_size; ++i ) {
        for ( int j = 0; j < y_size; ++j ) {
            printf("%f\t", m[i][j]);
        }
        printf("\n");
    }
    printf(delim);
}
/*
int main() {
    float **MyMatrix;
    MyMatrix = new float*[4];
    for(int i=0;i<4;i++)
    MyMatrix[i] = new float[4];

    float **Y;
    Y = new float*[4];
    for(int i=0;i<4;i++)
    Y[i] = new float[4];

    MyMatrix[0][0] = 1;
    MyMatrix[0][1] = 1;
    MyMatrix[0][2] = 1;
    MyMatrix[0][3] = 1;

    MyMatrix[1][0] = 1;
    MyMatrix[1][1] = 1;
    MyMatrix[1][2] = 1;
    MyMatrix[1][3] = 0;

    MyMatrix[2][0] = 1;
    MyMatrix[2][1] = 1;
    MyMatrix[2][2] = 0;
    MyMatrix[2][3] = 0;

    MyMatrix[3][0] = 1;
    MyMatrix[3][1] = 0;
    MyMatrix[3][2] = 0;
    MyMatrix[3][3] = 0;

    cout<<endl;
    for (int i=0;i<4;i++) {
        for (int j=0;j<4;j++) {
            cout<<MyMatrix[i][j]<<"\t";
        }
        cout<<endl;
    }

    MatrixInversion(MyMatrix, 4, Y);

    for (int i=0;i<4;i++) {
        for (int j=0;j<4;j++){
            cout<<Y[i][j]<<"\t";
        }
        cout<<endl;
    }
}
*/
/*
int main() {
    printf("Inverting matrix by Cramer rule\n");
    printf("See: http://chi3x10.wordpress.com/2008/05/28/calculate-matrix-inversion-in-c/\n");
    float **m;
    float ** res;
    int size = 4;

    allocateMatrix(m, size, size);
    allocateMatrix(res, size, size);
    
    fillMatrix(m, size, size);
    MatrixInversion(m, size, res);

    printMatrix(res, size, size);
    printMatrix(res, size, size);

    releaseMatrix(m,size);
    releaseMatrix(res, size);
}
*/
