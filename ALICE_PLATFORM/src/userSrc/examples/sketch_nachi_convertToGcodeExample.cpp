

#ifdef _MAIN_

#include "main.h"
#include "nachi.h"
#include "RenderMesh.h"


//////////////////////////////////////				INCLUDES				//////////////////////////////////////




//////////////////////////////////////	OTHER DEFINITIONS / GLOBAL VARIABLES	//////////////////////////////////////



pathImporter path;
char s[200];
char t[200];
char jts[400];

RenderMesh RM;
double lightScale = 300.0;
bool flipNormals = false;
bool drawFaces = false;
bool drawWire = false;
bool drawEdges = true;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////				MVC APPLICATION				//////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
///// ---------------------------------------------------------- MODEL ------------------------------


void setup()
{
	path.readPath("data/path_1.txt", ",", 2.0);



	S = *new SliderGroup();
	S.addSlider(&path.Nachi.rot[0], "J1");
	S.addSlider(&path.Nachi.rot[1], "J2");
	S.addSlider(&path.Nachi.rot[2], "J3");
	S.addSlider(&path.Nachi.rot[3], "J4");
	S.addSlider(&path.Nachi.rot[4], "J5");
	S.addSlider(&path.Nachi.rot[5], "J6");

	S.sliders[0].attachToVariable(&path.Nachi.rot[0], -170, 170);
	S.sliders[1].attachToVariable(&path.Nachi.rot[1], -170, 170);
	S.sliders[2].attachToVariable(&path.Nachi.rot[2], -170, 170);
	S.sliders[3].attachToVariable(&path.Nachi.rot[3], -170, 170);
	S.sliders[4].attachToVariable(&path.Nachi.rot[4], -170, 170);
	S.sliders[5].attachToVariable(&path.Nachi.rot[5], -170, 170);


	S.addSlider(&lightScale, "lightScale");
	S.sliders[S.numSliders - 1].maxVal = 1500;

	B.addButton(&flipNormals, "flipNormals");
	B.addButton(&drawFaces, "drawFaces");
	B.addButton(&drawWire, "drawWire");
	B.addButton(&drawEdges, "drawEdges");
}

void update(int value)
{
	path.Nachi.ForwardKineMatics(path.Nachi.rot);


	{
		RM.reset();
		path.transformRobotMeshes();

		//RM.addMesh(path.Nachi.base);
		
		for (int i = 0; i < 5; i++)
		{
			RM.addMesh(path.Nachi.link_meshes[i]);
		}
		RM.addMesh(path.E.M);

		path.invertTransformRobotMeshes();
	}



	

}


///// ---------------------------------------------------------- VIEW  ------------------------------

void draw()
{


	backGround(0.75);
	drawGrid(20);

	path.draw(false,false);
	RM.draw(drawFaces, drawWire, drawEdges);

	sprintf_s(s, " current point id : %i", path.currentPointId);
	sprintf_s(t, " total points in path : %i", path.actualPathLength - 1);
	int cid = path.currentPointId;

	setup2d();

	drawString(s, winW * 0.5, winH - 50);
	drawString(t, winW * 0.5, winH - 75);
	drawString(jts, winW * 0.5, winH - 100);

	restore3d();

}


///// ---------------------------------------------------------- CONTROLLER ------------------------------


void mousePress(int b, int state, int x, int y)
{

}

void mouseMotion(int x, int y)
{

	{
		int cur_msx = winW * 0.5;
		int cur_msy = winH * 0.5;
		vec camPt = screenToCamera(cur_msx, cur_msy, 0.2);

		RM.updateColorArray(lightScale, flipNormals, camPt);
	}
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
	


	if (k == 'd')
	{
		
		for ( int i = 0 ; i < 6 ; i++)
		{
			//for ( int j = 0 ; j < 4 ; j++)
			{
				path.Nachi.Bars[i].print();

			}
		}
		
		cout << path.Nachi.Bars[5].d << endl;
	}
}





#endif // _MAIN_
