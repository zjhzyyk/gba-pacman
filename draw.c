// #include "draw.h"
#include "header.h"

point ptArr[365];
u16 has[15][23];
u16 maxDotNum;
u16 dotNum;
u16 preDirection = RIGHT;
u16 pacManCurDirection = STOP;

const u16* pac[4][2]={
		{up_close, up_open},
		{down_close, down_open},
		{left_close, left_open},
		{right_close, right_open}
};
point pacManPosition, redPosition, yellowPosition, pinkPosition, bluePosition, blueCurCood, blueNextCood,
redCurCood, redNextCood, pacManCurCood, pacManNextCood, redPrePosition, bluePrePosition, preUseless;

unsigned short *videoBuffer = (u16 *)0x6000000;

void setPixel4(int row, int col, u8 index)
{
	int pixel = row*240+col;
	int whichShort = pixel/2;
	if(col&1)
	{
		// Odd column must insert index into left side
		videoBuffer[whichShort] = (videoBuffer[whichShort] & 0x00FF) | (index<<8);
	}
	else
	{
		// Even column
		videoBuffer[whichShort] = (videoBuffer[whichShort] & 0xFF00) | index;
	}
}

void FlipPage()
{
	if(REG_DISPCNT & BUFFER1FLAG)
	{
		REG_DISPCNT = REG_DISPCNT & ~BUFFER1FLAG;
		videoBuffer = BUFFER1;
	}
	else
	{
		REG_DISPCNT = REG_DISPCNT | BUFFER1FLAG;
		videoBuffer = BUFFER0;
	}
}

void drawRect(u16 row, u16 col, u16 height, u16 width, u8 index)
{
	u16 r,c;
	for(r=0; r<height; r++)
	{
		for(c=0; c<width; c++)
		{
			setPixel4(row+r, col+c, index);
		}
	}
}


void writeNum(short s, u16 row, u16 col, u16 width, u16 height, u8 ci){
	// FlipPage();
	drawRect(row, col, height, width, BLACKIDX);
	// FlipPage();
	// drawRect(row, col, height, width, BLACKIDX);
	char num[4] ="000\0";
	short i = 2;
	while (s && i>=0) {
		num[i--] = (s%10+'0');
		s/=10;
	}
	// FlipPage();
	// drawString4(row, col, num, ci);
	// FlipPage();
	drawString4(row, col, num, ci);
}

void drawImage(u16 row, u16 col, u16 width, u16 height, const u16 img[]) {
	for(u16 r=0; r<height; r++)
	{
		DMA[3].src = img+r*width/2;
		DMA[3].dst = videoBuffer + OFFSET(row+r, col)/2;
		DMA[3].cnt = DMA_ON | width/2;	
	}
}

void drawMap(u16 yOffset, const char* mp[]){
	// for (u16 i = 0; i<15; i++) {
	// 	DEBUG_PRINTF("%s\n", mp[i]);
	// }
	maxDotNum = 0;
	u16 wallWidth = 4;
	u16 xOffset;
	u16 hOffset;
	u16 wOffset;
	u16 tmp = yOffset;
	for (u16 i=0; i<15; i++) {
		for (u16 j = 0; j<24; j++) {
			xOffset = 0;
			yOffset = tmp;
			wOffset = 0;
			hOffset = 0;
			if (mp[i][j]=='#') {
				if (i!=0 && j!=0 && mp[i][j-1]=='#' && mp[i-1][j]=='#' && mp[i-1][j-1]=='#') {
					drawRect(yOffset-10+10*i+5-wallWidth/2, 10*j+5-(wallWidth/2)-10, 
						10+wallWidth, 10+wallWidth, BLUEIDX);
				}
				else {
					if (j!=0 && mp[i][j-1]=='#') {
					wOffset = xOffset = 10-wallWidth;
					}
					if (i!=0 && mp[i-1][j]=='#') {
						hOffset = 10-wallWidth;
						yOffset -= hOffset;
					}
					drawRect(yOffset+10*i+5-wallWidth/2, 10*j+5-(wallWidth/2), hOffset+wallWidth, wallWidth, BLUEIDX);
					drawRect(tmp+10*i+5-wallWidth/2, 10*j+5-(wallWidth/2)-xOffset, wallWidth, wOffset+wallWidth, BLUEIDX);
				}
			}
			else {
				ptArr[maxDotNum].x = i;
				ptArr[maxDotNum].y = j;
				ptArr[maxDotNum].hasDot = 0;
				// stack[stackSize++] = maxDotNum;
				maxDotNum++;
			}
		}
	}
}

//draw discription information such as lives and score
void drawDiscription(){
	drawString4( 1,  1, "SCORE:", YELLOWIDX);
	writeNum(0, 1, 55, 35, 10, YELLOWIDX);
	drawRect(1, 90, 10, 10, BLACKIDX);
	drawString4( 1, 90, "LIVES:", YELLOWIDX);
	writeNum(3, 1, 140, 35, 10, YELLOWIDX);
	drawRect(1, 175, 10, 10, BLACKIDX);
}

