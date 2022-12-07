function [result] = GCR(M, b, x, maxIter, errorThres)
% Generalized Conjugate Residual
% M,b,x-Mx = b
% x0-the initial x
% errorThres-the boundary of error
% maxIter-the maximum GCR iteration 

[m,n] = size(M);

if (m ~= n)
    disp ('matrix M is not squart');
    result = x;
    return
end

% if (norm(b,2) == 0)
%     result = zeros(size(x));
%     return
% end

maxIter = min(maxIter,m);

r = b - M * x;
iter = 0;

if M' == M % symmetirc case
    disp('symmetric matrix');
    p = r;
    for j = 1 : m
        Mpj = M * p;
        % stop criteria
        if (iter >= maxIter || norm(Mpj,2) == 0 )
            break;
        end
        a = (r' * Mpj) / (Mpj' * Mpj);
        x = x + a * p;
        if (norm(a*p,2) < errorThres)
            break;
        end
        r = r - a * Mpj;
        beta = ((M * r)' * Mpj) / (Mpj' * Mpj);
        p = r - beta * p;
        iter = iter + 1;
    end
    fprintf('(%d/%d)\n',j,m);
else % non-symmetric case   
    Mp = zeros(m, maxIter);
    p = zeros(m, maxIter);
    for j = 1:m
        p(:,j) = r;
        Mp(:,j) = M * p(:,j);  
        for i = 1:j-1
            beta = Mp(:,j)' * Mp(:,i);
            p(:,j) = p(:,j) - beta * p(:,i);
            Mp(:,j) = Mp(:,j) - beta * Mp(:,i);
        end
        normMp = norm(Mp(:,j),2);
        if (normMp ~= 0)
            p(:,j) = p(:,j) / normMp;
            Mp(:,j) = Mp(:,j) / normMp;
        end
        a = (r' * Mp(:,j));
        x = x + a * p(:,j);
        r = r - a * Mp(:,j);                  
        iter = iter + 1;
        if (iter >= maxIter || norm(a * p(:,j),2) < errorThres)
            break;
        end
    end
    fprintf('(%d/%d)\n',j,m);
end
% return value 
result = x;



