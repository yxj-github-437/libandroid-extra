/**
 * conio4linux (Conio.h for linux)
 * Copyright (C) 2013 by Carlos J. Pinto B.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#ifndef __CONIO_H
#define __CONIO_H

#include <stdio.h>
#include <unistd.h>

__BEGIN_DECLS

enum COLORS
{
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    BROWN = 6,
    LIGHTGRAY = 7,
    DARKGRAY = 8,
    LIGHTBLUE = 9,
    LIGHTGREEN = 10,
    LIGHTCYAN = 11,
    LIGHTRED = 12,
    LIGHTMAGENTA = 13,
    YELLOW = 14,
    WHITE = 15,
    BLINK = 128
};

enum CURSORTYPE
{
    _NOCURSOR,   //     turns off the cursor
    _SOLIDCURSOR,//  solid block cursor
    _NORMALCURSOR// normal underscore cursor
};

struct text_info
{
    unsigned char attribute; /* text attribute */
    unsigned char normattr;  /* normal attribute */
    int screenheight;        /* text screen's height */
    int screenwidth;         /* text screen's width */
    int curx;                /* x-coordinate in current window */
    int cury;                /* y-coordinate in current window */
};

#define cprintf printf
#define cscanf scanf
#define cgets gets
#define cputs puts
#define putch putchar

void gotoxy(int x, int y);
void clrscr();
void textcolor(short color);
void textbackground(int color);
void wherexy(int* x, int* y);
int wherex();
int wherey();
int ungetch(int ch);
int getch();
int getche();
int kbhit();
void clreol();
void insline();
void delline();
void setcursortype(int cur_t);
void gettextinfo(struct text_info* r);
void textattr(int attr);
void delay(int ms);

__END_DECLS

#endif
