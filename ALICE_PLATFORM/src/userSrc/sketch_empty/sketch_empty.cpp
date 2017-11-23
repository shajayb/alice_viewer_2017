//#define _MAIN_



#ifdef _MAIN_

//////////////////////////////////////				INCLUDES				//////////////////////////////////////

#include "main.h"
#include "newPhysics.h"


//////////////////////////////////////	OTHER DEFINITIONS / GLOBAL VARIABLES	//////////////////////////////////////


//////////////////////////////////////	OTHER DEFINITIONS / GLOBAL VARIABLES	//////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////				MVC APPLICATION				//////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
///// ---------------------------------------------------------- MODEL ------------------------------

Mesh M;



void setup()
{

	MeshFactory fac;
	M = fac.createPrism(3, 1, 0, false);
	
	for (int i = 0; i < 3; i++)M.positions[i + 3].z = 1.0;
	
	vec min, max;
	M.boundingBox(min, max);
	for (int i = 0; i < M.n_v; i++)M.positions[i] -= (min + max)*0.5;
}



void update(int value)
{


}

void draw()
{


	backGround(0.75);
	drawGrid(20.0);

	M.draw(true);

	drawCircle( vec(0, 0, 0), 1.0, 32 );

}

void keyPress(unsigned char k, int xm, int ym)
{

}

void mousePress(int b, int s, int x, int y)
{
}

void mouseMotion(int x, int y)
{

	
}



#endif // _MAIN_
