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
	int vals[ITEMS] = {60, 70, 110, 50, 90, 120, 100};
	char keys[ITEMS][VALIDX+1] = {"apple", "orange", "kiwi", "banana", "lime", "melon", "pear"};
	int hashtable[HASHSIZ];
	int i, j, hash;

	int fd;
	int rowlen = VALIDX+1;
	char db[ITEMS*(rowlen)];

	printf("%-8s %3s %4s\n", "item", "$/kg", "hash");
	for (i = 0; i < ITEMS; i++) {
		hash = 0;
		for (j = 0; keys[i][j] != '\0'; j++) {
			hash += keys[i][j];
		}
		keys[i][VALIDX] = vals[i];
		printf("%-8s %3d\n", keys[i], keys[i][VALIDX]);

		/* Don't store the pointer but the hash index */
		hashtable[hash % HASHSIZ] = i;
	}

	for (i = 0; i < ITEMS; i++) {
		memcpy(&db[i*rowlen], keys[i], rowlen);
	}
	/* dump the data table to the file */
	fd = open("db", O_WRONLY|O_CREAT, 0644);
	write(fd, db, sizeof(db));
	/* also print to the standard output */
	write(1, db, sizeof(db));
	close(fd);

	/* dump the hash table to the file */
	fd = open("hashtable", O_WRONLY|O_CREAT, 0644);
	write(fd, hashtable, sizeof(hashtable));
	close(fd);
	return 0;
}
