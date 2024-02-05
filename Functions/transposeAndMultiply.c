/*
 * transposeAndMultiply.c
 *
 * Transose an input matrix A (i x j) and multiply it with A.
 *
 * B (j x j) is the output matrix.
 *
 * The calling syntax is:
 *		B = transposeAndMultiply(A)
 *
 * This is a MEX file for MATLAB.
*/

#include "mex.h"
#include "matrix.h"
#include "omp.h"

/* nlhs - Number of output (left-side) arguments, 
 * or the size of the plhs array.
 * plhs - Array of output arguments.
 * nrhs - Number of input (right-side) arguments,
 * or the size of the prhs array.
 * prhs - Array of input arguments.
 */

void mexFunction(int nlhs, mxArray *plhs[],
                 int nrhs, const mxArray *prhs[])
{
    double *A;
    double *B;
    
    size_t i; // Number of rows in A
    size_t j; // Number of columns in A
    
    if(nrhs != 1) {
        mexErrMsgIdAndTxt("MyToolbox:matrixMultiplication:nrhs",
                "One input required.");
    }
    
    A = mxGetPr(prhs[0]);

    mwSize nDimA = mxGetNumberOfDimensions(prhs[0]);

    if(nDimA != 2) {
        mexErrMsgIdAndTxt("MyToolbox:matrixMultiplication:nDimA",
                "Matrix A must have two dimensions.");
    }
    
    i = mxGetM(prhs[0]);
    j = mxGetN(prhs[0]);

    plhs[0] = mxCreateDoubleMatrix(j,j,mxREAL);
    B = mxGetPr(plhs[0]);
    
    transposeAndMultiply(A, B, i, j);
}

void transposeAndMultiply(double *A, double *B, size_t i, size_t j)
{
    int n; // row index of B
    int m; // column index of B
    int l;
    double result;
    
    double *additionPointerTransposedA;
    double *additionPointerLowerB;
    double *additionPointerUpperB;
    double *pointerLowerB;
    double *pointerUpperB;
    double *pointerTransposedA;
    double *additionPointerA;
    double *pointerA;
            
    //Calculate only the lower triangular matrix and assign it to the upper one
//     #pragma omp parallel for
    for (n=0; n<j; n++){
        additionPointerTransposedA = A + n*i;
        additionPointerLowerB = B + n;
        additionPointerUpperB = B + n*j;
        for (m=0; m<=n; m++) {
            additionPointerA = A + m*i;
            pointerLowerB = additionPointerLowerB + m*j;
            pointerUpperB = additionPointerUpperB + m;
            result = 0;
            for (l=0; l<i; l++){
                pointerTransposedA = additionPointerTransposedA + l;
                pointerA = additionPointerA + l;
                result += *pointerTransposedA * *pointerA;
            }
            *pointerLowerB = result;
            *pointerUpperB = result;
        }
    }
}
