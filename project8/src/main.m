function [] = main(order, f0, fs, fe, sampleNum)
% model order reduction by PRIMA

load PEEC.mat;

%% Solve/Load the frequency response of the transmission function H(s) of the original system
% % Solve the frequency response of H(s) before the reduction, which can be calculated once and then read
% C = full(C); 
% G = full(G);  
% [freq, originAmp, originRuntime] = freq_respond(fs, fe, sampleNum, C, G, B, LT);
% %Save original result
% save('origin_system.mat', 'freq', 'originAmp', 'originRuntime','-mat');

% Load original results
load origin_system.mat;

%% Model reduction
[Cr, Gr, Br, LTr, primaTime] = prima(C, G, B, LT, order, 1i*2*pi*f0);
%save the C, G, B, LT after reduction
save('reduced_result.mat', 'Cr', 'Gr', 'Br', 'LTr','-mat');
% load result
load reduced_result.mat;

%% Solve/Load the frequency response of the transmission function H(s) of the order reduced system
%Calculate the H(s) after the reduction
[freq, reducedAmp, reducedTime] = freq_respond(fs, fe, sampleNum, Cr, Gr, Br, LTr);

%% Error calculation
%Distribution of absolute error
error = abs(originAmp - reducedAmp);
%Maximum absolute error
maxError = max(error, [], 2);   
%Mean square error
MSE = sqrt(sum(error.^2, 2) ./ (sampleNum + 1));

fprintf('PRIMA降阶过程的时间： %.4fs\n', primaTime);
fprintf('求解原始的H(s)频率响应的时间： %.4fs\n', originRuntime);
fprintf('求解降阶后H(s)频率响应的时间： %.4fs\n', reducedTime);
fprintf('频响最大绝对误差： %.4f\n', maxError);   
fprintf('频响均方差： %.4f\n', MSE);

%Before & After reduction H(s) plot
subplot(2, 2, 1);
plot(log10(freq), 20 * log10(originAmp(1, :)), 'b--', log10(freq), 20 * log10(reducedAmp(1, :)), 'r--');
title('降阶前后|H(s)|对比图');
xlabel('log10(f)'); ylabel('20*log10|H(s)|/dB');
legend('原始|H(s)|','降阶后|H(s)|');
grid on;

%Before & After reduction H(s) plot
subplot(2, 2, 2);
plot(freq, 20 * log10(originAmp(1, :)), 'b--', freq, 20 * log10(reducedAmp(1, :)), 'r--');
title('降阶前后|H(s)|对比图');
xlabel('f/Hz'); ylabel('20*log10|H(s)|/dB');
legend('原始|H(s)|','降阶后|H(s)|');
grid on;

%Distribution of absolute error
subplot(2, 2, 3);
plot(log10(freq),20 * log10(error(1, :)), 'g');
title('降阶前后绝对误差分布');
xlabel('log10(f)');ylabel('20*log10| \Delta H(s)|/dB');
grid on;

%Distribution of absolute error
subplot(2, 2, 4);
plot(freq,20 * log10(error(1, :)), 'g');
title('降阶前后绝对误差分布');
xlabel('f/Hz');ylabel('20*log10| \Delta H(s)|/dB');
grid on;
end

