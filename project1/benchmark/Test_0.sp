* Test Example 0

Vin 1  	0 		5
X1	1	4	3	sub
X2	2	4	5	sub
Cs	3	5	10
Ls1	1	2	1
Rs	0	2	1

.SUBCKT sub  1  2  3
Ls1		1		0		3
Ls2		3		2		2
K2		Ls1		Ls2		0.5
Rs		1		3		5
.ENDS sub

.PROBE  V(5)
.end
