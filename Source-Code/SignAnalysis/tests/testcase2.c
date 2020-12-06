#include<stdio.h>

int foo(int a, int b);
int bar(int x);

int main(){
int a= 4, b=-6;
int c= foo(a,-19);
int d= bar(c);
return 0;
}

int foo(int a, int b){
int y,x;
y=bar(-a);
int z= bar(b);
if(y>0){
x= y*z;
}
else{
x= z;
}
return x;
}

int bar(int x){
int m=-6,n;
n=foo(m,-x);
return n;
}
