clear all
clc

kwh = 125;
cargof = 886,2;
trans = 1,4758;
clc
cont= input('\n khw utilizados ');

v1=(kwh + trans);
v2 =(v1*cont);
v3= (v2 + cargof);
valor= v3 ;
fprintf('\n Costo total $%.2f ',valor);
fprintf('\n');
