%This file calculates some parameters under different q and s0
load origin_system.mat;
load PEEC.mat;

% grid range
order = 10:10:200;
s0 = 1e9:1e9:15e9;

order_num = length(order);
s0_num = length(s0);
% init result matrix
primaTime = zeros(order_num,s0_num);
frTime = zeros(order_num,s0_num);
absError = zeros(order_num,s0_num);
MSE = zeros(order_num,s0_num);

for i = 1 : order_num
    for j = 1 : s0_num
        
        % prima
        [Cr, Gr, Br, LTr, primaTime(i,j)] = prima(C,G,B,LT,order(i),1i*2*pi*s0(j));
        % frequence respond
        [freq, amp, frTime(i,j)] = freq_respond(0, 1.5e10, 20000, Cr, Gr, Br, LTr);
        % absolute error and MSE
        absError(i,j) = max(abs(originAmp-amp),[],2);
        MSE(i,j) = sqrt(sum(abs(originAmp-amp).^2 ,2)/20001);
        fprintf('order = %d, s0 = %d \n', order(i), s0(j));
    end
end

save('compare.mat','primaTime','frTime','absError','MSE');

load('compare.mat');
subplot(2, 2, 1);
mesh(s0,order,primaTime);
xlabel('Expanding Point');
ylabel('Model Order');
zlabel('PRIMA time/s');

subplot(2, 2, 2);
mesh(s0,order,frTime);
xlabel('Expanding Point');
ylabel('Model Order');
zlabel('Frequency calculating time/s');

subplot(2, 2, 3);
mesh(s0,order,absError);
xlabel('Expanding Point');
ylabel('Model Order');
zlabel('max absolute error/V');

subplot(2, 2, 4);
mesh(s0,order,MSE);
xlabel('Expanding Point');
ylabel('Model Order');
zlabel('MSE')

