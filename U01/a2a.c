#include <stdio.h>

int main ()
{
	float f;
	int i;
	long l;

	i = 1024;
	l = (long) i;
	printf("%d = %ld \n",i,l); // 1024 = 1024
	// longs und ints sehen gleich aus, typkonvertierung macht Sinn.

	l = 23;
	printf("l = %ld\n",l); // l = 23
	// Werte können überschrieben werden.

	i = (int) l;
	printf("%ld = %d\n",l,i); // 23 = 23
	// Umgekehrt können longs auch in ints umgewandelt werden.

	l = 17179869184;
	printf("l = %ld\n",l); // l = 17179869184
	// longs können sehr groß sein.

	i = (int) l;
	printf("%ld = %d\n",l,i); // 17179869184 = 0
	// Wenn ein zu langer long in einen int gecastet wird, kriegt
	// dieser den Wert 0.

	f = 5/2;
	printf("5/2 = %6.2f\n",f); // 5/2 =   2.00
	// Wenn Zahlen nicht explizit als floats eingegeben werden,
	// dann wird int-Division verwendet.

	f = (float)5/2;
	printf("5/2 = %6.2f\n",f); // 5/2 =   2.50
	// Selbst wenn Zahlen nach der Division in einen float gecastet
	// werden, dann ist das Ergebnis eine float-Division.

	f = 1.333;
	i = (int)f;
	printf("1.333 = %d\n",i); // 1.333 = 1
	// Wenn floats in ints gecastet werden, geht der Nachkommaanteil
	// verloren.

	f = 5.0/2;
	i = (int)f;
	printf("2.5 = %d\n",i); // 2.5 = 2
	// Beim int-casting wird nicht gerundet.

	return 0;
}

