#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int
main(int argc, char **argv)
{
	int fd, len;
	char buf[1024];

	fd = open(argv[1], O_RDONLY);
	if (fd < 0) {
		perror("open");
		return 0;
	}
	len = read(fd, buf, sizeof(buf));
	write(1, buf, len);
	return 0;
}
