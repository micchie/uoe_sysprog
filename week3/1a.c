#include <stdio.h>

int main() {
	int i;
	char c;
	
	i = 1;
	c = 'a';
	
	printf("%p %d %p %c\n", &i, i, &c, c);
	// you will see something like
	// “0x5f94, 1, 0x5f98, a”
	char *cp;
	
	cp = &c;
	printf("%p %p %c\n", &cp, cp, *cp);
	// you will see something like
	// 0x5ff4, 0x5f98, a
	
	printf("%d\n", c);
	// you will see 97
}
