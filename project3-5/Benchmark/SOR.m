function [result] = SOR(M, b, x, maxIter, errorThres, w )
% Successive Over Relaxation
% M,b,x-Mx = b
% x-the initial x
% maxIter-the maximum GCR iteration 
% errorThres-the boundary of error
% w-weight
[m, n] = size(M);
if (m ~= n)
    disp ('matrix M is not squart');
    result = x;
    return
end
%% preperation
% swap line to make the diagnal element is non-zero
for i = 1:n-1 
    if M(i,i) == 0
        for j = 1:n
            if M (j,i) ~= 0 && M (i,j) ~= 0
                temp = M(i,:);
                M(i,:) = M (j,:);
                M(j,:) = temp;
                temp = 	b(i,:);
                b(i,:) = b(j,:);
                b(j,:) = temp;
                break;
            end
        end
    else
        continue;
    end
end

% linear combination lines to make the pivot non-zero
for i = 1:n
    if M(i,i) == 0
        for j = 1:n
            if M (j,i) ~= 0
                M(i,:) = M(i,:) + M(j,:);
                b(i,:) = b(i,:) + b(j,:);
                break;
            end
        end
    else
        continue;
    end
end

D = diag(diag(M));
L = tril(M,-1);
U = triu(M,1);
G = inv(D+w*L);
N = (1-w)*D-w*U;
%% iteration
for i=1:maxIter
    xh = x;
    x = G*(N*x+w*b);
    if norm(x - xh, 2) <= errorThres
        break;
    end
end
fprintf('(%d/%d)\n',i,maxIter);
result = x;
