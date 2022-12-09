function [Cr,Gr,Br,LTr,runtime] = prima(C, G, B, LT, order, s0 )
% prima

runtime = cputime;
R = (G + s0 .* C) \ B;
A = -(G + s0 .* C) \ C;
numOut = size(B, 2);
n = ceil(order / numOut); 

%calculate X
[X(:, 1 : numOut), ~] = qr(R, 0);
for k = 1 : n
    Xk = A * X(:, (k - 1) * numOut + 1 : k * numOut);
    for j = 1 : k
       H = (X(:, (k - j) * numOut + 1 : (k-j+1) * numOut))'* Xk;
       Xk = Xk - (X(:, (k - j) * numOut + 1 : (k-j+1) * numOut)) * H;
    end
	[X(:, k * numOut + 1 : (k+1) * numOut), ~] = qr(Xk, 0);
end
Xr = X(:,1:order);

%���㽵�׺�ľ���
Cr = Xr' * (-1 .* A) * Xr;
Gr = Xr' * (eye(size(A, 1)) + s0 .* A) * Xr;
Br = Xr' * R;
LTr = LT * Xr;


%���׹��̺ķ�ʱ��
runtime = cputime - runtime;
return
end

