//#define _MAIN_
//#define _ALG_LIB_

#ifdef _MAIN_




#include "main.h"

#include <array>
#include <memory>
#include<time.h>

#include<experimental/generator> 
using namespace std;
using namespace std::experimental;

#include "graph.h"
#include "Matrix3x3.h"
#include "matrices/matrices.h"
//


#include <time.h>       /* time_t, struct tm, difftime, time, mktime */
#include "linearAlgebra.h"
#include "gl2ps.h"

Graph G;
importer imp;

bool strEdges = true;

//FDM

vector<g_Vert> GV;
vector<g_Edge> GE;
nvec densities;
double load = 1.0;
double forceDensity = 1.0;
// stresses
nvec dists;
vector<vec> nPts;

//////////////////////////////////////////////////////////////////////////

//void GraphFromFile(string fileToRead, Graph &G)
//{
//
//
//	std::fstream fs(fileToRead.c_str(), ios::in);
//
//	if (fs.fail())
//	{
//		cout << " error in file reading " << fileToRead << endl;
//		return;
//	}
//
//	//actualPathLength = 0;
//	G.reset();
//	//
//	int cnt = 0;
//	while (!fs.eof() && cnt < 10000)
//	{
//		char str[2000];
//		fs.getline(str, 2000);
//		vector<string> content = splitString(str, ",");
//
//		cout << content.size() << endl;
//		if (content.size() == 4)
//		{
//			vec p = extractVecFromStringArray(0, content);
//			//p *= 0.1;
//			G.createVertex(p);
//		}
//
//		if (content.size() == 2)
//		{
//			int v_str = atoi(content[0].c_str());
//			int v_end = atoi(content[1].c_str());
//			G.createEdge(G.vertices[v_str], G.vertices[v_end]);
//		}
//
//
//	}
//
//	fs.close();
//}

float nearestEdge(vec &inPt, vec &nPt)
{
	float minD = pow(10.0, 10.0);
	int nearest = 0;

	float LineMag, U;
	vec Intersection;

	for (int m = 0; m < GE.size(); m++)
	{
		vec p1 = GV[GE[m].to].pos;
		vec p2 = GV[GE[m].from].pos;

		LineMag = (p2 - p1).mag();

		U = (((inPt.x - p1.x) * (p2.x - p1.x)) +
			((inPt.y - p1.y) * (p2.y - p1.y)) +
			((inPt.z - p1.z) * (p2.z - p1.z))) /
			(LineMag * LineMag);

		if (U < 0.0f || U > 1.0f)continue;


		Intersection.x = p1.x + U * (p2.x - p1.x);
		Intersection.y = p1.y + U * (p2.y - p1.y);
		Intersection.z = p1.z + U * (p2.z - p1.z);

		float dis = inPt.distanceTo(Intersection);


		if (dis < minD)
		{
			minD = dis;
			nPt = Intersection;
		}
	}

	return minD;
}

void straightenEdges(bool bestFit = true)
{
	//for (int i = 0; i < G.n_v; i++)G.positions[i] = GV[i].orig;

	densities = nvec(G.n_e);
	densities = calculateBestFitQ(GV, GE, -load);
	for (int i = 0; i < densities.size(); i++)densities.x[i] = bestFit ? ofClamp(densities.x[i], 0.1, 8.0) : forceDensity;

	FDM(GV, GE, densities, -load);

	for (int i = 0; i < G.n_v; i++)G.positions[i] = GV[i].pos;
}

void approxStresses()
{

	dists.x.clear();
	nPts.clear();

	for (int i = 0; i < G.n_v; i++)
	{
		nPts.push_back(GV[i].orig);
		dists.x.push_back(GV[i].orig.distanceTo(GV[i].pos));
	}


}

void setup()

