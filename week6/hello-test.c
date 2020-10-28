#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>

#define MMAPSIZ 8192

int
main()
{
	int fd, len;
	char buf[32];
	char *p;

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

	p = mmap(NULL, MMAPSIZ, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if (p == MAP_FAILED) {
		perror("mmap");
	}
	printf("%s\n", p);
	printf("p[MMAPSIZ-1] %c\n", p[MMAPSIZ-1]);
	p[MMAPSIZ-1] = 'z';
	munmap(p, MMAPSIZ);
	close(fd);

	return 0;
}
