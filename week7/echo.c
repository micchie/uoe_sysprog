#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc, char *argv[])
{
	struct sockaddr_storage ss;
	struct sockaddr_in *sin;
	int fd, newfd, error;
	u_int len;
	char buf[128];
       
	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (fd < 0) {
		perror("socket");
		return 0;
	}

	memset(&ss, 0, sizeof(ss));
	sin = (struct sockaddr_in *)&ss;
	sin->sin_family = AF_INET;
	sin->sin_port = htons(atoi(argv[1]));
	sin->sin_addr.s_addr = INADDR_ANY;

	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int))) {
		perror("setsockopt");
		goto close_fd;
	}

	error = bind(fd, (struct sockaddr *)sin, sizeof(ss));
	if (error) {
		perror("bind");
		goto close_fd;
	}

	error = listen(fd, 10);
	if (error) {
		perror("listen");
		goto close_fd;
	}

	memset(&ss, 0, sizeof(ss));
	newfd = accept(fd, (struct sockaddr *)&ss, &len);
	if (newfd < 0) {
		perror("accept");
		goto close_fd;
	}
	printf("accepted newfd %d\n", newfd);

	for (;;) {
		memset(buf, 0, sizeof(buf));
		len = read(newfd, buf, sizeof(buf));
		if (len < 0) {
			perror("read");
			goto close_new;
		}
		write(1, buf, sizeof(buf));
		if (strncmp(buf, "quit", 4) == 0) {
			break;
		}
		len = write(newfd, buf, sizeof(buf));
		if (len < 0) {
			perror("write");
			goto close_new;
		}
	}
close_new:
	close(newfd);
close_fd:
	close(fd);
	return 0;
}
