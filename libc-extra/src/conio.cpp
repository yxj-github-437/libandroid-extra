#include "conio.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

class _conio
{
private:
    int bgc;

public:
    _conio()
    {
        bgc = 40;
        check_teminal();
    }

    ~_conio()
    {
        printf("\033[0m");
    }

    static void check_teminal(void)
    {
        if (!isatty(STDIN_FILENO)) {
            fprintf(stderr, "Not a terminal.\n");
            exit(EXIT_FAILURE);
        }
    }

    void clreol()
    {
        printf("\033[K");
    }

    void insline()
    {
        printf("\x1b[1L");
    }

    void delline()
    {
        printf("\033[1M");
    }

    void gotoxy(int x, int y)
    {
        printf("\033[%d;%df", y, x);
        fflush(stdout);
    }

    void clrscr()
    {
        printf("\033[%dm\033[2J\033[1;1f", bgc);
        fflush(stdout);
    }

    void textbackground(int color)
    {
        switch (color % 16) {
            case BLACK:
                bgc = 40;
                break;
            case BLUE:
                bgc = 44;
                break;
            case GREEN:
                bgc = 42;
                break;
            case CYAN:
                bgc = 46;
                break;
            case RED:
                bgc = 41;
                break;
            case MAGENTA:
                bgc = 45;
                break;
            case BROWN:
                bgc = 43;
                break;
            case LIGHTGRAY:
                bgc = 47;
                break;
            case DARKGRAY:
                bgc = 40;
                break;
            case LIGHTBLUE:
                bgc = 44;
                break;
            case LIGHTGREEN:
                bgc = 42;
                break;
            case LIGHTCYAN:
                bgc = 46;
                break;
            case LIGHTRED:
                bgc = 41;
                break;
            case LIGHTMAGENTA:
                bgc = 45;
                break;
            case YELLOW:
                bgc = 43;
                break;
            case WHITE:
                bgc = 47;
                break;
        }
    }

    void textcolor(short color)
    {
        switch (color % 16) {
            case BLACK:
                printf("\033[0;%d;%dm", 30, bgc);
                break;
            case BLUE:
                printf("\033[0;%d;%dm", 34, bgc);
                break;
            case GREEN:
                printf("\033[0;%d;%dm", 32, bgc);
                break;
            case CYAN:
                printf("\033[0;%d;%dm", 36, bgc);
                break;
            case RED:
                printf("\033[0;%d;%dm", 31, bgc);
                break;
            case MAGENTA:
                printf("\033[0;%d;%dm", 35, bgc);
                break;
            case BROWN:
                printf("\033[0;%d;%dm", 33, bgc);
                break;
            case LIGHTGRAY:
                printf("\033[0;%d;%dm", 37, bgc);
                break;
            case DARKGRAY:
                printf("\033[1;%d;%dm", 30, bgc);
                break;
            case LIGHTBLUE:
                printf("\033[1;%d;%dm", 34, bgc);
                break;
            case LIGHTGREEN:
                printf("\033[1;%d;%dm", 32, bgc);
                break;
            case LIGHTCYAN:
                printf("\033[1;%d;%dm", 36, bgc);
                break;
            case LIGHTRED:
                printf("\033[1;%d;%dm", 31, bgc);
                break;
            case LIGHTMAGENTA:
                printf("\033[1;%d;%dm", 35, bgc);
                break;
            case YELLOW:
                printf("\033[1;%d;%dm", 33, bgc);
                break;
            case WHITE:
                printf("\033[1;%d;%dm", 37, bgc);
                break;
        }
    }

    int ungetch(int ch)
    {
        return ungetc(ch, stdin);
    }

    int getch_echo(bool echo = true)
    {
        struct termios oldt, newt;
        int c;

        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~ICANON;

        if (echo) {
            newt.c_lflag &= ECHO;
        }
        else {
            newt.c_lflag &= ~ECHO;
        }

        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        c = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

        return c;
    }

    int getch()
    {
        return getch_echo(false);
    }

    int getche()
    {
        return getch_echo(true);
    }

    void wherexy(int& x, int& y)
    {
        printf("\033[6n");
        if (getch() != '\x1B')
            return;
        if (getch() != '\x5B')
            return;
        int in;
        int ly = 0;
        while ((in = getch()) != ';')
            ly = ly * 10 + in - '0';
        int lx = 0;
        while ((in = getch()) != 'R')
            lx = lx * 10 + in - '0';
        x = lx;
        y = ly;
    }

    int wherex()
    {
        int x = 0, y = 0;
        wherexy(x, y);
        return x;
    }

    int wherey()
    {
        int x = 0, y = 0;
        wherexy(x, y);
        return y;
    }

    int kbhit(void)
    {
        struct termios oldt, newt;
        int ch;
        int oldf;

        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        fcntl(STDIN_FILENO, F_SETFL, oldf);
        if (ch != EOF) {
            ungetc(ch, stdin);
            return 1;
        }

        return 0;
    }

    void setcursortype(int cur_t)
    {
        switch (cur_t) {
            case _NOCURSOR:
                printf("\x1b[?25l");
                break;

            case _NORMALCURSOR:
                printf("\x1b[?25h");
                break;

            case _SOLIDCURSOR:// TODO
                printf("\x1b[?25h");
                break;
        }
    }

    void gettextinfo(struct text_info* r)
    {

        struct winsize w;
        ioctl(0, TIOCGWINSZ, &w);

        r->screenheight = w.ws_row;
        r->screenwidth = w.ws_col;

        int x, y;
        wherexy(x, y);

        r->curx = x;
        r->cury = y;
    }

    void textattr(int)
    {}

    void delay(int ms)
    {
        int secs, millis;
        secs = ms / 1000;
        millis = ms % 1000;
        struct timespec t = {secs, millis * 1000000L};
        nanosleep(&t, 0);
    }
};

static _conio conio;

void gotoxy(int x, int y)
{
    conio.gotoxy(x, y);
}
void clrscr()
{
    conio.clrscr();
}
void textcolor(short color)
{
    conio.textcolor(color);
}
void textbackground(int color)
{
    conio.textbackground(color);
}
void wherexy(int* x, int* y)
{
    conio.wherexy(*x, *y);
}
int wherex()
{
    return conio.wherex();
}
int wherey()
{
    return conio.wherey();
}
int ungetch(int ch)
{
    return conio.ungetch(ch);
}
int getch()
{
    return conio.getch();
}
int getche()
{
    return conio.getche();
}
int kbhit()
{
    return conio.kbhit();
}
void clreol()
{
    conio.clreol();
}
void insline()
{
    conio.insline();
}
void delline()
{
    conio.delline();
}
void setcursortype(int cur_t)
{
    conio.setcursortype(cur_t);
}
void gettextinfo(struct text_info* r)
{
    conio.gettextinfo(r);
}
void textattr(int attr)
{
    conio.textattr(attr);
}
void delay(int ms)
{
    conio.delay(ms);
}
