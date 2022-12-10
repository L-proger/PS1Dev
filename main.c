#include <stdlib.h>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBETC.H>
#include <stdio.h>
#include <stdint.h>

//VRAM size 1024,512

//#define PAL

#define ORDER_TABLE_SIZE 8
#define SCREEN_WIDTH  320

#ifdef PAL
	#define	SCREEN_HEIGHT 256
#else
	#define	SCREEN_HEIGHT 240
#endif

u_long _ramsize   = 0x00200000;
u_long _stacksize = 0x00004000;


typedef struct RenderFrame {
	DISPENV displayEnvironment;
	DRAWENV drawEnvironment;
	uint8_t primitivesBuffer[0xffff / 4];
	u_long orderTable[ORDER_TABLE_SIZE];
}RenderFrame;


struct RenderFrame _renderFrames[2];
int _currentRenderFrame = 0;

void initGpu();
void renderFrame();

extern void printDebugMessage();

int main() {
	initGpu();
	FntLoad(960, 256);
	SetDumpFnt(FntOpen(0, 0, 320, 240, 0, 512)); 

	printDebugMessage();

	while (1){ //Render loop
		renderFrame();
	}
}

void initGpu(){
	ResetCallback();
	ResetGraph(0);
	SetGraphDebug(0);
	InitGeom();
	//GsInit3D

	//Setup display environment
	SetDefDispEnv(&_renderFrames[0].displayEnvironment, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	SetDefDispEnv(&_renderFrames[1].displayEnvironment, 0, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT);

	//Set draw environments
	SetDefDrawEnv(&_renderFrames[0].drawEnvironment, 0, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT);
	SetDefDrawEnv(&_renderFrames[1].drawEnvironment, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// Set clear colors
	setRGB0(&_renderFrames[0].drawEnvironment, 63, 0, 127);
	setRGB0(&_renderFrames[1].drawEnvironment, 63, 127, 0);
	// Enable background clear
	_renderFrames[0].drawEnvironment.isbg = 1;
	_renderFrames[1].drawEnvironment.isbg = 1;


#ifdef PAL
	// TODO: why???
	// Screen offset to center the picture vertically
	_renderFrames[0].displayEnvironment.screen.y = 24;
	_renderFrames[1].displayEnvironment.screen.y = _renderFrames[0].displayEnvironment.screen.y;

	SetVideoMode(MODE_PAL);
	//GsInitGraph(SCREEN_WIDTH, SCREEN_HEIGHT, GsINTER | GsOFSGPU, 1, 0);
#else
	SetVideoMode(MODE_NTSC);
	//GsInitGraph(SCREEN_WIDTH, SCREEN_HEIGHT, GsNONINTER | GsOFSGPU, 1, 0);
#endif


	SetGeomOffset(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);	/* set geometry origin as (OFX, OFY) */
	SetGeomScreen(SCREEN_WIDTH / 2);		/* distance to viewing-screen */  // fov


	SetBackColor(0, 0, 0); /* set background(ambient) color*/
 	SetFarColor(0, 0, 0);
	SetFogNearFar(1500, 8000, SCREEN_WIDTH / 2);


	//GsDefDispBuff(0, 0, 0, SCREEN_HEIGHT);
//
	//orderTable[0].length = ORDER_TABLE_CAPACITY;
	//orderTable[1].length = ORDER_TABLE_CAPACITY;
	//orderTable[0].org = orderTableTags[0];
	//orderTable[1].org = orderTableTags[1];
//
	//GsClearOt(0, 0, &orderTable[0]);
	//GsClearOt(0, 0, &orderTable[1]);



	// Apply environments
	PutDispEnv(&_renderFrames[0].displayEnvironment);
	PutDrawEnv(&_renderFrames[0].drawEnvironment);

	// enable display
	SetDispMask(1);
}

int frameId = 0;

void renderFrame() {


//ClearOTagR()
	//currentFrameBuffer = GsGetActiveBuff();

	//GsSetWorkBase((PACKET*)gpuPackets[currentFrameBuffer]);

	//GsClearOt(0, 0, &orderTable[currentFrameBuffer]);


	RECT r;
	r.x = 0;
	r.y = 0;
	r.w = 100;
	r.h = 100;

	DrawSync(0); // Wait for GPU to finish drawing
	VSync(0);    // Wait for V-BLANK


	_currentRenderFrame ^= 1;

	//Swap buffers
	PutDispEnv(&_renderFrames[_currentRenderFrame].displayEnvironment);
	PutDrawEnv(&_renderFrames[_currentRenderFrame].drawEnvironment);


	u_long* orderTable = _renderFrames[_currentRenderFrame].orderTable;
	ClearOTagR(orderTable, ORDER_TABLE_SIZE);  // Clear ordering table

	uint8_t* primitiveBuffer = _renderFrames[_currentRenderFrame].primitivesBuffer;


	TILE* tile = (TILE*)primitiveBuffer;      // Cast next primitive

	setTile(tile);              // Initialize the primitive (very important)
	setXY0(tile, 32, 32);       // Set primitive (x,y) position
	setWH(tile, 64, 64);        // Set primitive size
	setRGB0(tile, 255, 255, 0); // Set color yellow
	addPrim(orderTable, tile);      // Add primitive to the ordering table

	primitiveBuffer += sizeof(TILE);    // Advance the next primitive pointer

  	DrawOTag(orderTable + ORDER_TABLE_SIZE - 1);   // Draw the ordering table


	
	//GsSwapDispBuff();
	//GsSortClear(0, 0, 255, &orderTable[currentFrameBuffer]);
	//GsDrawOt(&orderTable[currentFrameBuffer]);
	//ClearImage2(&r, 0xff, 0, 0);

	if((frameId / 50) % 2 == 0){
		FntPrint("EVEN");
	}else{
		FntPrint("ODD");
	}
	frameId++;

	FntFlush(-1);
}