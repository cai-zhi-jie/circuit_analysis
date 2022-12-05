function [source] = GenerateSrc(SRC, t)
% Generate Source Signal from the stamp result
% SRC-stamp from netlist
% t-sample time
source = zeros(length(SRC), 1);

for i = 1 : length(SRC)
    
    type = SRC{i}{1};
    argc = size(SRC{i},1);
    argv = zeros(7,1);
    if (strcmp(type, 'sin')) % SIN TYPE
        % arg list: voffset,vamp,freq,tdelay,decay,phasedelay
        % arg list: 1,      2,   3,   4,     5,    6,
        for j = 2:argc
            argv(j-1) = SRC{i}{j};
        end
        source(i) = exp(-argv(5)*t)*argv(2)*sin(2*pi*argv(3)*(t-argv(4))-argv(6))+argv(1);

    elseif (strcmp(type, 'pulse')) % PULSE TYPE
        % arg list: lv,hv,tdelay,trise,tfall,tpulsewidth,tperiod
        % arg list: 1, 2, 3,     4,    5,    6,          7,
        for j = 2:argc
            argv(j-1) = SRC{i}{j};
        end
        % (t - tdelay) % tperiod
        if (t < argv(3))
            source(i) = argv(1);
        else
            tlocal = rem(t-argv(3),argv(7));
            if tlocal <= argv(4)
                source(i) = (argv(2) - argv(1)) / argv(4) * tlocal + argv(1);
            elseif tlocal >= argv(4) && tlocal <= argv(4) + argv(6)
                source(i) = argv(2);
            elseif tlocal >= argv(4) + argv(6) && tlocal <= argv(4) + argv(5) + argv(6)
                source(i) = argv(2) - (argv(2) - argv(1)) / argv(5) * (tlocal - argv(4) - argv(6));
            else
                source(i) = argv(1);
            end
        end
    else
        error('UNKNOWN SOURCE TYPE');
    end
end