{

	S = *new SliderGroup();
	S.addSlider(&load, "load");
	S.sliders[0].minVal = 0.01;
	S.sliders[0].maxVal = 1.0;

	S.addSlider(&forceDensity, "forceDensity");
	S.sliders[1].minVal = 0.01;
	S.sliders[1].maxVal = 15.0;


	strEdges = false;


	imp = *new importer("data/comp_net.txt", 100000, 1.0);
	imp.readEdges();

	vec min(1e32, 1e32, 1e32);
	vec max = min * -1;

	for (int i = 0; i < imp.nCnt; i++)
	{
		min.x = MIN(imp.nodes[i].pos.x, min.x);
		min.y = MIN(imp.nodes[i].pos.y, min.y);
		min.z = MIN(imp.nodes[i].pos.z, min.z);

		max.x = MAX(imp.nodes[i].pos.x, max.x);
		max.y = MAX(imp.nodes[i].pos.y, max.y);
		max.z = MAX(imp.nodes[i].pos.z, max.z);
	}

	//vec cen = (min + max)*0.5;
	//for (int i = 0; i < imp.nCnt; i++)imp.nodes[i].pos -= cen;
	//for (int i = 0; i < imp.nCnt; i++)imp.nodes[i].pos /= (max - min).mag() * 0.01;

	for (int i = 0; i < imp.nCnt; i++)imp.nodes[i].pos *= 1;

	//////////////////////////////////////////////////////////////////////////
	
	G = *new Graph();

	for (int i = 0; i < imp.nCnt; i++)G.createVertex(imp.nodes[i].pos);
	for (int i = 0; i < imp.eCnt; i++)G.createEdge(G.vertices[imp.edges[i].n0], G.vertices[imp.edges[i].n1]);
	

	//////////////////////////////////////////////////////////////////////////

	GV.clear();
	GE.clear();

	for (int i = 0; i < G.n_v; i++)
	{
		g_Vert v;
		v.valence = 0;
		v.pos = v.orig = G.positions[i];
		v.boundary = G.vertices[i].onBoundary();
		GV.push_back(v);
	}

	for (int i = 0; i < G.n_e; i++)
	{
		//if(m.edges[i].onBoundary())continue ;

		g_Edge e;
		e.from = G.edges[i].vStr->id;
		e.to = G.edges[i].vEnd->id;
		GE.push_back(e);
		GV[e.from].valence += 1;
		GV[e.to].valence += 1;
	}

	for (int i = 0; i < G.n_v; i++)
		GV[i].boundary = (GV[i].valence == 1) ? 1 : 0;




}


void update(int value)
{
}


void draw()
{
	backGround(1.0);


	S.draw();
	B.draw();

	glColor3f(0, 0, 0);
	glLineWidth(1);

	if (strEdges)
		G.draw(dists, nPts);
	else
		G.draw();
	//G.draw(dists, nPts);

	////
	glPointSize(8);
	for (int i = 0; i < GV.size(); i++)
		if (GV[i].boundary)drawPoint(GV[i].pos);

	glPointSize(1);

	//////////////////////////////////////////////////////////////////////////

	//glColor3f(0, 0, 0);
	//glLineWidth(1);
	//lineStyle(5);
	//for (int i = 0; i < GE.size(); i++)drawLine( GV[GE[i].to].orig, GV[GE[i].from].orig );

	glDisable(GL_LINE_STIPPLE);


}

void keyPress(unsigned char k, int xm, int ym)
{

	if (k == 's')
	{
		for (int i = 0; i < G.n_v; i++)G.positions[i] = GV[i].orig;
		straightenEdges();
		approxStresses();
		strEdges = !strEdges;
	}

	if (k == 'a')
	{
		for (int i = 0; i < G.n_v; i++)G.positions[i] = GV[i].orig;
		straightenEdges(false);
		approxStresses();
		strEdges = !strEdges;
	}

	if (k == 'S')for (int i = 0; i < G.n_v; i++)G.positions[i] = GV[i].orig;

	if (k == 'w')
	{
		time_t rawtime;
		struct tm * ptm;

		time(&rawtime);
		ptm = gmtime(&rawtime);

		char tStamp[300];
		sprintf(tStamp, "%2d_%2d_%2d_%02d", ptm->tm_year - 100, (ptm->tm_mon + 1), (ptm->tm_hour + 0) % 24, ptm->tm_min);

		string solvedOBJ = "data/edges_" + string(tStamp) + ".OBJ";
		string solvedTXT = "data/edges_" + string(tStamp) + ".txt";

		G.writeGraph(1.0, solvedTXT);
		G.writeGraph(1.0, "data/comp_net_out.txt");

	}

	if (k == 'A')
	{
		FILE *fp;
		int state = GL2PS_OVERFLOW, buffsize = 0;

		fp = fopen("out.eps", "w");
		printf("Writing 'out.eps'... ");

		while (state == GL2PS_OVERFLOW)
		{
			buffsize += winW * winH;
			gl2psBeginPage("test", "gl2psTestSimple", NULL, GL2PS_EPS, GL2PS_SIMPLE_SORT,
				GL2PS_USE_CURRENT_VIEWPORT,
				GL_RGBA, 0, NULL, 0, 0, 0, buffsize, fp, "out.eps");

			draw();

			state = gl2psEndPage();
		}

		fclose(fp);
		printf("Done!\n");

	}

}


void mousePress(int b, int s, int x, int y)
{
	if (GLUT_LEFT_BUTTON == b && GLUT_DOWN == s)
	{
		S.performSelection(x, y, HUDSelectOn);
		B.performSelection(x, y);
		if (strEdges && HUDSelectOn)
		{
		straightenEdges();
		approxStresses();
		}

	}
}

void mouseMotion(int x, int y)
{
	S.performSelection(x, y, HUDSelectOn);
	if (strEdges && HUDSelectOn)
	{
		straightenEdges();
		approxStresses();
	}
}


#endif // _MAIN_