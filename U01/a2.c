#include <stdio.h>

int main ()
{
	float f;
	int i;
	long l;

	i = 1024;
	l = (long) i;
	printf("%d = %ld \n",i,l);// 1024 = 1024
	//

	l = 23;
	printf("l = %ld\n",l);// l = 23

	i = (int) l;
	printf("%ld = %d\n",l,i);// 23 = 23

	l = 17179869184;
	printf("l = %ld\n",l);// l = 17179869184

	i = (int) l;
	printf("%ld = %d\n",l,i);// 17179869184 = 0

	f = 5/2;
	printf("5/2 = %6.2f\n",f);// 5/2 =   2.00

	f = (float)5/2;
	printf("5/2 = %6.2f\n",f);// 5/2 =   2.50

	f = 1.333;
	i = (int)f;
	printf("1.333 = %d\n",i);// 1.333 = 1

	f = 5.0/2;
	i = (int)f;
	printf("2.5 = %d\n",i);// 2.5 = 2

	return 0;
}

