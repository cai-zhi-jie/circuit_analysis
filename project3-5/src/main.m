function [] = main(caseName, methodName, algorithmName, startTime, endTime, stepNum, errorThres)
% PJ-V : Solving MNA Equation.
% caseName
% methodName
% startTime
% endTime
% stepNum
% errorThres

%% read file
% read hsipce netlist and stamp it
hspiceFile = strcat(caseName,'.sp');
stamp(hspiceFile);

% read hspice simulation result
hspiceData = read_data(strcat(caseName,'.lis'));

%% solving MNA
switch algorithmName
    case 'GCR'% 
        f = @(M, b, x)GCR(M, b, x, 1000, errorThres);
    case 'GCR1'% 
        f = @(M, b, x)gcr1(M, b, x, errorThres, 1000);
    case 'SOR'% 
        f = @(M, b, x)SOR(M, b, x, 1000, errorThres, 0.6);
    otherwise 
        f = @(M, b, x)Golden(M, b, x)
end


tic
switch methodName
    case 'BE'% Backward Euler
        [output, source, time] = BackwardEuler(C, G, B, LT, SRC, startTime, endTime, stepNum, f);
    case 'TR'% Trapezoidal Rule
        [output, source, time] = TrapezoidalRule(C, G, B, LT, SRC, startTime, endTime, stepNum, f);
    otherwise 
        error('ALGORITHM CHOOSE ERROR');
end
runtime = toc;

%% evaluate error
% caculate the error between my Solver and SPICE
hspiceTime = hspiceData(:,1);
hspiceOutput = hspiceData(:,2:end);
% linear interpolation according to hspice time step
matchOutput = interp1(time, output, hspiceTime,'linear','extrap');
% absolute error
absError = abs(matchOutput - hspiceOutput);
% max absolute error
maxError = max(absError(:));
% mean square error
MSE = sqrt(sum(absError.^2) / size(absError(:),1));

%% save and report result
% save result
result= strcat(caseName,'.mat');
save(result,'methodName','stepNum','runtime','source','output','time','hspiceTime','hspiceOutput','absError','maxError','MSE','-mat'); 
% report result
report(strcat(caseName,'.mat'))

end

