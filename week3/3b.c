#include <stdio.h>

int
main()
{
	char foo[4];
	char *p;

	foo[0] = 'a';
	foo[1] = 'b';
	foo[2] = 'z';
	foo[3] = '\0';

	printf("foo %s\n", foo);
	printf("foo[0] %c foo[1] %c\n", foo[0], foo[1]);
	p = foo;
	printf("p %p foo %p *p %c\n", p, foo, *p);
	p = &foo[1];
	printf("*p %c\n", *p);

	return 0;
}
