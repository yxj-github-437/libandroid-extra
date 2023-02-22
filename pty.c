#include <errno.h>
#include <fcntl.h>
#include <pty.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <utmp.h>

int openpty(int* pty, int* tty, char* name, const struct termios* t, const struct winsize* ws)
{
    *pty = getpt();
    if (*pty == -1) {
        return -1;
    }

    if (grantpt(*pty) == -1 || unlockpt(*pty) == -1) {
        close(*pty);
        return -1;
    }

    char buf[32];
    if (name == NULL) {
        name = buf;
    }
    if (ptsname_r(*pty, name, sizeof(buf)) != 0) {
        close(*pty);
        return -1;
    }

    *tty = open(name, O_RDWR | O_NOCTTY);
    if (*tty == -1) {
        close(*pty);
        return -1;
    }

    if (t != NULL) {
        tcsetattr(*tty, TCSAFLUSH, t);
    }
    if (ws != NULL) {
        ioctl(*tty, TIOCSWINSZ, ws);
    }

    return 0;
}

int forkpty(int* parent_pty, char* child_tty_name, const struct termios* t, const struct winsize* ws)
{
    int pty;
    int tty;
    if (openpty(&pty, &tty, child_tty_name, t, ws) == -1) {
        return -1;
    }

    pid_t pid = fork();
    if (pid == -1) {
        close(pty);
        close(tty);
        return -1;
    }

    if (pid == 0) {
        // Child.
        *parent_pty = -1;
        close(pty);
        if (login_tty(tty) == -1) {
            _exit(1);
        }
        return 0;
    }

    // Parent.
    *parent_pty = pty;
    close(tty);
    return pid;
}

int login_tty(int fd)
{
    setsid();

    if (ioctl(fd, TIOCSCTTY, NULL) == -1) {
        return -1;
    }

    dup2(fd, STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
    if (fd > STDERR_FILENO) {
        close(fd);
    }

    return 0;
}
