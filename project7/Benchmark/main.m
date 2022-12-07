function [] = main(caseName, L, newtonMaxIter, newtonErrorThres, errorThres)
% PJ-VII : Steady State Method for Periodic Linear System.

%% read file
% read hsipce netlist and stamp it
hspiceFile = strcat(caseName,'.sp');
stamp(hspiceFile);

% read hspice simulation result
hspiceData = read_data(strcat(caseName,'.lis'));

%% shooting method
% choosing matlab built-in function or gcr to slove linear function
f = @(M, b, x)Golden(M, b, x); % built-in
% f = @(M, b, x)GCR(M, b, x, 1000, errorThres); % gcr

tic
% get period from src
[~ ,T] = GenerateSrc(SRC,0) ;
shootingStep = T / L;
jacobi = (C * (C + shootingStep * G)^(-1))^L - eye(size(G,1));
nodeNum = size(G, 1);
xk = zeros(nodeNum, 1);
% newton iterate to get initial value
for i = 1 : newtonMaxIter
    [~, phik, ~, ~] = BackwardEuler(C, G, B, LT, SRC, xk, 0, T, L, f);
    hk = phik - xk;
    xkk = xk + f(jacobi,-hk,ones(nodeNum,1));
    if (norm(xkk-xk,2) < newtonErrorThres(1) && norm(hk,2) < newtonErrorThres(2))
        break;
    end
    xk = xkk;
end
newtonIter = i;

% copy hspice simulating timestamp and invterval
hspiceTime = hspiceData(:,1);
st = hspiceTime(1); %start time
et = hspiceTime(end); %end time
deltaT = hspiceTime(2)-hspiceTime(1);% 
n = floor((et - st)/deltaT);
stepNum = n;
% backward euler get simulation result by the given initial value
[output, ~, source, time] = BackwardEuler(C, G, B, LT, SRC, xkk, st, et, n, f);

runtime = toc;

%% evaluate error
% caculate the error between my Solver and SPICE
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
save(result,'xkk','L','stepNum','newtonIter','runtime','source','output','time','hspiceTime','hspiceOutput','absError','maxError','MSE','-mat'); 
% report result
report(strcat(caseName,'.mat'))

end

