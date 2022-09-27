* Example RLC-3

Vin N_in1  	0 		5
X1	n_in1	n_in2	seg
X2	N_in2	N_in3	seg
X3	n_in3	n_out	seg
Cl	n_out	0		2

.SUBCKT seg n_in	n_out
Rseg	N_IN	N_1		3
Lseg	n_1		N_OUT	2
Cseg	N_OUT	0		4
.ENDS seg

.PROBE  V(n_out)
.end
