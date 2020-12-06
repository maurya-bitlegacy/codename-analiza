#include<stdio.h>

int fun() {
  return 51;
}

int main() {

int retval_fun = fun();
 int var1 = retval_fun - 10;
 int var2 = -var1;
 int var3 = var1+var2;
 int var4 = -1*fun();
}
