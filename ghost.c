// #include "object.h"
#include "header.h"

u16 killPacMan(point* ghostCood){
	if (ghostCood->x > pacManCurCood.x-killRadius &&
			ghostCood->x < pacManCurCood.x+killRadius &&
			ghostCood->y > pacManCurCood.y-killRadius &&
			ghostCood->y < pacManCurCood.y+killRadius) {
		lives--;
		drawRect(pacManCurCood.x-7, pacManCurCood.y-7, 14,14,BLACKIDX);
		initPacMan();
		writeNum(lives, 1, 140, 35, 10, YELLOWIDX);
		if (lives==0) return 2; //game over
		return 1;
	}
	else
		return 0;
}

void restoreDot(point* curCood, point* prePosit, u16 curDir) {
//	DEBUG_PRINTF("curDir %d cx: %d, cy: %d, px: %d, py: %d, in restoredot\n", curDir, curCood->x, curCood->y,
//			COODX(prePosit->x), COODY(prePosit->y));
	switch (curDir) {
	case UP: {
		if ((curCood->x+8)==COODX(prePosit->x) && has[prePosit->x][prePosit->y]) {
			drawDot(prePosit);
		}
		break;
	}
	case DOWN: {
		if ((curCood->x-8)==COODX(prePosit->x) && has[prePosit->x][prePosit->y]) {
			drawDot(prePosit);
		}
		break;
	}
	case LEFT: {
		if ((curCood->y+8)==COODY(prePosit->y) && has[prePosit->x][prePosit->y]) {
			drawDot(prePosit);
		}
		break;
	}
	case RIGHT: {
		if ((curCood->y-8)==COODY(prePosit->y) && has[prePosit->x][prePosit->y]) {
			drawDot(prePosit);
		}
		break;
	}
	default: break;
	}
}