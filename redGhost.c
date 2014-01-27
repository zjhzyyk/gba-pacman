// #include "object.h"
#include "header.h"

void calcRedNextDirection(){
	//random pick next position. never go back
	restoreDot(&redCurCood, &redPrePosition, redCurDirection);
	if (redCurCood.x!=COODX(redPosition.x) || redCurCood.y!=COODY(redPosition.y)) return;
	stackSize = 0; //clear stack
	if (redCurDirection!=DOWN && canMove(&redPosition, UPWARD, map)) stack[stackSize++] = UP;
	if (redCurDirection!=UP && canMove(&redPosition, DOWNWARD, map)) stack[stackSize++] = DOWN;
	if (redCurDirection!=RIGHT && canMove(&redPosition, LEFTWARD, map)) stack[stackSize++] = LEFT;
	if (redCurDirection!=LEFT && canMove(&redPosition, RIGHTWARD, map)) stack[stackSize++] = RIGHT;
	if (stackSize==1)
		redCurDirection = stack[0];
	else if (stackSize)
		redCurDirection = stack[rand()%stackSize];
	else
		redCurDirection = STOP; 

}

void updateRed(){
	updatePosition(&redCurCood, &redPosition, &redPrePosition);
	u16 tmp = killPacMan(&redCurCood);
	if (tmp==2) {
		gameOver = 1;
		return;
	}
	else if (tmp==1) {
		redNextCood.x = COODX(ptArr[maxDotNum-1].x);
		redNextCood.y = COODY(ptArr[maxDotNum-1].y);
		redPosition.x = ptArr[maxDotNum-1].x;
		redPosition.y = ptArr[maxDotNum-1].y;
	}
	drawRect(redCurCood.x-7, redCurCood.y-8, 14,15,BLACKIDX);
	drawDot(&redPrePosition);
	drawImage(redNextCood.x-7, redNextCood.y-7, 14,14,red);
	redCurCood = redNextCood;
}