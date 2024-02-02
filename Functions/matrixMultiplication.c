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

/* The gateway function */
void mexFunction(int nlhs, mxArray *plhs[],
                 int nrhs, const mxArray *prhs[])
{
    double *A;
    double *B;
    double *C;
    
    int i; // Number of rows in A
    int j; // Number of columns in A
    int l; // Number of rows in B
    int k; // Number of columns in B
    
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
   
    matrixMultiplication(A, B, C, i, j, k);
}

void matrixMultiplication(double *A, double *B, double *C, int i, int j, int k)
{
    int n;
    int m;
    int l;
    double result;
    double *currentPointerA;
    double *currentPointerB;
    double *currentPointerC;

//     SYSTEM_INFO sysInfo;
//     GetSystemInfo(&sysInfo);
//     int numProcessors = sysInfo.dwNumberOfProcessors;
//     Final matrix with size of i x k
//     #pragma omp parallel for // num_threads(numProcessors)
    
    for (n=0; n<i; n++) {
        for (m=0; m<k; m++){
            result = 0;
            currentPointerC = C + (n + m*i);
            for (l=0; l<j; l++){
                currentPointerA = A + (n + l*i);
                currentPointerB = B + (l + m*j);
                result += *currentPointerA * *currentPointerB;
            }
            *currentPointerC = result;
        }
    }
}
