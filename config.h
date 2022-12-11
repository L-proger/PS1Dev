#pragma once

//VRAM size 1024,512

//#define PAL

#define ORDER_TABLE_SIZE 4096
#define SCREEN_WIDTH  320

#ifdef PAL
	#define	SCREEN_HEIGHT 256
#else
	#define	SCREEN_HEIGHT 240
#endif