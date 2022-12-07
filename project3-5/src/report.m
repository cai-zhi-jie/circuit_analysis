function  report(filename)
% report result by statistic and figure
load(filename,'-mat');
% output some useful info 
fprintf('Number of Steps: %d\n',stepNum);
fprintf('Runtime: %.4f\n', runtime);
fprintf('Max Absolute Error: %.4f\n',maxError)
fprintf('Mean Square Error: %.4f\n',MSE);

subplot(2, 2, 1);
plot(time, source);
title('source signal');
xlabel('T/s');
ylabel('V/V');

subplot(2, 2, 2);
plot(time, output(:,1));
title(strcat(methodName,' output'));
xlabel('T/s');
ylabel('V/V');

subplot(2, 2, 3);
plot(time, output(:,1), 'b-');
hold on;
plot(hspiceTime, hspiceOutput(:,1), 'r');
hold off;
title(strcat(methodName,' vs SPICE'));
xlabel('T/s');
ylabel('V/V');
legend(methodName, 'SPICE');

subplot(2, 2, 4);
plot(hspiceTime, absError(:,1));
title(strcat(methodName,' Absolute Error Distribution'));
xlabel('T/s');
ylabel('V/V');

end

