function [output, source, time] = BackwardEuler(C, G, B, LT, SRC, startTime, endTime, stepNum,f)
% Backward Euler solver
% C,G,B,LT,SRC-the matrixs from stamp command
% [startTime, endTime]-simulated interval
% errorThres-the error boundary for GCR solution
% output-simulated output
% source-input source
% f-Mx=b solver

%% initial state G * X(0) = B * U(0)
nodeNum = size(G, 1);
outputNum = size(LT, 1);
x0 = ones(nodeNum, 1);
ut = GenerateSrc(SRC, 0);
% x = G\(B * ut);
count = 1;
fprintf('%d',count);
count=count+1;
x = f(G, B * ut, x0);
t = 0;
% (C + tStep*G) * X(t) = C * X(t-1) + tStep*B*U(t)
tStep = (endTime - startTime) / stepNum;     
M = C + tStep*G;
%% before startTime
for i = 1:floor(startTime/tStep)
    t = t + tStep;
    ut = GenerateSrc(SRC, t);
    b = C*x + tStep*B*ut;
%     x = M\b;
    fprintf('%d',count);
    count=count+1;
    x = f(M, b, x0);
end
if (t ~= startTime) 
    deltaT = startTime - t;
    t = startTime;
    ut = GenerateSrc(SRC, t);
    b = C*x + deltaT*B*ut;
%     x = M\b;
    fprintf('%d',count);
    count=count+1;
    x = f(M, b, x0);
end
%% after startTime, start recording
output = zeros(stepNum + 1, outputNum);
source = zeros(stepNum + 1, length(SRC));
time = linspace(startTime, endTime, stepNum + 1)';
output(1,:) = (LT * x)';
source(1,:) = ut';
for i = 1 : stepNum
    t = t + tStep;
    ut = GenerateSrc(SRC, t);
    b = C * x + tStep * B * ut;
%     x = M\b;
    fprintf('%d',count);
    count=count+1;
    x = f(M, b, x0);
    
    output(i+1,:) = (LT * x)';
    source(i+1,:)= ut';
end



