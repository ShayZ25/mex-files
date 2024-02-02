clear

%% Configuration
cFileName = 'matrixMultiplication.c';
mexExtension = 'mexw64';
apiVersion = 'R2018a';
forceBuild = true;

A = randn(100, 400);
B = randn(400, 200);

%% Build MEX

[~,fileNameWithoutExtension] = fileparts(cFileName);
mexFileName = [fileNameWithoutExtension, '.', mexExtension];

if ~isfile(mexFileName) || forceBuild
    mex(cFileName, ['-', apiVersion]);
end

%% Test MEX
mexResult = matrixMultiplication(A, B);
matlabResult = A * B;

numericError = max(abs(mexResult - matlabResult),[],'all');
fprintf("The numeric error is %.2f e-6.\n", numericError*1e6);

%% Test Runtime

mexFunctionHandle = @() matrixMultiplication(A, B);
fprintf('MEX runtime is %.3f micro sec.\n', timeit(mexFunctionHandle)*1e6);

matlabFunctionHandle = @() A*B;
fprintf('MATLAB runtime is %.3f micro sec.\n', timeit(matlabFunctionHandle)*1e6);
