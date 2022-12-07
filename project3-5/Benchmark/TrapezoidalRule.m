function [output, source, time] = TrapezoidalRule(C, G, B, LT, SRC, startTime, endTime, stepNum, f)
% Trapezoidal Rule solver
% C,G,B,LT,SRC-the matrixs from stamp command
% [startTime, endTime]-simulated interval
% stepNum-TR step number
% output-simulated output
% source-input source
% time-sample time vector


%% initial state G * X(0) = B * U(0)
nodeNum = size(G, 1);
outputNum = size(LT, 1);
x0 = ones(nodeNum, 1);
ut  = GenerateSrc(SRC, 0);
utp = ut;
count = 1;
fprintf('%d',count);
count=count+1;
x = f(G, B * ut, x0);
t = 0;
% (2C + tStep*G) * X(t) = (2C - tStep*G) * X(t-1) + tStep*B*(U(t) + U(t-1))
tStep = (endTime - startTime) / stepNum;     
M1 = 2*C + tStep*G;  
M2 = 2*C - tStep*G;
M3 = tStep*B;
%% before startTime
for i = 1:floor(startTime/tStep)
    t = t + tStep;
    ut = GenerateSrc(SRC, t);
    b = M2*x + M3*(ut+utp);
    fprintf('%d',count);
    count=count+1;
    x = f(M1, b, x0);
    utp = ut;
end
if (t ~= startTime) 
    deltaT = startTime - t;
    t = startTime;
    ut = GenerateSrc(SRC, t);
    b = (2*C - deltaT*G)*x + deltaT*B*(ut+utp);
    fprintf('%d',count);
    count=count+1;
    x = f(2*C + deltaT*G, b, x0);
    utp = ut;
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
    b = M2*x + M3*(ut+utp);
    fprintf('%d',count);
    count=count+1;
    x = f(M1, b, x0);
    utp = ut;
    
    output(i+1,:) = (LT * x)';
    source(i+1,:)= ut';
end
