#include <unistd.h>
#include <curses.h>
#include <stdlib.h>

#define HALTKEY ' '
#define RSEED 82657378
#define TIMEPERFRAME 33300 // Time spent showing frame
#define MAXDROPS 4096 // Max allocated drops
#define MAXDROPSPEED 2
#define SCREENTORAINRATIO 6 // Amount of screen to amount of rain (0-10)

char rainType[MAXDROPSPEED] = ":|";

typedef struct {
	int x;
	int y;
	int speed;
} drop;

drop rainArr[MAXDROPS];
int usedDrops = 0;

int main() {
	srand(RSEED);
	int keypress = 0;
	int width = 0;
	int height = 0;
	int screenRatio = 0;

	initscr();
	timeout(0);
	curs_set(0);
	cbreak();
	getmaxyx(stdscr, height, width);

	for (int i = 0; i < MAXDROPS; i++) {
		rainArr[i].x = rand()%width;
		rainArr[i].y = 0-(rand()%50+1);
		rainArr[i].speed = rand()%MAXDROPSPEED+1;
	}

	while(1) {
		getmaxyx(stdscr, height, width);
		usedDrops = (width*height)>>1*SCREENTORAINRATIO;

		for (int i = 0; i < usedDrops; i++) {
			if (i > MAXDROPS) { continue; }

			// Update positions
			rainArr[i].y += rainArr[i].speed;
			if (rainArr[i].y > height) {
				rainArr[i].y = 0-(rand()%10+1);
				rainArr[i].x = rand()%width;
			}

			// Draw
			if (rainArr[i].x < 0 || rainArr[i].x > width) { continue; }
			if (rainArr[i].y < 0 || rainArr[i].y > height) { continue; }
			mvaddch(rainArr[i].y, rainArr[i].x, rainType[rainArr[i].speed-1]);
		}

		// Clear
		refresh();
		usleep(TIMEPERFRAME);
		keypress = getch();
		if (keypress == HALTKEY) {
			endwin();
			break;
		}
		erase();
	}

}

