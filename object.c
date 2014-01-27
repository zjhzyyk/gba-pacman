// #include "object.h"
#include "header.h"

u16 canMove(point* curPosition, short x, short y, const char* mp[]){
	// DEBUG_PRINTF("cx %d, cy %d, x: %d, y: %d\n", curPosition->x, curPosition->y, x, y);
	if (curPosition->x==0 && x==-1) return 0;
	if (curPosition->x==13 && x==1) return 0;
	if (curPosition->y==0 && y==-1) return 0;
	if (curPosition->y==22 && y==1) return 0;

	if (mp[((short)curPosition->x)+x][((short)curPosition->y)+y]=='#') {
		return 0;
	}
	else return 1;
}

u16 canMove2(point* curPosition, u16 direction, const char* mp[]) {
	short x,y;
	x=y=0;
	switch (direction) {
	case UP : x=-1;y=0;break;
	case DOWN: x=1;y=0;break;
	case LEFT: x=0;y=-1;break;
	case RIGHT: x=0; y=1;break;
	case STOP: return 0;break;
	}
	return canMove(curPosition, x,y,mp);
}



void calcNextCood(point* curCood, point* nextCood, u16 direction) {
	short x=0, y=0;
	switch (direction) {
	case RIGHT : x=0;y=1;break;
	case LEFT: x=0;y=-1;break;
	case UP: x=-1;y=0;break;
	case DOWN: x=1;y=0;break;
	case STOP: x=0;y=0;break;
	}
	nextCood->x = (u16)((short)curCood->x+x);
	nextCood->y = (u16)((short)curCood->y+y);
//	DEBUG_PRINTF("next Cood: x: %d y: %d\n", nextCood->x, nextCood->y);
}

void updatePosition(point* curCood, point* curPosition, point* prePos) {
	point tmp = *curPosition;
	u16 flag = 0;
	if (curCood->x < (COODX(curPosition->x)-5)) {
		curPosition->x--;
		flag = 1;
	}
	if (curCood->x > (COODX(curPosition->x)+5)) {
		curPosition->x++;
		flag = 1;
	}
	if (curCood->y < (COODY(curPosition->y)-5)) {
		curPosition->y--;
		flag = 1;
	}
	if (curCood->y > (COODY(curPosition->y)+5)) {
		curPosition->y++;
		flag = 1;
	}
	if (flag==1) *prePos=tmp;
}