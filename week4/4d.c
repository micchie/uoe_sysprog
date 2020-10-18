#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ITEMS	7
#define VALIDX 9
#define HASHSIZ	128

int
main(int argc, char **argv)
{
	char keys[ITEMS][VALIDX+1];
	char *test = argv[1];
	int hashtable[HASHSIZ];
	int i, hash, rowlen = VALIDX+1;

	int fd;
	char buf[70];

	fd = open("db", O_RDONLY);
	read(fd, buf, sizeof(buf));
	close(fd);
	printf("%-8s %3s\n", "item", "$/kg");
	for (i = 0; i < ITEMS; i++) {
		memcpy(keys[i], &buf[i*rowlen], rowlen);
		printf("%-8s %3d\n", keys[i], keys[i][VALIDX]);
	}

	fd = open("hashtable", O_RDONLY);
	read(fd, hashtable, sizeof(hashtable));
	close(fd);

	// Now we want to find the price of something
	hash = 0;
	for (i = 0; test[i] != '\0'; i++) {
		hash += test[i];
	}
	printf("%s %d\n", test, keys[hashtable[hash % HASHSIZ]][VALIDX]);

	return 0;
}
