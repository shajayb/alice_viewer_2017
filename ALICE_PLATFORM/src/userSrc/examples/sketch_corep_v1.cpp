#define _MAIN_
#define _ALG_LIB_


#ifdef _MAIN_

#include "main.h"
#include "Matrix3x3.h"
#include "MODEL.h"
#include "interpolate.h"
#include "metaMesh.h"
#include "rigidCube.h"
#include "RenderMesh.h"
#include "skeletonMesh.h"

#include <array>
#include <memory>
#include<time.h>
#include<experimental/generator> 
#include<experimental/generator> 
using namespace std;
using namespace std::experimental;


//////////////////////////////////////////////////////////////////////////


#define rx ofRandom(-1,1)




//////////////////////////////////////////////////////////////////////////

RenderMesh RM;
double lightScale = 52;
bool flipNormals = true;
bool drawFaces = false;
bool drawWire = false;
bool drawEdges = true;


//////////////////////////////////////////////////////////////////////////

//metaMesh MM;
double threshold = 1.0;

Graph G;
Mesh M;
double Eoff = 0.2;
double Width = 0.2;
bool edges = true;
bool nodes = true;

SkeletonMesh skeleton;
bool removeFaces = false;
//////////////////////////////////////////////////////////////////////////


void setup()
{
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//////////////////////////////////////////////////////////////////////////
	
	skeleton = *new SkeletonMesh("data/comp_net_out.txt");

	skeleton.meshFromGraph();
	RM.addMesh(skeleton.combinedMesh);

	//////////////////////////////////////////////////////////////////////////

	S = *new SliderGroup(vec(50, 50, 0));
	S.addSlider(&threshold, "threshold");
	S.sliders[S.numSliders - 1].minVal = -5;
	S.sliders[S.numSliders-1].maxVal = 5;

	S.addSlider();
	S.addSlider();
	S.addSlider(&Eoff, "edgeOffset");
	S.sliders[S.numSliders - 1].maxVal = 5;
	S.addSlider(&Width, "width");
	S.sliders[S.numSliders - 1].maxVal = 5;

	S.addSlider(&lightScale, "lightScale");
	S.sliders[S.numSliders - 1].maxVal = 1500;

	
	B = *new ButtonGroup(vec(50, 500, 0));
	B.addButton(&flipNormals, "flipNormals");
	B.addButton(&drawFaces, "drawFaces");
	B.addButton(&drawWire, "drawWire");
	B.addButton(&drawEdges, "drawEdges");
	B.addButton(&edges, "edges");
	B.addButton(&nodes, "nodes");
	B.addButton(&removeFaces, "removeFaces");
}

void update(int value)
{

}

void draw()
{

	backGround(0.75);
	//drawGrid(20);

	glPushMatrix();
	//glScalef(5, 5, 5);

	glColor3f(0, 0, 0);
	skeleton.combinedMesh.drawIsoContoursInRange(threshold, 0.3);
	skeleton.draw();
	skeleton.G.draw();

	

	////
	int cur_msx = winW * 0.5;
	int cur_msy = winH * 0.5;
	vec camPt = screenToCamera(cur_msx, cur_msy, 0.2);

	RM.updateColorArray(lightScale, flipNormals , camPt );
 	RM.draw(drawFaces, drawWire, drawEdges);
	
	//glPopMatrix();

}

///////////////////////////////////////////////////////////////////////////////////////////////

void mousePress(int b, int state, int x, int y)
{
	if (HUDSelectOn)
	{

		keyPress('s', 0, 0);
		keyPress('S', 0, 0);
	}

}

void mouseMotion(int x, int y)
{
	if (HUDSelectOn)
	{

		keyPress('s', 0, 0);
		keyPress('S', 0, 0);
	}
}

void keyPress(unsigned char k, int xm, int ym)
{



	if (k == 's')
	{
		
		skeleton.meshFromGraph( Eoff, Width,edges,nodes, removeFaces);
	}
	if (k == 'S')
	{
		double mn, mx;
		skeleton.combinedMesh.getMinMaxOfScalarField(mn, mx);
		S.sliders[0].maxVal = mx * 1.5;
		S.sliders[0].minVal = mn;
		skeleton.combinedMesh.createIsoContourGraph(threshold);

		RM.reset();
		RM.addMesh(skeleton.combinedMesh);
	}

	if (k == 'w')
	{
		skeleton.combinedMesh.writeOBJ("data/convex.obj", "", skeleton.combinedMesh.positions, false);
		//M.writeOBJ("data/convex_tmp.obj", "", M.positions, false);
	}
}





#endif // _MAIN_
