# vg101-s2-lab5-2020fall
An interpreter written by C to interpret simple matlab code.

- calculate algebra expressions with +-*/^ and brackets.
- variable assignment
- fprintf (only allow at most 2 parameters, i.e. fprintf("%f\n",a) is accepted but fprintf("%f%d\n",a,b)) is invalid
- output variable list at the end of output.

**Do not copy these code if you also have this assignment. That will definitely violate um-sjtu ji and Umich's Honor code. I'll not be responsible for the violation.**

----

sample input:

a=32
b=-5+8.21-7^2
c=  a\*b
aa=a\*(b+c)+ 6^3;
fprintf(" aa = %f\n " , aa)
aa

sample output:

32.000000
-45.790000
-1465.280000
 aa = -48138.240000
 -48138.240000
---var list---
a=32.000000
b=-45.790000
c=-1465.280000
aa=-48138.240000