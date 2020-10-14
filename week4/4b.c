#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int
main(int argc, char **argv)
{
	int fd, fd2, len;
	char buf[1024];

	if (argc < 3) {
		printf("usage: ./a.out src dst\n");
		return 0;
	}

	fd = open(argv[1], O_RDONLY);
	if (fd < 0) {
		perror("open");
		return 0;
	}
	fd2 = open(argv[2], O_WRONLY|O_CREAT, 0644);
	if (fd2 < 0) {
		perror("open");
		return 0;
	}
	len = read(fd, buf, sizeof(buf));
	write(fd2, buf, len);
	return 0;
}
