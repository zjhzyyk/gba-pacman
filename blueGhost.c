// #include "object.h"
#include "header.h"

void updateBlue(){
	updatePosition(&blueCurCood, &bluePosition, &bluePrePosition);
	u16 tmp = killPacMan(&blueCurCood);
	if (tmp==2) {
		gameOver = 1;
		return;
	}
	else if (tmp==1) {
		blueNextCood.x = COODX(ptArr[maxDotNum-1].x);
		blueNextCood.y = COODY(ptArr[maxDotNum-1].y);
		bluePosition.x = ptArr[maxDotNum-1].x;
		bluePosition.y = ptArr[maxDotNum-1].y;
	}
	drawRect(blueCurCood.x-7, blueCurCood.y-8, 14,15,BLACKIDX);
	drawDot(&bluePrePosition);
	drawImage(blueNextCood.x-7, blueNextCood.y-7, 14,14,blue);
	blueCurCood = blueNextCood;
}

void calcBlueNextDirection(){
//	DEBUG_PRINT("in calc blue direction\n");
	restoreDot(&blueCurCood, &bluePrePosition, blueCurDirection);
	if (blueCurCood.x!=COODX(bluePosition.x) || blueCurCood.y!=COODY(bluePosition.y)) return;
	stackSize = 0; //clear stack
	if (blueCurDirection!=DOWN && canMove(&bluePosition, UPWARD, map)) stack[stackSize++] = UP;
	if (blueCurDirection!=UP && canMove(&bluePosition, DOWNWARD, map)) stack[stackSize++] = DOWN;
	if (blueCurDirection!=RIGHT && canMove(&bluePosition, LEFTWARD, map)) stack[stackSize++] = LEFT;
	if (blueCurDirection!=LEFT && canMove(&bluePosition, RIGHTWARD, map)) stack[stackSize++] = RIGHT;
	if (stackSize==1) {
		blueCurDirection = stack[0];
	}
	else if (stackSize) {
//			DEBUG_PRINTF("at x: %d y: %d\n", bluePosition.x, bluePosition.y);
		point np;
		u16 mi=0, mmin=1000, tmp;
		for (u16 i = 0; i<stackSize; i++) {
			calcNextCood(&bluePosition, &np, stack[i]);
			tmp = dijkstra(np.x, np.y, pacManPosition.x, pacManPosition.y);
//				DEBUG_PRINTF("d: %d dist: %d\n", i, tmp);
			if (tmp < mmin) {
				mi = i;
				mmin = tmp;
			}
		}
//			DEBUG_PRINTF("choose %d\n", mi);
		blueCurDirection = stack[mi];
	}
	else
		blueCurDirection = STOP;
}
