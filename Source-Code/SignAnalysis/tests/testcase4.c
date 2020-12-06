#include<stdio.h>

int f1(int);
int f2(int);

int f2(int b){

	int a = -2;
	
	if(a==b ){

		if(b ==3){
				
			a = 1;
		}

		b = 2;

	}else{

		a = 1;
	}
	return a;

}

int f1(int b){

	int recur = b;
	
	recur++;
	recur = f2(recur);
	recur++;
	return recur;
}

int main()
{

	int a = 1;
	int b = -2;
	int c = -3;

	if(a == 1)
	{
			b = 300;
			b = f1(b);
	
			c = 100;
		}
		else {

			b = 300;
			b =f2(b);
			c = -200;
		}

	
	return 0;

}

