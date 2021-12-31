#include <stdio.h>

void sex(int &a)
{
	
}

void main()
{
	int a = 10;
	const int* p = &a;
	printf("%d", *p);
	a = 20;
	printf("%d", *p);
}