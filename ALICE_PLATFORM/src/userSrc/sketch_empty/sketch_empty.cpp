#define _MAIN_

#ifdef _MAIN_

#include "main.h"
//////////////////////////////////////				INCLUDES				//////////////////////////////////////




//////////////////////////////////////				OTHER DEFINITIONS / GLOBAL VARIABLES				//////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////				MVC APPLICATION				//////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
///// ---------------------------------------------------------- MODEL ------------------------------



void setup()
{



}

void update(int value)
{

}


///// ---------------------------------------------------------- VIEW  ------------------------------

void draw()
{

	backGround(0.9);
	drawGrid(20);

	wireFrameOn();
	
		drawCube(vec(-1, -1, -1), vec(1, 1, 1));

	wireFrameOff();

}


///// ---------------------------------------------------------- CONTROLLER ------------------------------


void mousePress(int b, int state, int x, int y)
{

}

void mouseMotion(int x, int y)
{

}

void keyPress(unsigned char k, int xm, int ym)
{




}





#endif // _MAIN_
