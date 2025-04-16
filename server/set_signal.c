/*
** EPITECH PROJECT, 2025
** Jetpack
** File description:
** set_signal
*/

#include <bits/types/sigset_t.h>
#include <errno.h>
#include <stddef.h>
#include <stdbool.h>
#include <signal.h>
#include <stdio.h>
#include <sys/poll.h>
#include <sys/signalfd.h>
#include <unistd.h>

int set_signal(void)
{
    sigset_t mask;
    int sfd = -1;

    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1) {
        perror("sigprocmack");
    }
    sfd = signalfd(-1, &mask, 0);
    if (sfd == -1) {
        perror("signalfd");
    }
    return sfd;
}

bool handle_signal(struct pollfd *restrict pfd)
{
    static struct signalfd_siginfo fdsi;
    ssize_t s = 0;

    if (pfd->revents & POLLIN) {
        s = read(pfd->fd, &fdsi, sizeof(fdsi));
        if (s != sizeof(fdsi)) {
            perror("read");
            return false;
        }
        if (fdsi.ssi_signo == SIGINT) {
            puts("\nReceived SIGINT. Exiting loop...");
            errno = 0;
            return false;
        }
    }
    return true;
}
