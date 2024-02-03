/*
 * matrixMultiplication.c
 *
 * Multiplies an input matrix A (i x j) with input matrix B (j x k).
 *
 * C (i x k) is the output matrix.
 *
 * The calling syntax is:
 *		C = matrixMultiplication(A, B)
 *
 * This is a MEX file for MATLAB.
*/

#include "mex.h"
#include "matrix.h"
#include "omp.h"
#include "windows.h"

/* nlhs - Number of output (left-side) arguments, 
 * or the size of the plhs array.
 * plhs - Array of output arguments.
 * nrhs - Number of input (right-side) arguments,
 * or the size of the prhs array.
 * prhs - Array of input arguments.
 */

double *transpose(double *A, size_t i, size_t j);
void mexFunction(int nlhs, mxArray *plhs[],
                 int nrhs, const mxArray *prhs[])
{
    double *A;
    double *B;
    double *C;
    double *transposedA;
    int numProcessors;
    
    size_t i; // Number of rows in A
    size_t j; // Number of columns in A
    size_t l; // Number of rows in B
    size_t k; // Number of columns in B
    
    if(nrhs != 2) {
        mexErrMsgIdAndTxt("MyToolbox:matrixMultiplication:nrhs",
                "Two inputs required.");
    }
    
    A = mxGetDoubles(prhs[0]);
    B = mxGetDoubles(prhs[1]);

    mwSize nDimA = mxGetNumberOfDimensions(prhs[0]);
    mwSize nDimB = mxGetNumberOfDimensions(prhs[1]);

    if(nDimA != 2) {
        mexErrMsgIdAndTxt("MyToolbox:matrixMultiplication:nDimA",
                "Matrix A must have two dimensions.");
    }
    
    if(nDimB != 2) {
        mexErrMsgIdAndTxt("MyToolbox:matrixMultiplication:nDimB",
                "Matrix B must have two dimensions.");
    }
    
    i = mxGetM(prhs[0]);
    j = mxGetN(prhs[0]);
    l = mxGetM(prhs[1]);
    k = mxGetN(prhs[1]);
    
    if(j != l) {
        mexErrMsgIdAndTxt("MyToolbox:matrixMultiplication:dimensionsMismatch",
                "Number of columns of A must be equal to the number of rows of B.");
    }

    plhs[0] = mxCreateDoubleMatrix(i,k,mxREAL);
    C = mxGetDoubles(plhs[0]);
    
    transposedA = transpose(A, i, j); // Change order of data in memory to reduce jumps between memory locations
    matrixMultiplication(transposedA, B, C, i, j, k);
}

double *transpose(double *A, size_t i, size_t j)
{
    int n;
    int m;
    double *transposedA;
    size_t nElements;
    
    nElements = i*j;
    transposedA = malloc(nElements*sizeof(double));

    #pragma omp parallel for
    for (n=0; n<nElements; n++){
//         linearIndexInput = n;
//         rowInput = n%i;
//         columnInput = n/i;
//         rowOutput = n/i;
//         columnOutput = n%i;
//         nRowsOutput = j;
//         linearIndexOutput = rowOutput + columnOutput*nRowsOutput;
        *(transposedA+(n%i)*j + n/i) = *(A+n);
    }
    return transposedA;
}

void matrixMultiplication(double *transposedA, double *B, double *C, int i, int j, int k)
{
    int n;
    int m;
    int l;
    double *additionPointerA;
    double *additionPointerB;
    double *additionPointerC;
    double result;
    double *currentPointerA;
    double *currentPointerB;
    double *currentPointerC;

    #pragma omp parallel for
    for (n=0; n<i; n++) {
        additionPointerA = transposedA + n*j;
        additionPointerC = C + n;
        for (m=0; m<k; m++){
            result = 0;
            currentPointerC = additionPointerC + m*i;
            additionPointerB = B + m*j;
            for (l=0; l<j; l++){
                currentPointerA = additionPointerA + l;
                currentPointerB = additionPointerB + l;
                result += *currentPointerA * *currentPointerB;
            }
            *currentPointerC = result;
        }
    }
}
