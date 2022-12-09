function [freq, amp, runtime] = freq_respond(fs, fe, sampleNum, C, G, B, LT )
% calculate transfer function by the given parameters
% tic
runtime = cputime;     
numOutput = size(LT, 1);
numInput = size(B, 2);
freq = linspace(fs, fe, sampleNum+1);
amp = zeros(numOutput * numInput, sampleNum + 1);

%calculate H(jf)
for i = 1 : 1 : sampleNum    
    tmp = abs(LT * ((G + 2 * pi * 1i * freq(i) * C) \ B));
    amp(:,i) = tmp(:);
end

runtime = cputime - runtime; 
% runtime = toc;  
return
end

