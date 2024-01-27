clear

%% Configuration
cFileName = 'arrayProduct.c';
mexExtension = 'mexw64';
apiVersion = 'R2018a';
forceBuild = true;

scalarTest = randn(1);
arrayTest = randn(1, 10000);

%% Build MEX

[~,fileNameWithoutExtension] = fileparts(cFileName);
mexFileName = [fileNameWithoutExtension, '.', mexExtension];

if ~isfile(mexFileName) || forceBuild
    mex(cFileName, ['-', apiVersion]);
end

%% Test MEX

mexResult = arrayProduct(scalarTest, arrayTest);
matlabResult = scalarTest * arrayTest;

numericError = max(abs(mexResult - matlabResult));
assert(numericError == 0, 'There is a numeric error!');

%% Test Runtime

mexFunctionHandle = @() arrayProduct(scalarTest, arrayTest);
fprintf('MEX runtime is %.3f micro sec.\n', timeit(mexFunctionHandle)*1e6);

matlabFunctionHandle = @() scalarTest*arrayTest;
fprintf('MATLAB runtime is %.3f micro sec.\n', timeit(matlabFunctionHandle)*1e6);
