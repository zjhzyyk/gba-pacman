#include <stdlib.h>
#include "start.h"
#include "images.h"

typedef unsigned short u16;
typedef u16 bool;
typedef unsigned int u32;
typedef unsigned char u8;
typedef u16 COLOR;
typedef struct tagTXT_BASE
{
    u16 *dst0;      // writing buffer starting point
    u32 *font;      // pointer to font used
     u8 *chars;     // character map (chars as in letters, not tiles)
     u8 *cws;       // char widths (for VWF)
     u8  dx,dy;     // letter distances
    u16  flags;     // for later
     u8  extra[12]; // ditto
} TXT_BASE;
typedef struct {
    u16 x, y, hasDot;
} point;
typedef struct 
{
    const volatile void *src;
    volatile void *dst;
    volatile u32 cnt;
} DMAREC;

#define BUTTON_A      (1<<0)
#define BUTTON_B      (1<<1)
#define BUTTON_SELECT (1<<2)
#define BUTTON_START  (1<<3)
#define BUTTON_RIGHT  (1<<4)
#define BUTTON_LEFT   (1<<5)
#define BUTTON_UP     (1<<6)
#define BUTTON_DOWN   (1<<7)
#define BUTTON_R      (1<<8)
#define BUTTON_L      (1<<9)
#define BUTTONS (*( volatile unsigned int *)0x04000130)
#define KEY_DOWN_NOW(key)  (~(BUTTONS) & key)
#define REG_DISPCNT *((u16*)(0x04000000))
#define DCNT_MODE3 0x0003
#define MODE4 4
#define BUFFER0 (u16 *)0x6000000
#define BUFFER1 (u16 *)0x600A000
#define BUFFER1FLAG (1<<4)
#define PALETTE ((u16 *)0x5000000)
#define DCNT_BG2 0x0400
// #define DMA_ON (1 << 31)
#define COLOR(r,g,b) ((r) | (g) << 5 | (b) << 10)
#define SCANLINECOUNTER (*(volatile unsigned short *)0x4000006)
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define STOP 10
#define KEEP 20
#define COODX(x) (15+10*(x))
#define COODY(y) (5+10*(y))
#define UPWARD -1,0
#define DOWNWARD 1,0
#define LEFTWARD 0,-1
#define RIGHTWARD 0,1
#define killRadius 10
#define DISTMAX 1000
#define DMA ((volatile DMAREC *)0x040000B0)
#define OFFSET(r,c) ((r)*240+(c))
#define DMA_ON (1 << 31)
#define DMA_SOURCE_FIXED (2 << 23)
#define DMA_SOURCE_INCREMENT (0 << 23)
#define YELLOW COLOR(31, 31, 0)
#define RED COLOR(31,0,0)
#define WHITE COLOR(31,31,31)
#define BLACK 0
#define GREEN COLOR(0,31,0)
#define BLUE COLOR(0,0,31)
#define CYAN COLOR(0,31,31)
#define MAGENTA COLOR(31,0,31)
#define YELLOW COLOR(31, 31, 0)
#define LTGRAY COLOR(25, 25, 25)
#define BLACKIDX 55
#define YELLOWIDX 62
#define MAGENTAIDX 60
#define BLUEIDX 59

// #define MEM_VRAM 0x06000000
// #define vid_mem ((u16*)MEM_VRAM)

extern const unsigned int toncfontTiles[192];
extern TXT_BASE __txt_base, *gptxt;
extern const char* map[15];
extern unsigned short *videoBuffer;
extern u16 maxDotNum;
extern point ptArr[365];
extern u16 dotNum;
extern u16 has[15][23];
extern const u16* pac[4][2];
extern point pacManPosition, redPosition, yellowPosition, pinkPosition, bluePosition, blueCurCood, blueNextCood,
redCurCood, redNextCood, pacManCurCood, pacManNextCood, redPrePosition, bluePrePosition, preUseless;
extern u16 preDirection;
extern u16 pacManCurDirection;
extern u16 lives;
extern u16 redCurDirection;
extern u16 blueCurDirection;
extern u16 stackSize;
extern u16 gameOver;
extern u16 stack[10];
extern short score;
extern short pacDotScore;
extern u16 pacManNextDirection;
extern u16 open;
extern u16 win;
extern const unsigned char fontdata_6x8[12288];

void heap_up(u16 id);
void heap_down(u16 id);
u16 heap_pop();
void heap_push(u16 x);
u32 dijkstra(u16 sx, u16 sy, u16 ex, u16 ey);
void txt_init_std();
void bm16_puts(u16 *dst, const char *str, COLOR clr, int pitch);
void drawRect(u16 row, u16 col, u16 height, u16 width, u8);
// void drawRect(int row, int col, int height, int width, u8 index);
void drawDiscription();
void putDots(u16, u16);
void drawDot(point* pos);
void drawDots(u16);
void initPacMan();
void drawGameScreen();
void drawStartScreen();
void clearStartScreen();
// void clearScreen(u8);
void clearScreen(u8 index);
void drawGameOver();
void drawWin();
void writeNum(short s, u16 row, u16 col, u16 width, u16 height, u8 ci);
u16 canMove(point* curPosition, short x, short y, const char* mp[]);
void calcRedNextDirection();
void calcNextCood(point* curCood, point* nextCood, u16 direction);
void calcBlueNextDirection();
void calcPacManNextDirection();
void updateRed();
void updateBlue();
void updatePacMan();
u16 canMove2(point* curPosition, u16 direction, const char* mp[]);
void write(int x, int y, const char *str, u8 clrid);
void restoreDot(point* curCood, point* prePosit, u16 curDir);
void updatePosition(point* curCood, point* curPosition, point* prePos);
u16 killPacMan(point* ghostCood);
void drawImage(u16 row, u16 col, u16 width, u16 height, const u16 img[]);
void FlipPage();
void drawString4(int row, int col, char *str, u8 index);
void setPixel4(int row, int col, u8 index);