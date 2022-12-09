function [result] = Golden(M, b, x)
% Generalized Conjugate Residual
% M,b,x-Mx = b
% x0-the initial x
% errorThres-the boundary of error
% maxIter-the maximum GCR iteration 

result = M\b;



