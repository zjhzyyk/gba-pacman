//author: Yukang Yang
// #include <stdlib.h>
// #include "main.h"
#include "header.h"
//#include <debugging.h>

u16 open = 0;
u16 stack[10];
u16 stackSize = 0;
u16 seed = 1;
u16 redCurDirection = STOP;
u16 blueCurDirection = STOP;
short score = 0;
u16 lives = 3;
u16 gameOver = 0;
u16 win = 0;
short pacDotScore = 1;
u16 pacManNextDirection = KEEP;

void copy() {
	if (videoBuffer==BUFFER1) {
		DMA[3].src = BUFFER0;
		DMA[3].dst = BUFFER1;
		DMA[3].cnt = DMA_ON | 19200 | DMA_SOURCE_INCREMENT;
	} else {
		DMA[3].src = BUFFER1;
		DMA[3].dst = BUFFER0;
		DMA[3].cnt = DMA_ON | 19200 | DMA_SOURCE_INCREMENT;
	}
}

void waitForVblank()
{
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}

void waitForStarting(){
	for (u16 i = 0; i<15; i++) for (u16 j=0; j<24; j++) has[i][j] = 0;
	lives = 3;
	srand(seed);
	while(!KEY_DOWN_NOW(BUTTON_START));
	open = 0;
	//reset palette
	for(u16 i=0; i<55;i++)
	{
		PALETTE[i] = images_palette[i];
	}
	u16 colors[] = {BLACK, LTGRAY, RED, GREEN, BLUE, MAGENTA, CYAN, YELLOW, WHITE};
	for (u16 i=55; i<64; i++) {
		PALETTE[i] = colors[i-55];
	}
	clearScreen(BLACKIDX);
	FlipPage();
	clearScreen(BLACKIDX);
	drawGameScreen();
	FlipPage();
	copy();
	score = 0;
	redCurDirection = 10;
	gameOver = 0;
	seed++;
	win = 0;
}



int main() {
	REG_DISPCNT = MODE4 | DCNT_BG2;
	// FlipPage();
//	DEBUG_PRINT("start\n");

	// txt_init_std();
	drawStartScreen();
	waitForStarting();
//	u16 i=3;
	u16 counter = 1;
	u16 counter2 = 1;

//	DEBUG_PRINTF("1,1 3,9: %d\n", dijkstra(1,1,3,9));

	while (1){
		if (counter==4) counter = 0;
		if (counter2==2) counter2=0;
		if (counter==0) open = (!open);
		counter++;
		counter2++;
		
		if(KEY_DOWN_NOW(BUTTON_SELECT)) //reset to start screen
		{
			clearScreen(BLACKIDX);
			drawStartScreen();
			FlipPage();
			waitForStarting();
		}
		if(KEY_DOWN_NOW(BUTTON_UP))
		{
//			DEBUG_PRINT("up is pressed\n");
			if (canMove(&pacManPosition, UPWARD, map)) {
//				DEBUG_PRINT("up can move\n");
				pacManNextDirection = UP;
			}
		}
		if(KEY_DOWN_NOW(BUTTON_DOWN))
		{
//			DEBUG_PRINT("down is pressed\n");
			if (canMove(&pacManPosition, DOWNWARD, map)) pacManNextDirection = DOWN;
		}
		if(KEY_DOWN_NOW(BUTTON_LEFT))
		{
//			DEBUG_PRINT("left is pressed\n");
			if (canMove(&pacManPosition, LEFTWARD, map)) pacManNextDirection = LEFT;
		}
		if(KEY_DOWN_NOW(BUTTON_RIGHT))
		{
//			DEBUG_PRINT("right is pressed\n");
			if (canMove(&pacManPosition, RIGHTWARD, map)) pacManNextDirection = RIGHT;
		}	
		//update position
		calcRedNextDirection();
		calcNextCood(&redCurCood, &redNextCood, redCurDirection);
		if (counter2==2) {
			calcBlueNextDirection();
			calcNextCood(&blueCurCood, &blueNextCood, blueCurDirection);
		}
		calcPacManNextDirection();
		calcNextCood(&pacManCurCood, &pacManNextCood, pacManCurDirection);
		if (counter2==2)
			updateBlue();
		updateRed();
		updatePacMan();
		if (gameOver) {
			//go to game over screen
			drawGameOver();
			FlipPage();
			waitForStarting();
		}
		if (win) {
			drawWin();
			FlipPage();
			waitForStarting();
		}
		waitForVblank();
		FlipPage();
		copy();
	}
	return 0;
}
