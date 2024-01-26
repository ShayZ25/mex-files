/*
 * arrayProduct.c - example in MATLAB External Interfaces
 *
 * Multiplies an input scalar (multiplier) 
 * times a 1xN matrix (inMatrix)
 * and outputs a 1xN matrix (outMatrix)
 *
 * The calling syntax is:
 *
 *		outMatrix = arrayProduct(multiplier, inMatrix)
 *
 * This is a MEX file for MATLAB.
*/

#include "mex.h"
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
    double multiplier;      /* input scalar */
    double *inMatrix;       /* 1xN input matrix */
    mwSize ncols;           /* size of matrix */
    double *outMatrix;      /* output matrix */
    
    /* get the value of the scalar input  */
    multiplier = mxGetScalar(prhs[0]);
    /* create a pointer to the real data in the input matrix  */
    inMatrix = mxGetDoubles(prhs[1]);
    /* get dimensions of the input matrix */
    ncols = mxGetN(prhs[1]);
    /* create the output matrix */
    plhs[0] = mxCreateDoubleMatrix(1,ncols,mxREAL);
    /* get a pointer to the real data in the output matrix */
    outMatrix = mxGetDoubles(plhs[0]);
    
    if(nrhs != 2) {
        mexErrMsgIdAndTxt("MyToolbox:arrayProduct:nrhs",
                          "Two inputs required.");
    }
    if(nlhs != 1) {
        mexErrMsgIdAndTxt("MyToolbox:arrayProduct:nlhs",
                          "One output required.");
    }
    /* make sure the first input argument is scalar */
    if( !mxIsDouble(prhs[0]) || 
         mxIsComplex(prhs[0]) ||
         mxGetNumberOfElements(prhs[0]) != 1 ) {
             mexErrMsgIdAndTxt("MyToolbox:arrayProduct:notScalar",
                               "Input multiplier must be a scalar.");
    }
    
    if( !mxIsDouble(prhs[1]) || 
     mxIsComplex(prhs[1])) {
        mexErrMsgIdAndTxt("MyToolbox:arrayProduct:notDouble",
                          "Input matrix must be type double.");
    }
    
    /* check that number of rows in second input argument is 1 */
    if(mxGetM(prhs[1]) != 1) {
        mexErrMsgIdAndTxt("MyToolbox:arrayProduct:notRowVector",
                          "Input must be a row vector.");
    }
    
    /* call the computational routine */
    arrayProduct(multiplier,inMatrix,outMatrix,ncols);
}

/* mwSize can be replaced by int */
void arrayProduct(double x, double *y, double *z, mwSize n)
{
  mwSize i;
  
  for (i=0; i<n; i++) {
    z[i] = x * y[i];
  }
}
