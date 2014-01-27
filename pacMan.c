// #include "object.h"
#include "header.h"

void calcPacManNextDirection(){
	if (pacManCurCood.x!=COODX(pacManPosition.x) || pacManCurCood.y!=COODY(pacManPosition.y)) return;
	if (pacManNextDirection != KEEP) {
		pacManCurDirection = pacManNextDirection;
		pacManNextDirection = KEEP;
	}
	if (!canMove2(&pacManPosition, pacManCurDirection, map)) {
		preDirection = pacManCurDirection;
		pacManCurDirection = STOP;
	}
//	DEBUG_PRINTF("curpos x: %d, y: %d\n", pacManPosition.x, pacManPosition.y);
	//check dot
	if (has[pacManPosition.x][pacManPosition.y]) {
//		DEBUG_PRINT("eat a dot\n");
		has[pacManPosition.x][pacManPosition.y] = 0;
		dotNum--;
		score+=pacDotScore;
		writeNum(score, 1, 55, 35, 10, YELLOWIDX);

		if (dotNum==0) {
			//win
			win = 1;
			return;
		}
	}
}

void updatePacMan(){
//	DEBUG_PRINTF("cood pos before update cx: %d, cy: %d, px: %d, py: %d\n", pacManCurCood.x, pacManCurCood.y, pacManPosition.x, pacManPosition.y);
	updatePosition(&pacManCurCood, &pacManPosition, &preUseless);
//	DEBUG_PRINTF("pos after update x: %d, y: %d\n", pacManPosition.x, pacManPosition.y);
	if (pacManCurDirection!=STOP) {
		drawRect(pacManCurCood.x-7, pacManCurCood.y-7, 14,14,BLACKIDX);
		drawImage(pacManNextCood.x-7, pacManNextCood.y-7, 14,14,pac[pacManCurDirection][open]);
		pacManCurCood = pacManNextCood;
	}
}