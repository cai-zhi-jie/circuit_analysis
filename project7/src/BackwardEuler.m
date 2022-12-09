function [output, xt, source, time] = BackwardEuler(C, G, B, LT, SRC, xt0, startTime, endTime, stepNum,f)
% Backward Euler solver
% C,G,B,LT,SRC-the matrixs from stamp command
% [startTime, endTime]-simulated interval
% errorThres-the error boundary for GCR solution
% xt0-init x value
% output-simulated output
% source-input source
% f-Mx=b solver

%% initial state by xt0
nodeNum = size(G, 1);
outputNum = size(LT, 1);
x0 = ones(nodeNum, 1);
ut = GenerateSrc(SRC, 0);
count = 1;
count=count+1;
x = xt0;
t = 0;
% (C + tStep*G) * X(t) = C * X(t-1) + tStep*B*U(t)
tStep = (endTime - startTime) / stepNum;     
M = C + tStep*G;
%% before startTime
for i = 1:floor(startTime/tStep)
    t = t + tStep;
    ut = GenerateSrc(SRC, t);
    b = C*x + tStep*B*ut;
    count=count+1;
    x = f(M, b, x0);
end
if (t ~= startTime) 
    deltaT = startTime - t;
    t = startTime;
    ut = GenerateSrc(SRC, t);
    b = C*x + deltaT*B*ut;
    count=count+1;
    x = f(M, b, x0);
end
%% after startTime, start recording
output = zeros(stepNum + 1, outputNum);
source = zeros(stepNum + 1, length(SRC));
time = linspace(startTime, endTime, stepNum + 1)';
xt = x;
output(1,:) = (LT * x)';
source(1,:) = ut';
for i = 1 : stepNum
    t = t + tStep;
    ut = GenerateSrc(SRC, t);
    b = C * x + tStep * B * ut;
    count=count+1;
    x = f(M, b, x0);
    xt = x;
    output(i+1,:) = (LT * x)';
    source(i+1,:)= ut';
end



