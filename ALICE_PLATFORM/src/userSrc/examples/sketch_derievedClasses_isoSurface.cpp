#define _MAIN_

#ifdef _MAIN_

//////////////////////////////////////				INCLUDES				//////////////////////////////////////

#include "main.h"
#include "nachi.h"


//////////////////////////////////////	OTHER DEFINITIONS / GLOBAL VARIABLES	//////////////////////////////////////

class nodalSurf : public isoSurf
{
public:
	double a1, a2, a3, a4, a5, a6;

	nodalSurf() {};
	nodalSurf(int _iDataSetSize, float _fTargetValue)
	{
		a1 = a2 = a3 = a4 = a5 = a6 = 1.0f;
		boxDim = 1.0; ;
		iDataSetSize = _iDataSetSize;
	}

	virtual void  boundingBox()
	{
		#define EPSILON pow(10.0f,-3.0f)
		b_min = vec(EPSILON, EPSILON, EPSILON);
		b_max = vec(PI - EPSILON, PI - EPSILON, PI - EPSILON);
		boxDim = b_max.x - b_min.x;

		fStepSize = boxDim / float(iDataSetSize);
	}

	virtual float  fSample(float x, float y, float z)
	{
		float fResult = (a1 * cos(1 * x) * cos(2 * y) * cos(3 * z)) +
			(a3 * cos(2 * x) * cos(1 * y) * cos(3 * z)) +
			(a4 * cos(2 * x) * cos(3 * y) * cos(1 * z)) +
			(a5 * sin(3 * x) * sin(1 * y) * sin(2 * z)) +
			(a2 * sin(1 * x) * sin(3 * y) * sin(2 * z)) +
			(a6 * sin(3 * x) * sin(2 * y) * sin(1 * z));

		return fResult;
	}

	
};



nodalSurf iso;




//////////////////////////////////////	OTHER DEFINITIONS / GLOBAL VARIABLES	//////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////				MVC APPLICATION				//////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
///// ---------------------------------------------------------- MODEL ------------------------------




void setup()
{

	iso = *new nodalSurf(12, 0.0);
	iso.fTargetValue = 0.0;

	//s = *new SliderGroup();
	S.addSlider();
	S.addSlider();
	S.addSlider();
	S.addSlider();
	S.addSlider();
	S.addSlider();
	S.addSlider();

	S.sliders[0].attachToVariable(&iso.a1, -(PI / 2.0), (PI / 2.0));
	S.sliders[1].attachToVariable(&iso.a2, -(PI / 2.0), (PI / 2.0));
	S.sliders[2].attachToVariable(&iso.a3, -(PI / 2.0), (PI / 2.0));
	S.sliders[3].attachToVariable(&iso.a4, -(PI / 2.0), (PI / 2.0));
	S.sliders[4].attachToVariable(&iso.a5, -(PI / 2.0), (PI / 2.0));
	S.sliders[5].attachToVariable(&iso.a6, -(PI / 2.0), (PI / 2.0));

	S.sliders[6].attachToVariable(&iso.fTargetValue, -PI, PI);
	
}



void update(int value)
{


}

void draw()
{


	backGround(0.75);

	drawGrid(20);

	// ---- draw iso surface ;	

	iso.display(false, true);

	glColor3f(1, 1, 1);


	// ---- draw 2d text ;	
	setup2d();

	char s[200];
//	sprintf(s, "FPS : %1.2f  RES : %i ", FPS, iso.iDataSetSize);
//	drawString(s, winW - 200, 50);

	restore3d();


}

void keyPress(unsigned char k, int xm, int ym)
{

	if (k == 27)S.clearSelection();

	if (k == '=') // increase threshold
		iso.fTargetValue *= 1.1;

	if (k == '-')// decrease threshold
		iso.fTargetValue /= 1.1;


	if (k == 'D') // increase density
		iso.increaseDensity();

	if (k == 'd') // increase density
		iso.decreaseDensity();


	if (k == 'w')
	{
		float diff = 0.05;
		iso.writeFile("isoSurf.obj");
		iso.fTargetValue += diff;
		iso.writeFile("isoSurf_neg_pos.obj");
		iso.fTargetValue -= diff;

		iso.fTargetValue -= diff;
		iso.writeFile("isoSurf_neg_neg.obj");
		iso.fTargetValue += diff;
	}

}

void mousePress(int b, int s, int x, int y)
{

	if (GLUT_LEFT_BUTTON == b && GLUT_DOWN == s)iso.fillCubeValues(0);

}

void mouseMotion(int x, int y)
{

	iso.fillCubeValues(0);

}



#endif // _MAIN_
