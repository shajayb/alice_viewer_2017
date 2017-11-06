#define _MAIN_

#ifdef _MAIN_

#include "main.h"
#include "RenderMesh.h"

//////////////////////////////////////				INCLUDES				//////////////////////////////////////




//////////////////////////////////////				OTHER DEFINITIONS / GLOBAL VARIABLES				//////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////				MVC APPLICATION				//////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
///// ---------------------------------------------------------- MODEL ------------------------------

RenderMesh RM;
double lightScale = 1.0;
bool flipNormals = true;

void setup()
{
	Mesh M;
	MeshFactory fac;
	for ( int i = 0 ; i < 6 ; i ++)
	{
		string file = "data/link";
		
		char s[200];
		itoa(i, s, 10);
		file += s;
		file += ".obj";
		cout << file.c_str() << endl;
		M = fac.createFromOBJ( file, 1.0, false);
		RM.addMesh(M);
	}
	
	S.addSlider(&lightScale, "lightScale");
	S.sliders[S.numSliders - 1].maxVal = 1500;

	B.addButton(&flipNormals, "flipNormals");

}

void update(int value)
{

}


///// ---------------------------------------------------------- VIEW  ------------------------------

void draw()
{

	backGround(0.9);
	drawGrid(20);

	////
	int cur_msx = winW * 0.5;
	int cur_msy = winH * 0.5;
	vec camPt = screenToCamera(cur_msx, cur_msy, 0.2);

	//cout << lightScale << endl;
	//cout << flipNormals << endl;
	//camPt.print();


	RM.updateColorArray(lightScale, flipNormals, camPt);

	RM.draw(false);

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
