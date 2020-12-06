#include<stdio.h>
int f(int x, int y);
int g(int z);

int main() {
	int var1 = 25;
	int var2 = f(var1, -99);
	int var3 = g(-var2);
	return 0;
}


int g(int z) {
  int retval_f = f(-z, z);
  return retval_f;
}

int f(int x, int y) {
  int result;
  if(x == 0) {
    result = x*y;
  } else {
    result = g(10);
  }
  return result;
}

