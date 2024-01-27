clear

%% Configuration
cFileName = 'matrixMultiplication.c';
mexExtension = 'mexw64';
apiVersion = 'R2018a';
forceBuild = true;

A = randn(8,5);
B = randn(5,6);

%% Build MEX

[~,fileNameWithoutExtension] = fileparts(cFileName);
mexFileName = [fileNameWithoutExtension, '.', mexExtension];

if ~isfile(mexFileName) || forceBuild
    mex(cFileName, ['-', apiVersion]);
end

%% Test MEX
C = matrixMultiplication(A, B);
mexResult = matrixMultiplication(A, B);
matlabResult = A * B;

numericError = max(abs(mexResult - matlabResult),[],'all');
assert(numericError == 0, 'There is a numeric error!');

%% Test Runtime

mexFunctionHandle = @() matrixMultiplication(A, B);
fprintf('MEX runtime is %.3f micro sec.\n', timeit(mexFunctionHandle)*1e6);

matlabFunctionHandle = @() A*B;
fprintf('MATLAB runtime is %.3f micro sec.\n', timeit(matlabFunctionHandle)*1e6);
