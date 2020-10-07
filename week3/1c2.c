#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ITEMS	7
#define VALIDX 9

int
scmp(char *a, char *b)
{
	int i;
	for (i = 0; a[i] != '\0' && b[i] != '\0'; i++) {
		if (a[i] != b[i])
			return -1;
	}
	if (a[i] != '\0' || b[i] != '\0')
		return -1;
	return 0;
}

int
main(int argc, char **argv)
{
	int vals[ITEMS] = {60, 70, 110, 50, 90, 120, 100};
	char keys[ITEMS][VALIDX+1] = {"apple", "orange", "kiwi", "banana", "lime", "melon", "pear"};
	char *test = argv[1];
	int i, j;

	printf("%-8s %3s\n", "item", "$/kg");
	for (i = 0; i < ITEMS; i++) {
		keys[i][VALIDX] = vals[i];
		printf("%-8s %3d\n", keys[i], keys[i][9]);
	}

	// Now we want to find the price of something
	for (i = 0; i < ITEMS; i++) {
		if (scmp(keys[i], test) == 0) {
			break;
		}
	}
	printf("%s is $%d/kg\n", test, keys[i][VALIDX]);

	return 0;
}
