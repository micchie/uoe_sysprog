#include <sys/socket.h>
#include <sys/poll.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CLIENTS 1000
#define MAX_EVENTS 100
#define BUFLEN 128

int
add_fd_epoll(int fd, int epfd)
{
	struct epoll_event evt;
	memset(&evt, 0, sizeof(struct epoll_event));
	evt.events = POLLIN;
	evt.data.fd = fd;
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, evt.data.fd, &evt)) {
		perror("epoll_ctl");
		return -1;
	}
	return 0;
}

int
main(int argc, char *argv[])
{
	struct sockaddr_storage ss;
	struct sockaddr_in *sin;
	int fd, error;
	u_int len;
	char buf[BUFLEN];

	struct epoll_event evts[MAX_EVENTS];
	int epfd;
	int i;

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

	epfd = epoll_create1(EPOLL_CLOEXEC);
	if (epfd < 0) {
		perror("epoll_create1");
		goto close_fd;
	}

	if (add_fd_epoll(fd, epfd)) {
		goto close_epfds;
	}

	for (;;) {
		int nfds;

		nfds = epoll_wait(epfd, evts, MAX_EVENTS, 2000);
		if (nfds < 0) {
			perror("epoll_wait");
			break;
		}
		for (i = 0; i < nfds; ++i) {
			int cur_fd = evts[i].data.fd;

			if (cur_fd == fd) {
				int newfd;

				memset(&ss, 0, sizeof(ss));
				newfd = accept(fd, (struct sockaddr *)&ss, &len);
				printf("accepted newfd %d\n", newfd);
				if (add_fd_epoll(newfd, epfd)) {
					goto close_epfds;
				}
				continue;
			}
			memset(buf, 0, sizeof(buf));
			len = read(cur_fd, buf, sizeof(buf));
			write(1, buf, sizeof(buf));
			if (strncmp(buf, "quit", 4) == 0) {
				close(cur_fd);
				continue;
			}
			len = write(cur_fd, buf, sizeof(buf));
		}

	}
close_epfds:
	close(epfd);
close_fd:
	close(fd);
	return 0;
}
