#include <stdio.h>

void f1(int a, int b, int c) {
  b = a * c / 2000;
  printf("%d, %d, %d\n", a, b, c);
}


void f2(int i, int j, int k) {
  int l = 2 * k;
  int m;
  if (j == 100) {
    m = -10 * j;
  } else {
    m = i;
  }
  f1(l, m, k);
  int o = m * 2;
  int q = -2;
  f1(o, q, k);
  printf("%d, %d, %d, %d\n", q, m, o, l);
}


int main() {
  f2(10, -100, 1000);
  return 0;
}

