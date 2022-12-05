* Example RLC-3

Vin N_in1  	0 PULSE 0 3 1m 1m  1m	8m	20m
*Vin N_in1  	0 sin(0 3 0.2k)

*Vin N_in1  	0 10


X1	n_in1	n_in2	seg
X2	N_in2	N_in3	seg
X3	n_in3	n_out	seg
Cl	n_out	0		10u

.SUBCKT seg n_in	n_out
Rseg	N_IN	N_1		3.5
Lseg	n_1		N_OUT	1.2m
Cseg	N_OUT	0		7.3u
.ENDS seg

.TRAN	0.004m 20m
.PROBE  V(n_out)
.print V(n_out)
.end
