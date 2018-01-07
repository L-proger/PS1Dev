#include <stdlib.h>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBETC.H>
#include <stdio.h>

#define PAL

#define ORDER_TABLE_CAPACITY 1 
#define MAXPACKETS 2 
#define SCREEN_WIDTH  320 

#ifdef PAL
#define	SCREEN_HEIGHT 240 
#else
#define	SCREEN_HEIGHT 256 
#endif

GsOT orderTable[2];
GsOT_TAG orderTableTags[2][1<<ORDER_TABLE_CAPACITY];
PACKET gpuPackets[2][MAXPACKETS];

u_long _ramsize   = 0x00200000;
u_long _stacksize = 0x00004000;

short currentFrameBuffer = 0;

void initGpu();
void renderFrame(); 

int main() {
	initGpu();
	FntLoad(960, 256);
	SetDumpFnt(FntOpen(0, 0, 320, 240, 0, 512)); 

	while (1){ //Render loop
		FntPrint("Test application!");
		renderFrame();
	}
}

void initGpu(){
#ifdef PAL
	SetVideoMode(1); 
	GsInitGraph(SCREEN_WIDTH, SCREEN_HEIGHT, GsINTER | GsOFSGPU, 1, 0);
#else
	SetVideoMode(0);
	GsInitGraph(SCREEN_WIDTH, SCREEN_HEIGHT, GsNONINTER | GsOFSGPU, 1, 0); 
#endif

	GsDefDispBuff(0, 0, 0, SCREEN_HEIGHT);
	
	orderTable[0].length = ORDER_TABLE_CAPACITY;
	orderTable[1].length = ORDER_TABLE_CAPACITY;
	orderTable[0].org = orderTableTags[0];
	orderTable[1].org = orderTableTags[1];
	
	GsClearOt(0, 0, &orderTable[0]);
	GsClearOt(0, 0, &orderTable[1]);
}


void renderFrame() {
	FntFlush(-1);

	currentFrameBuffer = GsGetActiveBuff();
	
	GsSetWorkBase((PACKET*)gpuPackets[currentFrameBuffer]);
	
	GsClearOt(0, 0, &orderTable[currentFrameBuffer]);
	
	DrawSync(0);
	
	VSync(0);
	GsSwapDispBuff();
	
	GsSortClear(0, 0, 255, &orderTable[currentFrameBuffer]);
	GsDrawOt(&orderTable[currentFrameBuffer]);
}