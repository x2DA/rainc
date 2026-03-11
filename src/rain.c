#include <unistd.h>
#include <curses.h>
#include <stdlib.h>

#define HALTKEY ' ' // Key used to stop
#define FLOORKEY 'v' // Key used to draw as if there was a floor

#define RSEED 82657378
#define TIMEPERFRAME 33300 // Time spent showing frame
#define INITIALMAXDROPS 4096 // Max allocated drops
#define MAXDROPSPEED 2
#define SCREENTORAINRATIO 6 // Amount of screen to amount of rain (0-10)

char rainType[MAXDROPSPEED*2] = ":|V@";

typedef struct {
	int x;
	int y;
	int speed;
} drop;

int allocatedDrops = 0;
int neededDrops = 0;

int running = 1;
int doFloor = 0;

/* Creates a drop at position i.
 * Does not check the size of the drop array.
 */
void generateDropAt(drop *rArr, int i, int w);


int main() {
	srand(RSEED);

	drop *rainArr = (drop *)malloc(allocatedDrops*sizeof(drop));

	int keypress = 0;
	int width = 0;
	int height = 0;
	int screenRatio = 0;

	initscr();
	timeout(0);
	curs_set(0);
	cbreak();
	getmaxyx(stdscr, height, width);

	for (int i = 0; i < allocatedDrops; i++) {
		generateDropAt(rainArr, i, width);
	}

	while(running) {
		getmaxyx(stdscr, height, width);
		neededDrops = (width*height)>>1*SCREENTORAINRATIO; // Calc. needed drops

		if (neededDrops != allocatedDrops) { // re-alloc if we have more or less
											 // drops than needed
			rainArr = (drop *)realloc(rainArr, neededDrops*sizeof(drop));

			for (int i = allocatedDrops; i < neededDrops; i++) {
				generateDropAt(rainArr, i, width);
			}
			allocatedDrops = neededDrops;
		}


		for (int i = 0; i < neededDrops; i++) {

			// Update positions
			rainArr[i].y += rainArr[i].speed;
			if (rainArr[i].y > height) {
				rainArr[i].y = 0-(rand()%10+1);
				rainArr[i].x = rand()%width;
			}

			// Draw
			if (rainArr[i].x < 0 || rainArr[i].x > width) { continue; }
			if (rainArr[i].y < 0 || rainArr[i].y > height) { continue; }

			char dropType = ( doFloor && (rainArr[i].y >= height-1)) ? rainType[rainArr[i].speed+1] : rainType[rainArr[i].speed-1];
			mvaddch(rainArr[i].y, rainArr[i].x, dropType);

		}

		// Clear
		refresh();
		usleep(TIMEPERFRAME);
		keypress = getch();

		switch(keypress) {
			case HALTKEY:
				endwin();
				free(rainArr);
				running = 0;
			break;

			case FLOORKEY:
				doFloor = !doFloor;
			break;
		}

		erase();
	}

}


void generateDropAt(drop *rArr, int i, int w) {
	rArr[i].x = rand()%w;
	rArr[i].y = 0-(rand()%50+1);
	rArr[i].speed = rand()%MAXDROPSPEED+1;
}

