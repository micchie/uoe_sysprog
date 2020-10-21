#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>

int
main()
{
	int fd, len;
	char buf[32];

	fd = open("/dev/hello", O_RDWR);
	if (fd < 0) {
		perror("open");
		return -1;
	}
	len = write(fd, "hello", 6);
	if (len < 0) {
		perror("write");
		close(fd);
		return -1;
	}
	len = read(fd, buf, sizeof(buf));
	if (len < 0) {
		perror("read");
		close(fd);
		return -1;
	}
	write(1, buf, sizeof(buf));
	printf("\n");
	return 0;
}
