#pragma once

#include <arpa/inet.h>

int Socket(int family, int type, int protocol);

void Bind(int fd, const sockaddr_in *sa, socklen_t salen);

ssize_t Recvfrom (int fd, void *ptr, size_t nbytes, int flags, sockaddr_in *sa, socklen_t *salenptr);

void Sendto(int fd, const void *ptr, int nbytes, int flags, const sockaddr_in *sa, socklen_t salen);

void Setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);

int Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,  timeval *timeout);
