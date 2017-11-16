#define _MAIN_

#ifdef _MAIN_

//////////////////////////////////////				INCLUDES				//////////////////////////////////////

#include "main.h"
#include "nachi.h"


//////////////////////////////////////	OTHER DEFINITIONS / GLOBAL VARIABLES	//////////////////////////////////////

//NOTES
//since pathImporter defined in Nachi.h seems to do all the things we need, 
// except generating a cylinder, 
// we will make a custom class that inherits everything else from pathImporter
// and we will only add the cylinder generation method;
// This way of inherit and extending a class is called derived classes in C++.
// read about derived classes 


class myPathImporter : public pathImporter 
{
public:
	
	void addCylinder( double r, double h , vec cen)
	{
		vec a;
		//use parametric form of cylinder equation to generate the position vectors of points on a cylinder
		// use a nested for loop to cover the cylinder in plan and elevation
		for (double ang = 0; ang <= 2 * PI; ang += PI / 32)
		{
			for (double z = 0; z <= h; z++)
			{
				//generate point
				a.x = r * cos(ang);
				a.y = r * sin(ang);
				a.z = z;
				
				// a robot needs 4 vectors to be defined 
				// such that it can uniquely locate a position and orientation
				// in space : 
				// 1. the location of the tool center point (tcp) ,
				// 2. the x-axis of the tool (tcp_x)
				// 3. y-axis of tool (tcp_y)
				// 4. z-axis of tool (tcp_z)

				/*
				since we have generate only the position vector (a), 
				we will assignDefaultValues to the tcp_x,tcp_y & tcp_z;
				a only replace tcp with a;
				*/
				
				assignDefaultValues(); 
				/*this is a method defined in the base class i.e pathImporter
				this assigns default z-up values to tcp_x,tcp_y,tcp_z;
				the variables tcp,tcp_x,tcp_y & tcp_z are defined in the base class also.*/
				
				
				tcp = a + cen;
				//replace tcp with a ;

				addPoint(tcp,tcp_x,tcp_y,tcp_z); 
				// this is also a method defined in base class
				// this method adds the current path-point as uniquely defined by tcp_tcp_x,tcp_y & tcp_z vectors
				// into the path of the robot.
			}
		}
	}
};

//////////////////////////////////////	OTHER DEFINITIONS / GLOBAL VARIABLES	//////////////////////////////////////

// make an instance of teh derieved class as normal
myPathImporter path;

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

void draw()
{


	backGround(0.75);

	//use the draw method of the base class, as normal;
	path.draw();
}

void keyPress(unsigned char k, int xm, int ym)
{
	// add a event call back of the method we have added.
	// i.e add path
	if (k == 'a')
	{
		path.addCylinder(5, 40 , vec (20,20,10));
	}

	// use the other methods, such as gotoNextPoint() and exportGCode(), as defined in pathImporter as usual.
	if (k == 'n')path.goToNextPoint();
	if (k == 'w')path.exportGCode();

}

void mousePress(int b, int s, int x, int y)
{
}

void mouseMotion(int x, int y)
{

	
}



#endif // _MAIN_
