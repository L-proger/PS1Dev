#define _WCHAR_T
#include <stdlib.h>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBETC.H>
#include <stdio.h>
#include <stdint.h>

#include "cube.h"
#include "config.h"

#include <array>
#include <new>
#include <limits>
#include <type_traits>

u_long _ramsize   = 0x00200000;
u_long _stacksize = 0x00004000;


struct RenderFrame {
	DISPENV displayEnvironment;
	DRAWENV drawEnvironment;
	uint8_t primitivesBuffer[0xffff / 4];
	u_long orderTable[ORDER_TABLE_SIZE];
};


struct RenderFrame _renderFrames[2];
int _currentRenderFrame = 0;


int tim_mode;
RECT tim_prect,tim_crect;
int tim_uoffs,tim_voffs;

void initGpu();
void renderFrame();

extern u_long Texture_Test_64x64[];
extern const int Texture_Test_64x64_Size;

Cube cube;

int testGlobalConstants = 7;

extern "C" {
	int main() {
		printf("Main: %d\n", testGlobalConstants);
		initGpu();
		FntLoad(960, 256);
		SetDumpFnt(FntOpen(0, 0, 320, 240, 0, 512)); 

		while (1){ //Render loop
			renderFrame();
		}
	}
}

void loadTexture(u_long *tim, TIM_IMAGE *tparam) {

    // Read TIM information (PsyQ)
    OpenTIM(tim);
    ReadTIM(tparam);

    // Upload pixel data to framebuffer
    LoadImage(tparam->prect, (u_long*)tparam->paddr);
    DrawSync(0);

    // Upload CLUT to framebuffer if present
    if( tparam->mode & 0x8 ) {
        LoadImage(tparam->crect, (u_long*)tparam->caddr);
        DrawSync(0);
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
	setRGB0(&_renderFrames[0].drawEnvironment, 32, 32, 40);
	setRGB0(&_renderFrames[1].drawEnvironment, 32, 32, 40);
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


	TIM_IMAGE texture;
	loadTexture(Texture_Test_64x64, &texture);
    //Copy the TIM coordinates
    tim_prect   = *texture.prect;
    tim_crect   = *texture.crect;
    tim_mode    = texture.mode;

	tim_uoffs = (tim_prect.x%64)<<(2-(tim_mode&0x3));
	tim_voffs = (tim_prect.y&0xff);

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


	//ONE

	SPRT* sprt = (SPRT*)primitiveBuffer;
	setSprt(sprt);
	setXY0(sprt, 48, 48);
	setWH(sprt, 64, 64);
	setUV0(sprt,                    // Set UV coordinates
            tim_uoffs, 
            tim_voffs);
	setClut(sprt,               // Set CLUT coordinates from TIM to sprite
		tim_crect.x,
		tim_crect.y);
	setRGB0(sprt,               // Set color of sprite, 128 is neutral
		128, 128, 128);
	addPrim(orderTable, sprt);
	primitiveBuffer += sizeof(SPRT);

	//Bind texture
	DR_TPAGE* tpage = (DR_TPAGE*)primitiveBuffer;
	setDrawTPage(tpage, 0, 1, getTPage(tim_mode & 0x3, 0, tim_prect.x, tim_prect.y) );
	addPrim(orderTable, tpage);         // Sort primitive to OT
	primitiveBuffer += sizeof(DR_TPAGE);    // Advance next primitive address

	//Flat color tile
	TILE* tile = (TILE*)primitiveBuffer;      // Cast next primitive
	setTile(tile);              // Initialize the primitive (very important)
	setXY0(tile, 32, 32);       // Set primitive (x,y) position
	setWH(tile, 64, 64);        // Set primitive size
	setRGB0(tile, 255, 255, 0); // Set color yellow
	addPrim(orderTable, tile);      // Add primitive to the ordering table
	primitiveBuffer += sizeof(TILE);    // Advance the next primitive pointer


	cube.render(orderTable);
	cube._rotation.vx += 10;
	cube._rotation.vz += 8;

  	DrawOTag(orderTable + ORDER_TABLE_SIZE - 1);   // Draw the ordering table


	//GsSwapDispBuff();
	//GsSortClear(0, 0, 255, &orderTable[currentFrameBuffer]);
	//GsDrawOt(&orderTable[currentFrameBuffer]);
	//ClearImage2(&r, 0xff, 0, 0);

	if((frameId / 50) % 2 == 0){
		if(Texture_Test_64x64_Size != 0){
			FntPrint("OMG!123456" );
		}
		FntPrint("EVEN" );
	}else{
		FntPrint("ODD");
	}
	frameId++;

	FntFlush(-1);
}