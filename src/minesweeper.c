//RECOMMENDED TERMINAL SIZE: 155 X 45

#include "minesweeper.h"

/* global variables */
int rows = 9, cols = 9, mines = 10, minesleft = 10;
int color = 1;
int cursory, cursorx;
int begin, end;
time_t roundstart, gamestart, pausestart, pausestop, totalpause;
Board x;

int main()
{

	/* window initializations */
	WINDOW *mainwnd = initscr();
	WINDOW *gamewnd = NULL;
	WINDOW *boardwnd = NULL;

	/* other initializations */
	curs_set(0);
	clear();
	noecho();
	cbreak();
	keypad(mainwnd, TRUE);
	nodelay(mainwnd, TRUE);
	ESCDELAY = 25;

	/* color pairs */
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(3, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(4, COLOR_CYAN, COLOR_BLACK);
	init_pair(5, COLOR_BLACK, COLOR_YELLOW);
	init_pair(6, COLOR_WHITE, COLOR_BLACK);
	attron(COLOR_PAIR(color));

	/* application start time */
	roundstart = time(NULL);

	/* random number generation seed */
	srand(time(NULL));

	/* menu options */
	char *options1[] = {"New game", "Settings", "About", "Quit"};
	char *options2[] = {"Resume", "New game", "Settings", "About", "Quit"};
	char *settings[] = {"Number of rows", "Number of columns",
						"Number of mines", "Color scheme", "Back"};
	char *colors[] = {"Green", "Red", "Purple", "Blue", "Back"};
 
	int optionsnr = 5, settingsnr = 5, selection = 0, i, resume_available = 0;
	
	/* dynamic memory allocation for the game board */
	x.map = (int**)calloc(ROWSMAX + 2, sizeof(int*));
	for(i = 0; i < ROWSMAX + 2; i++)
		x.map[i] = (int*)calloc(COLSMAX + 2, sizeof(int));
	x.reveal = (int**)calloc(ROWSMAX + 2, sizeof(int*));
	for(i = 0; i < ROWSMAX + 2; i++)
		x.reveal[i] = (int*)calloc(COLSMAX + 2, sizeof(int));

	/* application must start in a new game, not in the main menu */
	goto firsttime;

	while (1) {
		clear();
		display_env(mainwnd);

		/* show main menu with or without resume option */		
		if(!resume_available) {
			optionsnr = 4;
			selection = menu(options1, optionsnr, selection); 
		}
	  	else {
			optionsnr = 5;
			selection = menu(options2, optionsnr, selection);
		}			
		

		/* resume */
		if(selection == optionsnr - 5) {
				pausestop = time(NULL);
				totalpause = totalpause + pausestop - pausestart; 
				goto play;
		}

		/* new game */
		if(selection == optionsnr - 4) {
			firsttime: // for initial goto
			reset();

			play: // when resume is chosen, don't reset board
			clear();
			gamewnd = newwin(0, 0, 0, 0);
			boardwnd = newwin(rows + 2, 3 * cols + 2, 3, 8);
			wattron(gamewnd, COLOR_PAIR(color));
			wattron(boardwnd, COLOR_PAIR(color));  

			/* game main cycle */
			while(1) {
			int p;
			if(!begin)
				gamestart = time(NULL);
			werase(boardwnd);
			wborder(boardwnd, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
			p = get_key_pressed();
			print_info(gamewnd);
			print_board(boardwnd);
			if(!end)
				action(p);
			if(p == 27) { //escape key
				if(!end)
					resume_available = 1;
				pausestart = time(NULL);
				break;
			}
			if(p == 'n' && end)
				goto firsttime;

			/* check the game outcome */
			if(!end)
			end = check_end();
			
			if(end) {
				wattron(gamewnd, A_BOLD);
				if(end == GOOD) {
						minesleft = 0;
						mvwprintw(gamewnd, 14, 3*cols + 20, "CONGRATULATIONS! YOU WON!");
						
				}
				if(end == BAD) {
						reveal();
						mvwprintw(gamewnd, 14, 3*cols + 20, "BOOM! YOU LOST!");
					}
				mvwprintw(gamewnd, 15, 3*cols + 20, "Esc to return to main menu");
				mvwprintw(gamewnd, 16, 3*cols + 20, "N to start a new game");
				wattroff(gamewnd, A_BOLD);
			}

			wrefresh(gamewnd);
			wrefresh(boardwnd);
			}
		}

		/* settings */
		if(selection == optionsnr - 3) {
			selection = 0;
			while(1) {
				clear();
				display_env(mainwnd);
				getch();
				selection = menu(settings, settingsnr, selection);

				/* mine number must remain valid */
				if(rows * cols <= mines)
					mines = rows * cols / 5 + 1;

				/* number of rows */
				if(selection == 0) {
					set(mainwnd, &rows, ROWSMAX);		
					resume_available = 0;
				}
				/* number of columns */
				if(selection == 1) {
					set(mainwnd, &cols, COLSMAX);
					resume_available = 0;
				}
				/* number of mines */
				if(selection == 2) {
					set(mainwnd, &mines, rows * cols - 1);
					minesleft = mines;
					resume_available = 0;
				}
			
				/* color scheme */
				if(selection == 3) {
					selection = 0;
					while(1) {
						clear();
						display_env(mainwnd);
						getch();
						selection = menu(colors, settingsnr, selection);
						if (selection < 4) {
							color = selection + 1;
							wattron(mainwnd, COLOR_PAIR(color));
						}
						if (selection == 4)
							break;
					}
				selection = 0;
				}
				
				/* back */
				if(selection == 4) 
					break;
			}
			selection = 0;
		}

		/* about */
		if(selection == optionsnr - 2) {
			int key;
				clear();
				display_env(mainwnd);
				attron(A_BOLD);
				mvprintw(14, 70, "MINESWEEPER");
				attroff(A_BOLD);
				mvprintw(19, 45, "The goal of the game is to hit all cells that are not mined");
				mvprintw(20, 45, "You can flag the cells which you believe are mined using the M key");
				mvprintw(21, 45, "Be careful! One mistake and the game is over");
				mvprintw(22, 45, "You can change the color, number of rows, columns and mines in Settings");
				mvprintw(23, 45, "Press Esc to return to the main menu and start playing");
				mvprintw(26, 45, "This game was created by Bogdan Stefan");
				
			while(1) {
				key = get_key_pressed();
				if(key == 27)
					break;
			}
		}
				
				

		/* Quit */
		if(selection == optionsnr - 1)
			break;

        refresh();

	}

	destroy_board(&x);

	/* destroy windows before exiting */
	delwin(gamewnd);
	delwin(mainwnd);
	delwin(boardwnd);
	endwin();
	return 0;
}


/** show a menu with input options */
int menu(char **options, int optionsnr, int selection) {
	curs_set(0);
	int key, i;
	
	/* show options */
	while(1) {
		for(i = 0; i < optionsnr; i++) {
			if(i == selection)
				attron(A_REVERSE);
			mvprintw(LINES/2 + i, COLS/2 - 4, "%s", options[i]);
			attroff(A_REVERSE);
		}

		key = get_key_pressed();
	
	/* check pressed key */	
		switch(key) {
			case KEY_UP:
				if(selection)
					selection--;
				else
					selection = optionsnr - 1;
				break;
			case KEY_DOWN:
				if(selection == optionsnr - 1)
					selection = 0;
				else
					selection ++;
				break;
			case 10: /* ENTER */
				return selection;
			case ' ': /* SPACEBAR */
				return selection;
			case 27: /* ESCAPE */
				return optionsnr - 1;
		}
	}
	return -1;
}

/** display game logo and borders */
void display_env(WINDOW *wnd) {
	FILE *p = fopen ("logo.txt" , "r");;
	char str[200];
	int i = 1;

	while(fgets (str, 200, p) != NULL)
       mvprintw(i++, COLS / 2 - 50, "%s", str);
	//mvprintw(LINES - 2, 3, "%s", "Bogdan Stefan 2014 (c)");
	attron(A_REVERSE);
	wborder(wnd, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	attroff(A_REVERSE);

    fclose (p);
}

/* print time left in the wnd window */
void print_time(WINDOW *wnd, int rows, int cols) {
		time_t currenttime, roundtime, gametime;
		char *ctimestr, *rtimestr = (char*)calloc(8,1);

		currenttime = time(NULL);
		ctimestr = ctime(&currenttime);
		roundtime = currenttime - roundstart;
		gametime = currenttime - gamestart - totalpause;

		convert_time(roundtime, rtimestr);

		mvwprintw(wnd, rows, 3*cols+40, "Current date and time");
		mvwprintw(wnd, rows + 1, 3*cols+40,  "%s", ctimestr);
		mvwprintw(wnd, rows + 3, 3*cols+40, "Round is running for");
		mvwprintw(wnd, rows + 4, 3*cols+40, "%s", rtimestr);
		if(!end) {
			mvwprintw(wnd, rows + 6, 3*cols+40, "Game time");
			mvwprintw(wnd, rows + 7, 3*cols+40, "%ds", gametime);
		}
		free(rtimestr);
}

/* display additional info */
void print_info(WINDOW *wnd) {
	print_time(wnd, 3, cols);
	wattron(wnd, A_BOLD);
	mvwprintw(wnd, 3, 3*cols+18, "--------------");
	 //"clear" the previous characters if number has less digits
	mvwprintw(wnd, 4, 3*cols+18, "Mines left:       ");
	mvwprintw(wnd, 4, 3*cols+18, "Mines left: %d", minesleft); 
	mvwprintw(wnd, 5, 3*cols+18, "--------------"); 
	wattroff(wnd, A_BOLD);
	mvwprintw(wnd, 7, 3*cols+18, "X to hit");
	mvwprintw(wnd, 8, 3*cols+18, "M to flag");
	mvwprintw(wnd, 9, 3*cols+18, "Esc to pause/exit");	
}

/* display game board */
void print_board(WINDOW *wnd) {
	int i, j;
	for(i = 1; i <= rows; i++)
		for(j = 1; j <= cols; j++) {
			if(!x.reveal[i][j]) {
				wattron(wnd, COLOR_PAIR(6));
				mvwaddch(wnd, i, 3 * j - 1, ACS_BULLET);
				wattroff(wnd, COLOR_PAIR(6));
				wattron(wnd, COLOR_PAIR(color));
			}
			if(x.reveal[i][j] == 1) {
				if(x.map[i][j] > 0)
					mvwprintw(wnd, i, 3 * j - 1, "%d", x.map[i][j]);
				if(x.map[i][j] == MINE) {
					wattron(wnd, COLOR_PAIR(5));
					mvwprintw(wnd, i, 3 * j - 1, "*");
					wattroff(wnd, COLOR_PAIR(5));
				}
			}
			if(x.reveal[i][j] == FLAG)
				mvwaddch(wnd, i, 3 * j - 1, ACS_DIAMOND);
				if (cursory == i && cursorx == j) {
					mvwprintw(wnd, i, 3 * j - 2, "[");
					mvwprintw(wnd, i, 3 * j, "]");
			}
		}
}

/* execute input action */
void action(int p) {
		switch(p) {
			case KEY_UP:
				if(cursory == 1)
					cursory = rows;
				else
					cursory--;
				break;
			case KEY_DOWN:
				if(cursory == rows)
					cursory = 1;
				else
					cursory++;
				break;
			case KEY_LEFT:
				if(cursorx == 1)
					cursorx = cols;
				else
					cursorx--;
				break;
			case KEY_RIGHT:
				if(cursorx == cols)
					cursorx = 1;
				else
					cursorx++;
				break;
			case 'x': // HIT
				if(!begin) { // prima lovitura
					gamestart = time(NULL);
					generate_mines(cursory, cursorx);
					populate_board();
					begin = 1;
				}
				if(x.reveal[cursory][cursorx] == 0)
					hit(cursory,cursorx);
				else
					if(x.reveal[cursory][cursorx] == 1)
						multiple_hit(cursory, cursorx);
				break;
			case 'm': // FLAG
				if(x.reveal[cursory][cursorx] == FLAG) {
					x.reveal[cursory][cursorx] = 0;
					minesleft++;
				}
				else
				if(x.reveal[cursory][cursorx] == 0) {
					x.reveal[cursory][cursorx] = FLAG;
					minesleft--;
				}
				else
				if(x.reveal[cursory][cursorx] == 1)
					multiple_flag(cursory, cursorx);
				break;
		}		
}

/* reset board for a new game */
void reset() {
	int i, j;
	roundstart = time(NULL);
	totalpause = 0;
	begin = 0;
	end = 0;
	cursory = rows / 2 + 1;
	cursorx = cols / 2 + 1;
	minesleft = mines;
	for(i = 0; i <= rows + 1; i++)
		for(j = 0; j <= cols + 1; j++)
			if(!i || !j || i > rows || j > cols)
				x.map[i][j] = x.reveal[i][j] = -2;
			else
				x.map[i][j] = x.reveal[i][j] = 0;
}

/* generate mines randomly */
void generate_mines(int cursory, int cursorx) {
	int i, a, b;
	for(i = 1; i <= mines; i++) {
		a = rand_n(rows);
		b = rand_n(cols);
		if(x.map[a][b] == MINE || (a == cursory && b == cursorx))
			i--;
		else
			x.map[a][b] = MINE;
	}
	
}

/* populate the board with number of adjacent mines on each square */ 
void populate_board() {
	int a, b, c, d;
	for(a = 1; a <= rows; a++)
		for(b = 1; b <= cols; b++) {
			if(x.map[a][b] != MINE)
			for(c = a - 1; c <= a + 1; c++)
				for(d = b - 1; d <= b + 1; d++)
					if(x.map[c][d] == MINE)
						x.map[a][b]++;
		}
} 

/* check if game has ended */
int check_end() {
	int i,j;
	for(i = 1; i <= rows; i++)
		for(j = 1; j <= cols; j++)
			if(!x.reveal[i][j] && x.map[i][j] != MINE)
				return 0;
	return GOOD;
}

/* reveal all the mines and numbers */
void reveal() {
	int i,j;
	for(i = 1; i <= rows; i++)
		for(j = 1; j <= cols; j++)
			x.reveal[i][j] = 1;
}

/* hit selected cell and adjacent ones, if applyable */
void hit(int a, int b) {
	if(!x.reveal[a][b])
		x.reveal[a][b] = 1;
	if(x.map[a][b] == MINE)
		end = BAD;
	if(x.map[a][b] == 0) {
		if(!x.reveal[a-1][b-1])
			hit(a - 1, b - 1);
		if(!x.reveal[a-1][b])
			hit(a - 1, b);
		if(!x.reveal[a-1][b+1])
			hit(a - 1, b + 1);
		if(!x.reveal[a][b-1])
			hit(a, b - 1);
		if(!x.reveal[a][b+1])
			hit(a, b + 1);
		if(!x.reveal[a+1][b-1])
			hit(a + 1, b - 1);
		if(!x.reveal[a+1][b])
			hit(a + 1, b);
		if(!x.reveal[a+1][b+1])
			hit(a + 1, b + 1);
	}
}	

/* multiple hit selected cell */
void multiple_hit(int a, int b) {
	int i, j, count = 0;
	for(i = a - 1; i < a + 2; i++)
		for(j = b - 1; j < b + 2; j++)
			if(i != a || j != b)
				if(x.reveal[i][j] == FLAG)
					count++;
	if(count == x.map[a][b])
		for(i = a - 1; i < a + 2; i++)
			for(j = b - 1; j < b + 2; j++)
					if(!x.reveal[i][j])
						hit(i, j);
}

/* multiple flag selected cell */
void multiple_flag(int a, int b) {
	int i, j, count = 0;
	for(i = a - 1; i < a + 2; i++)
		for(j = b - 1; j < b + 2; j++)
			if(i != a || j != b)
				if(!x.reveal[i][j] || x.reveal[i][j] == FLAG)
					count++;
	if(count == x.map[a][b])
		for(i = a - 1; i < a + 2; i++)
			for(j = b - 1; j < b + 2; j++)
					if(!x.reveal[i][j])
						x.reveal[i][j] = FLAG;
}

/* set variable input by var variable */
void set(WINDOW *wnd, int *var, int max) {
	char string[3];

	echo();
	nocbreak();
	nodelay(wnd, FALSE);

	do {
	clear();
	display_env(wnd);

	if(*var < 2 || *var > max) {
		mvwprintw(wnd, LINES/2 + 3, COLS/2 - 20, "You entered an invalid value, please try again");
		string[0] = 0;
	}
	mvwprintw(wnd, LINES/2, COLS/2 - 25, "Please enter the new value (curr. %d)(max. %d): ", *var, max);
	wrefresh(wnd);
	wgetstr(wnd, string);
	*var = atoi(string);
	} while(*var < 2 || *var > max);

	noecho();
	cbreak();
	nodelay(wnd, TRUE);
}

/**
 * returns the pressed key charcater code or NO_KEY_PRESSED
 * if no key was pressed
 */
int get_key_pressed() {
	int ch = getch();
	if (ch != ERR) {
		return ch;
	}
	return NO_KEY_PRESSED;
}

/* transforms time in seconds(int) into a string with HH:MM:SS format*/
void convert_time(int time, char *timestr) {
	int h, m, s;
	h = time / 3600;
	time %= 3600;
	m = time / 60;
	time %= 60;
	s = time;
	sprintf (timestr, "%.2d:%.2d:%.2d", h, m, s);
}

/** memory free */
void destroy_board(Board *board) {
	int i = 0;
	while(board->map[i] != NULL) {
		free(board->map[i]);
		free(board->reveal[i]);
		i++;
	}
	free(board->map);
	free(board->reveal);
}

/* returns a random integer between 1 and n */
int rand_n(int n) {
	return rand() % n + 1;
}
