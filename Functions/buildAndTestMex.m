clear

%% Configuration
cFileName = 'matrixMultiplication.c';
mexExtension = 'mexw64';
forceBuild = true;

A = randn(300, 100);
B = randn(100, 400);

%% Build MEX

[~,fileNameWithoutExtension] = fileparts(cFileName);
mexFileName = [fileNameWithoutExtension, '.', mexExtension];

switch cFileName
    case 'matrixMultiplication.c'
        buildAndTestMatrixMultiplication(mexFileName, forceBuild, A, B);
    case 'transposeAndMultiply.c'
        buildAndTestTransposeAndMultiply(mexFileName, forceBuild, A);
end

function buildAndTestTransposeAndMultiply(mexFileName, forceBuild, A)
    if ~isfile(mexFileName) || forceBuild
%         mex transposeAndMultiply.c -R2018a -V CFLAGS="$CFLAGS -fopenmp" LDFLAGS="$LDFLAGS -fopenmp"
        mex transposeAndMultiply.c -R2018a -V CFLAGS="$CFLAGS -O3" LDFLAGS="$LDFLAGS -O3"
    end

    %% Test MEX
    mexResult = transposeAndMultiply(A);
    matlabResult = A.'*A;

    numericError = max(abs(mexResult - matlabResult),[],'all');
    fprintf("The numeric error is %.2f e-6.\n", numericError*1e6);

    %% Test Runtime

    mexFunctionHandle = @() transposeAndMultiply(A);
    fprintf('MEX runtime is %.3f micro sec.\n', timeit(mexFunctionHandle)*1e6);

    matlabFunctionHandle = @() A.'*A;
    fprintf('MATLAB runtime is %.3f micro sec.\n', timeit(matlabFunctionHandle)*1e6);
end

function buildAndTestMatrixMultiplication(mexFileName, forceBuild, A, B)
    if ~isfile(mexFileName) || forceBuild
        mex matrixMultiplication.c -R2018a -V CFLAGS="$CFLAGS -fopenmp" LDFLAGS="$LDFLAGS -fopenmp"
%         mex matrixMultiplication.c -R2018a
%         mex transposeAndMultiply.c -R2018a -V CFLAGS="$CFLAGS -O3" LDFLAGS="$LDFLAGS -O3"
%         mex matrixMultiplication.c -R2018a -V CFLAGS="$CFLAGS -fopenmp -O3" LDFLAGS="$LDFLAGS -fopenmp -O3"
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
end