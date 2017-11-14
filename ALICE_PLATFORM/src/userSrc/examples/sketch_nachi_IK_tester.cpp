


#ifdef _MAIN_

#include "main.h"
#include "nachi.h"
#include "RenderMesh.h"


//////////////////////////////////////				INCLUDES				//////////////////////////////////////




//////////////////////////////////////	OTHER DEFINITIONS / GLOBAL VARIABLES	//////////////////////////////////////


Robot_Symmetric Nachi;
Mesh E;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////				MVC APPLICATION				//////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
///// ---------------------------------------------------------- MODEL ------------------------------

Matrix4 T_w0, T_ew, T_e0, T_ew_in , T_e0_in, T_w0_inv;
Matrix4 TOOL, TOOL_req;

void setup()
{
	Nachi.addMeshes();
	MeshFactory fac;
	E = fac.createFromOBJ("data/end_effector.obj", 1.0, false);
	//////////////////////////////////////////////////////////////////////////

	T_w0 = Nachi.Bars_to_world_matrices[5];
	normaliseFrame(T_w0);

	// assumes vector vtx1-vtx0 points in positive X, and vtx2 - vtx0, points in positive Y;
	// *-1 adjustment is to get the EE to point upward. This is internal adjustment, not to be changed.
	T_e0.setColumn(0, (E.positions[1] - E.positions[0]).normalise() * -1);
	T_e0.setColumn(1, (E.positions[2] - E.positions[0]).normalise());
	T_e0.setColumn(2, T_e0.getColumn(0).cross(T_e0.getColumn(1)).normalise());
	T_e0.setColumn(3, E.positions[0]);

	T_e0_in = T_e0;
	T_e0_in.invert();

	T_w0_inv = T_w0;
	T_w0_inv.invert();
	T_ew = T_w0_inv * T_e0;
	
	T_ew_in = T_ew;
	T_ew_in.invert();

	//////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < E.n_v; i++)E.positions[i] = T_w0_inv * E.positions[i];
	//for (int i = 0; i < E.n_v; i++)E.positions[i] = T_w0 * E.positions[i];
}

void update(int value)
{
	
	//Nachi.ForwardKineMatics(Nachi.rot);

}


///// ---------------------------------------------------------- VIEW  ------------------------------

void draw()
{


	backGround(0.75);
	drawGrid(20);

	
	Nachi.draw();
	
	{
		for (int i = 0; i < E.n_v; i++)E.positions[i] = T_w0 * E.positions[i];
			E.draw(false);
		for (int i = 0; i < E.n_v; i++)E.positions[i] = T_w0_inv * E.positions[i];
	}

		
	

	drawFrame(T_w0);
	drawFrame(T_e0);

	Matrix4 T;
	T = T_e0  * T_ew_in;
	T.setColumn(3, T.getColumn(3) + vec(0, 0, 0.5));
	drawFrame(T);


	drawFrame(TOOL_req);
	drawCircle(TOOL_req.getColumn(3), 3, 32);
	drawFrame(TOOL);
	

	
}


///// ---------------------------------------------------------- CONTROLLER ------------------------------


void mousePress(int b, int state, int x, int y)
{

}

void mouseMotion(int x, int y)
{

	
}

double inc = 0.0;
double r = 10;

void keyPress(unsigned char k, int xm, int ym)
{



	if (k == 'a')
	{
		


		assignDefaultFrame(TOOL);
		vec cen = vec(r * sin(inc), r * cos(inc), 0) + vec(60, 10, 10);
		inc += PI * 0.05;

		TOOL.setColumn(3, cen);

		TOOL_req = TOOL * T_ew_in;


		Nachi.inverseKinematics_analytical(TOOL_req, false);
		Nachi.ForwardKineMatics(Nachi.rot);

		vec ax, ay, az;
		ax = Nachi.Bars_to_world_matrices[5].getColumn(0);
		ay = Nachi.Bars_to_world_matrices[5].getColumn(1);
		az = Nachi.Bars_to_world_matrices[5].getColumn(2);
		//cout << pt.mag() << " lenghth " << endl;

		cout << ax.angle(TOOL_req.getColumn(0)) << endl;
		cout << ay.angle(TOOL_req.getColumn(1)) << endl;
		cout << az.angle(TOOL_req.getColumn(2)) << endl;

		T_w0 = Nachi.Bars_to_world_matrices[5];
		T_w0_inv = T_w0;
		T_w0_inv.invert();
	}

	
}





#endif // _MAIN_
