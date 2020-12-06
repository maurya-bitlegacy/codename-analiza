#include<stdio.h>


int g(int u);

int f(int a, int b) {
  int c;
  if(a == 0) {
    c = a*b;
  } else {
    c = g(10);
  }
  return c;
}

int g(int u) {
  int v = f(-u, u);
  return v;
}


int main() {
	int p = 5;
	int q = f(p, -3);
	int r = g(-q);
	return 0;
}
