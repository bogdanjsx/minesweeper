#ifndef __MINESWEEPER_H__
#define __MINESWEEPER_H__

#include <curses.h>	/* curses includes stdio.h */
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define NO_KEY_PRESSED (-1)
#define ROWSMAX 30
#define COLSMAX 30
#define FLAG 2
#define MINE (-1)
#define GOOD 1
#define BAD 2

/* game board struct */
typedef struct Board {
	/* number of mines array */
	int **map;
	/* reveal state array */
	int **reveal;
} Board;

/* function signatures */
int menu(char **options, int optionsnr, int selection);
void display_env(WINDOW *wnd);
void set(WINDOW *wnd, int *var, int max);
void print_info(WINDOW *wnd);
void print_board(WINDOW *wnd);
void action(int p);
void reset();
void generate_mines(int cursory, int cursorx);
void populate_board();
int check_end();
void reveal();
void hit(int y, int x);
void multiple_hit(int a, int b);
void multiple_flag(int a, int b);
int get_key_pressed();
void convert_time(int time, char *timestr);
int rand_n(int n);
void destroy_board(Board *board);

#endif
