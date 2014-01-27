// #include "dijkstra.h"
#include "header.h"

u16 dist[365];
u16 heap[1000];
u16 heapSize = 0;

u16 calc(u16 x, u16 y) {
	return 24*x+y;
}

void heap_up(u16 id){
//	DEBUG_PRINT("in heap up\n");
	u16 fa = id>>1;
	u16 tmp = heap[id];
	while (id != 1 && dist[tmp]<dist[heap[fa]]) {
		heap[id] = heap[fa];
		id = fa;
		fa = id >> 1;
	}
	heap[id] = tmp;
}

void heap_down(u16 id){
//	DEBUG_PRINT("IN HEAP DOWN\n");
	u16 pos, tmp = heap[id], r;
	while ((pos = id<<1) <= heapSize) { //in heap, size should be one less
		//than the actual size since index starts from 1.
		r = id<<1|1;
		if (r <= heapSize && dist[heap[r]]<dist[heap[pos]])
			pos = r;
		if (dist[tmp] > dist[heap[pos]]) {
			heap[id] = heap[pos];
		} else
		break;
		id = pos;
	}
	heap[id] = tmp;
}

u16 heap_pop(){
	u16 tmp = heap[1];
	heap[1] = heap[heapSize--];
	heap_down(1);
	return tmp;
}

void heap_push(u16 x){
	heap[++heapSize] = x;
}

u32 dijkstra(u16 sx, u16 sy, u16 ex, u16 ey) { //start is the index in ptArr
//	DEBUG_PRINT("in dijkstra\n");
	heapSize = 0; //clear heap
	for (u16 i = 0; i<365; i++) dist[i] = DISTMAX;
	heap_push(calc(sx, sy));
	dist[calc(sx, sy)]=0;
	u16 tmp;
	point cur;
	while (heapSize>0) {
		tmp = heap_pop();
		cur.x = tmp/24;
		cur.y = tmp%24;
		if (cur.x==ex && cur.y==ey) break;
		for (u16 i=0; i<4; i++) {
			if (canMove2(&cur, i, map)) {
				short x=0, y=0;
				switch (i) {
				case RIGHT : x=0;y=1;break;
				case LEFT: x=0;y=-1;break;
				case UP: x=-1;y=0;break;
				case DOWN: x=1;y=0;break;
				case STOP: x=0;y=0;break;
				}
				if (dist[tmp]+1 < dist[calc(cur.x+x,cur.y+y)]) {
					dist[calc(cur.x+x,cur.y+y)] = dist[tmp]+1;
					heap_push(calc(cur.x+x, cur.y+y));
				}
			}
		}
	}
	return dist[calc(ex,ey)];
}