void putDots(u16 quickVersion, u16 possibility){
	dotNum = 0;
	if (quickVersion) {
		u16 r;
		r = rand()%maxDotNum;
		//only one dot will be put for testing.
		ptArr[r].hasDot = 1;
		has[ptArr[r].x][ptArr[r].y] = 1;
		return;
	}
	for (u16 i = 1;i<maxDotNum-3; i++) { //reserve the first and last three positions for pacman and ghosts.
		if ((rand() % 100)<possibility) {
			ptArr[i].hasDot = 1;
			has[ptArr[i].x][ptArr[i].y]=1;
			dotNum++;
		}
	}
}

u16 dotWidth = 2;

void drawDot(point* pos){
	if (has[pos->x][pos->y])
		drawRect(COODX(pos->x)-dotWidth/2, COODY(pos->y)-dotWidth/2, dotWidth, dotWidth, MAGENTAIDX);
}

void drawDots(u16 yOffset){
	for (u16 i = 0; i<maxDotNum; i++) {
		if (ptArr[i].hasDot){
			drawRect(5+yOffset+10*ptArr[i].x-dotWidth/2, 5+10*ptArr[i].y-dotWidth/2, dotWidth, dotWidth, MAGENTAIDX);
		}
	}
}

void initPacMan(){
	// FlipPage();
	// drawImage(15+10*ptArr[0].x-7, 10*ptArr[0].y-2, 14,14, pac[3][0]);
	// FlipPage();
	drawImage(15+10*ptArr[0].x-7, 10*ptArr[0].y-2, 14,14, pac[3][0]);
	pacManPosition.x = ptArr[0].x;
	pacManCurCood.x = COODX(pacManPosition.x);
	pacManPosition.y = ptArr[0].y;
	pacManCurCood.y = COODY(pacManPosition.y);
	preDirection = RIGHT;
	pacManCurDirection = STOP;
	pacManNextCood = pacManCurCood;
}

void drawGameScreen(){
	// FlipPage();
	// drawDiscription();
	// drawMap(10, map);
	// FlipPage();
	drawDiscription();
	drawMap(10, map);
	putDots(0, 60);
	// FlipPage();
	// drawDots(10);
	// FlipPage();
	drawDots(10);
	//draw pacman in the first available position
//	backup(15+10*ptArr[0].x-7, 1+10*ptArr[0].y, 13,13,pacmanBackup);
	initPacMan();
	//draw red ghost in the last available position
//	backup(15+10*ptArr[maxDotNum-1].x-7, 10*ptArr[maxDotNum-1].y-2, 14,14,redBackup);
	drawImage(15+10*ptArr[maxDotNum-1].x-7, 10*ptArr[maxDotNum-1].y-2, 14,14,red);
	redPosition.x = ptArr[maxDotNum-1].x;
	redCurCood.x= COODX(redPosition.x);
	redPosition.y =ptArr[maxDotNum-1].y;
	redCurCood.y= COODY(redPosition.y);
	redPrePosition = redPosition;
	//draw blue ghost one position before red guy
	// FlipPage();
	// drawImage(15+10*ptArr[maxDotNum-2].x-7, 10*ptArr[maxDotNum-2].y-2, 14,14,blue);
	// FlipPage();
	drawImage(15+10*ptArr[maxDotNum-2].x-7, 10*ptArr[maxDotNum-2].y-2, 14,14,blue);
	bluePosition.x = ptArr[maxDotNum-2].x;
	blueCurCood.x= COODX(bluePosition.x);
	bluePosition.y = ptArr[maxDotNum-2].y;
	blueCurCood.y= COODY(bluePosition.y);
	//draw yellow ghost one position before blue
	// drawImage(15+10*ptArr[maxDotNum-3].x-7, 1+10*ptArr[maxDotNum-3].y, 14,14,yellow);
	// yellowPosition.x = ptArr[maxDotNum-3].x;
	// yellowPosition.y = ptArr[maxDotNum-3].y;

}

void drawStartScreen(){
	for(u16 i=0; i<255;i++)
	{
		PALETTE[i] = start_palette[i];
	}
	// drawRect(70, 100, 15, 45, BLUE);
	// write( 103,  75, "START", YELLOW);
	drawImage(0,0,240,160,start);
}

void clearStartScreen(){
	// write( 103,  75, "START", BLACKIDX);
	// drawRect(70, 100, 15, 45, BLACKIDX);
	clearScreen(BLACKIDX);
}

void clearScreen(u8 index)
{
	volatile u16 color = (index<<8) | index;
	DMA[3].src = &color;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = DMA_ON | DMA_SOURCE_FIXED | 19200;	
}

void drawGameOver(){
	// clearScreen(BLACK);
	// drawRect(70, 90, 15, 75, BLUE);
	// write( 93,  75, "Game Over", YELLOW);
	drawImage(0,0,240,160,gameover);
}

void drawWin(){
	// clearScreen(BLACK);
	// drawRect(70, 100, 15, 45, BLUE);
	// write( 103,  75, "Win", YELLOW);
	drawImage(0,0,240,160,winImage);
}