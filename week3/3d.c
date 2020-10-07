#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ITEMS	7
#define VALIDX 9
#define HASHSIZ	128

int
main(int argc, char **argv)
{
	int vals[ITEMS] = {60, 70, 110, 50, 90, 120, 100};
	char keys[ITEMS][VALIDX+1] = {"apple", "orange", "kiwi", "banana", "lime", "melon", "pear"};
	char *test = argv[1];
	char *hashtable[HASHSIZ];
	int i, j, hash;

	printf("%-8s %3s %4s\n", "item", "$/kg", "hash");
	for (i = 0; i < ITEMS; i++) {
		hash = 0;
		for (j = 0; keys[i][j] != '\0'; j++) {
			hash += keys[i][j];
		}
		hashtable[hash % HASHSIZ] = keys[i];

		keys[i][VALIDX] = vals[i];
		printf("%-8s %3d %4d\n", keys[i], keys[i][VALIDX], hash);
	}

	// Now we want to find the price of something
	hash = 0;
	printf("%s %d\n", test, hashtable[hash % HASHSIZ][VALIDX]);

	return 0;
}
