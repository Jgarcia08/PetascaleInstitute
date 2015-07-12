#include<stdio.h>
#include<math.h>
int main() {
	int i;
	int c;
	int n = 1000;
	int pie = 0;
	int d;
	int e;
	int f;
	int g;
	for (i = 0; i < n; i++)
	{
		c = pow(n,2);
		d = pow(i,2);
		e = c - d;
		f = pow(e,.5);
		g = f/c;
		pie = pie + g;
	}
	printf("%f", pie);

}
