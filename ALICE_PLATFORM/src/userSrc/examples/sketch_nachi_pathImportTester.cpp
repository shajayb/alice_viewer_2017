

#ifdef _MAIN_

#include "main.h"
#include "nachi.h"
#include "RenderMesh.h"


//////////////////////////////////////				INCLUDES				//////////////////////////////////////




//////////////////////////////////////	OTHER DEFINITIONS / GLOBAL VARIABLES	//////////////////////////////////////


pathImporter path;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////				MVC APPLICATION				//////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
///// ---------------------------------------------------------- MODEL ------------------------------

void setup()
{
	
	//path = *new pathImporter("data/ee.obj");
	path.readPath("data/path_1.txt", ",", 0.0);
	
}

void update(int value)
{

}


///// ---------------------------------------------------------- VIEW  ------------------------------

void draw()
{


	backGround(0.75);
	drawGrid(20);

	path.draw();

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
	if (k == 'n')path.goToNextPoint();
	if (k == 'b')
	{
		path.currentPointId -= 2;;
		path.goToNextPoint();
	}
	if (k == 'N')path.currentPointId = 0;
	if (k == 'q')path.checkPathForReachability();
	if (k == 'w')path.exportGCode();


	
}





#endif // _MAIN_
