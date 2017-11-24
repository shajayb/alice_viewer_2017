#ifndef _GRAPH_
#define _GRAPH_



#include "utilities.h"
#include <array>

#include "nvec.h"
//#include "matrices.h"
//#define _use_vector_
//#define _push_back_
#define MAX_GRAPH_EDGES 5000
#define MAX_GRAPH_VERTS 5000

class Graph
{

public:
	// ------------- topology 
	
	/// islands
	vector<int> connected_edges;
	vector<int> connected_vertices;
	array<bool, MAX_GRAPH_EDGES> eChecked;
	array<bool, MAX_GRAPH_EDGES> eAdded;


#ifdef _use_vector_

	vector<Vertex>vertices;
	vector<Edge>edges;
	int n_e, n_v;

	// ------------- attributes ;

	vector<vec>positions;
	

	// ------------- ------------- ------------- -------------------------- CONSTRUCTORS  ;
	Graph()
	{
		n_e = n_v = 0;
		
		vertices.reserve(MAX_VERTS);
		edges.reserve(MAX_EDGES);
		positions.reserve(MAX_VERTS);

		cout << "graph arrays reserved" << endl;
	}

#else


	Vertex *vertices;
	Edge *edges;
	int n_e, n_v;

	// ------------- attributes ;
	vec * positions;

	// ------------- ------------- ------------- -------------------------- CONSTRUCTORS  ;
	Graph()
	{
		n_e = n_v = 0;
		vertices = new Vertex[MAX_GRAPH_VERTS];
		edges = new Edge[MAX_GRAPH_EDGES];
		positions = new vec[MAX_GRAPH_VERTS];
	}


#endif // !_use vector_

	// ------------- ------------- ------------- -------------------------- TOPOLOGY
#ifdef _push_back_


	// VERTEX
	Vertex * createVertex(vec &p)
	{
		vertices.push_back(Vertex(vertices.size()));
		positions.push_back(p);
		n_v = vertices.size() -1;

		return &vertices[n_v];
	}

	// EDGE
	Edge * createEdge(Vertex &str, Vertex &end)
	{
		edges.push_back( Edge(str, end, edges.size()) );
		n_e = edges.size() -1;
		str.addEdge(&edges[n_e]);
		end.addEdge(&edges[n_e]);
		
		return &edges[n_e];
	}

#else 

	Vertex * createVertex(vec &p)
	{
		//if (n_v >= MAX_GRAPH_VERTS)return NULL;

		vertices[n_v] = Vertex(n_v);
		positions[n_v] = p;
		n_v++;

		return &vertices[n_v - 1];
	}

	// EDGE
	Edge * createEdge(Vertex &str, Vertex &end)
	{
		//if (n_e >= MAX_GRAPH_EDGES)return NULL;

		edges[n_e] = Edge(str, end, n_e);
		str.addEdge(&edges[n_e]);
		end.addEdge(&edges[n_e]);

		n_e++;
		return &edges[n_e - 1];
	}

#endif // !_push_back_


	// ------------- ------------- ------------- -------------------------- UTILITIES - TOPOLOGY


	void constructFromGraph(Graph &G)
	{

		for (int i = 0; i < G.n_v; i += 1)
			createVertex(G.positions[i]);

		for (int i = 0; i < G.n_v; i += 1)
			createEdge(vertices[G.edges[i].vStr->id], vertices[G.edges[i].vEnd->id]);

	}

	void constructFromFile( string fileToRead )
	{


		std::fstream fs(fileToRead.c_str(), ios::in);

		if (fs.fail())
		{
			cout << " error in file reading " << fileToRead << endl;
			return;
		}

		//actualPathLength = 0;
		reset();
		//
		int cnt = 0;
		while (!fs.eof() && cnt < 10000)
		{
			char str[2000];
			fs.getline(str, 2000);
			vector<string> content = splitString(str, ",");

			if (content.size() == 3)
			{
				vec p = extractVecFromStringArray(0, content);
				//p *= 0.1;
				createVertex(p);
			}

			if (content.size() == 2)
			{
				int v_str = atoi(content[0].c_str());
				int v_end = atoi(content[1].c_str());
				createEdge( vertices[v_str], vertices[v_end]);
			}

		}

		fs.close();
	}
	
	void boundingbox( vec &min,vec &max)
	{
		min = vec (1e10, 1e10, 1e10);
		max = min * -1;
		vec p;

		for ( int i = 0 ; i < n_v ; i ++ )
		{
			p = positions[i];
			min.x = MIN(min.x, p.x);
			min.y = MIN(min.y, p.y);
			min.z = MIN(min.z, p.z);

			max.x = MAX(max.x, p.x);
			max.y = MAX(max.y, p.y);
			max.z = MAX(max.z, p.z);
		}
	}

	void reset()
	{

		n_v = n_e = 0;
		
#ifdef _use_vector_

		vertices.clear();
		edges.clear();
		positions.clear();

#endif // _use_vector_



	}

	Edge * edgeExists(Vertex &str, Vertex &end, bool &found)
	{

		for (int i = 0; i < str.n_e; i++)
		{
			found = true;
			Edge e = *(str.edgePtrs[i]);
			//cout << &(str.edges[i]) <<  " " <<  &(str.edges[i].vStr) << " --- c -- " <<  &end <<  " " << &str << endl ;
			if (e.vStr == &end || e.vEnd == &end)return str.edgePtrs[i];

		}

		//printf( " none found , creating new %i %i \n " , str.id , end.id ) ;
		found = false;
		return createEdge(str, end);
	}

	vec getEdgeCenter( int edgeId)
	{
		return ( positions[ edges[edgeId].vEnd->id] + positions[ edges[edgeId].vStr->id]) * 0.5;
	}
	vec getEdgeDir( int edgeId)
	{
		return positions[ edges[edgeId].vEnd->id ] - positions[ edges[edgeId].vStr->id ];
	}
	
	vec getEdgeDir( int vertexId , int relativeEdgeId )
	{
		int other = getOtherVertex(vertexId, relativeEdgeId);
		vec n = positions[other] - positions[vertexId];
		return n;
	}

	int getOtherVertex(int vertexId, int relativeEdgeId)
	{
		Edge E = *vertices[vertexId].edgePtrs[relativeEdgeId];
		int other = E.vEnd->id == vertexId ? E.vStr->id : E.vEnd->id;
		return other; 
	}
	// ------------- ------------- ------------- -------------------------- COMPUTE - TOPOLOGY

	generator<Edge> getNextEdge(Edge &e)
	{

		for (int i = 0; i < e.vEnd->n_e; i++)
			if (e.vEnd->edgePtrs[i] != &e)co_yield *(e.vEnd->edgePtrs[i]);

		for (int i = 0; i < e.vStr->n_e; i++)
			if (e.vStr->edgePtrs[i] != &e)co_yield *(e.vStr->edgePtrs[i]);

	}

	void addEdgeToList( Edge &e , vector<int> &connectedEdgeList)
	{
		if (!eAdded[e.id])
		{
			eAdded[e.id] = true;
			connectedEdgeList.push_back(e.id);
		}
	}
	// generator<Edge>  !! IMP : co-routine functions cannot be recursive. !
	void recurseEdges(Edge &startEdge, vector<int> &connectedEdgeList)
	{
		addEdgeToList(startEdge, connectedEdgeList);

		vector<Edge> con_edges;
		for (auto e : getNextEdge(startEdge))
			if (!eChecked[e.id])
			{
				con_edges.push_back(e);
				addEdgeToList(e, connectedEdgeList);
			}

		eChecked[startEdge.id] = true;
		

		for (auto &e : con_edges)
			if (!eChecked[e.id])
			{
				recurseEdges(e, connectedEdgeList);
				eChecked[e.id] = true;
			}
			//else
				
	}

	void computeIslandsAsEdgeAndVertexList()
	{

		for (auto &c : eChecked)c = false;
		for (auto &c : eAdded)c = false;
		
		connected_edges.clear();
		
		for (int i = 0; i < n_e; i++)
			if (!eChecked[i])//glColor3f(clr.r, clr.g, clr.b);
				recurseEdges(edges[i], connected_edges);

		connected_vertices.clear();
		computeConnectedVertexListFromEdgeList(connected_edges);
	}

	void computeConnectedVertexListFromEdgeList(vector<int> &connectedEdgeList)
	{
		if (!connectedEdgeList.size() > 0)return;
		
		swap(connected_edges[0], connected_edges[2]); // !! temporary fix.. this function should be recursive
		swap(connected_edges[1], connected_edges[2]); // !! temporary fix.. this function should be recursive

		int str_Vid;
		Edge e = edges[connectedEdgeList[0]];
		
		connected_vertices.push_back(e.vEnd->id);
		connected_vertices.push_back(e.vStr->id);
		str_Vid = e.vEnd->id;

		
		int n = connectedEdgeList.size();
		for (int i = 1; i < n ; i += 1) 
		{
			Edge e = edges[connectedEdgeList[i]];
			int e1 = e.vEnd->id;
			int e2 = e.vStr->id;
			str_Vid = (e1 == str_Vid) ? e2 : e1;
			
			connected_vertices.push_back(str_Vid);
		}

		// !! temporary fix.. this is for digitalFUTURES hotFix;
		{
			bool flipped = false;
			vec e1 = positions[connected_vertices[1]] - positions[connected_vertices[0]];
			vec e2 = positions[connected_vertices[2]] - positions[connected_vertices[1]];
			flipped = e1 * e2 < 0 ? true : false;
			if (flipped)
				swap(connected_vertices[0], connected_vertices[1]);
		}
		// !!! IMP test
		//for (int i = 0; i < 1; i++)connected_vertices.pop_back();
	}
	
	void smooth_connectedVertices( double breakAngle = 360.0 )
	{
		if (!connected_vertices.size() > 0)return;
		
		int n = connected_vertices.size();
		
		for (int i = 0; i < n; i += 1)
		{
			int Vi = connected_vertices[Mod(i, n)];
			int Vi_minus = connected_vertices[Mod(i - 1, n)];
			int Vi_plus = connected_vertices[Mod(i + 1, n)];

			positions[Vi] = positions[Vi_minus] * 0.3 + positions[Vi] * 0.4 + positions[Vi_plus] * 0.3;
		}
	}

	double averageEdgeLenght()
	{
		double d = 0;
		for (int i = 0; i < n_e; i++) d += positions[edges[i].vEnd->id].distanceTo(positions[edges[i].vStr->id]);
		d /= n_e;
		return d;
	}

	void smoothGraph( double breakAngle = 360.0)
	{
		int n = n_v;

		for (int i = 0; i < n; i += 1)
		{
			int Vi = Mod(i, n);
			int Vi_minus = Mod(i - 1, n);
			int Vi_plus = Mod(i + 1, n);
			vec ed1 = positions[Vi] - positions[Vi_minus];
			vec ed2 = positions[Vi_plus] - positions[Vi];
			
			double ang = ed2.angle(ed1);
			if( fabs(ang) < breakAngle )
				positions[Vi] = positions[Vi_minus] * 0.3 + positions[Vi] * 0.4 + positions[Vi_plus] * 0.3;
		}
	}

	vec  centroid()
	{
		vec cen;
		for (int i = 0; i < n_v; i++)cen += positions[i];

		cen /= n_v;
		return cen;
	}

	void inflateVertices()
	{
		vec cen;
		for (int i = 0; i < n_v; i++)cen += positions[i];

		cen /= n_v;

		for (int i = 0; i < n_v; i++)
		{
			vec n = positions[i] - cen;
			n.normalise();
			positions[i] += n * 0.01;
		}
	}

	void inflateWRTMedialAxis( Graph &G )
	{
		for (int i = 0; i < n_v; i++)
		{
			vec np = G.nearestPointOnGraph(positions[i]);

			vec n = positions[i] - np;
			n.normalise();
			positions[i] += n * 0.07;
		}
	}

	void redistribute_toroidal(double dis = 0.2)
	{
		Graph A;


		for (int i = 0; i < n_v; i += 1)
		{
			vec ptStr = positions[i];
			vec ptEnd = positions[ Mod(i + 1, n_v)];
			vec ed = (ptEnd - ptStr);
			double len = ed.mag();
			ed.normalise();
		
			
			if( dis < len ) 
			{
				int numDivs = floor(len / dis);
				for (int n = 0; n <= numDivs; n += 1)
					A.createVertex(ptStr + ed * dis * n);
			}
			else
			{
				A.createVertex(ptStr);
			}
		}


		reset();

		for (int i = 0; i < A.n_v; i += 1)
			createVertex(A.positions[i]);

		for (int i = 0; i < n_v; i += 1)
			createEdge(vertices[i], vertices[Mod(i + 1, n_v)]);
		//for (int i = 0; i < grid_points.rows(); i++)
	}

	void renumber( vec refPt)
	{
			Graph A;

			int nearest;
			double dMin = 1e10;

			for (int i = 0; i < n_v; i += 1)
			{
				double d = refPt.distanceTo(positions[i]);
				if (d < dMin)
				{
					dMin = d;
					nearest = i;
				}
			}
			

			for (int i = nearest; i < n_v; i++)	A.createVertex( positions[i]);
			for (int i = 0; i < nearest; i++)A.createVertex(positions[i]);

			reset();

			for (int i = 0; i < A.n_v; i += 1)
				createVertex(A.positions[i]);

			for (int i = 0; i < n_v; i += 1)
				createEdge(vertices[i], vertices[Mod(i + 1, n_v)]);

	}

	// ------------- ------------- ------------- -------------------------- DISPLAY 

	void drawConnectedEdgeList( bool debugDraw = false)
	{
		if (!connected_vertices.size() > 0)return;

		int e1, e2;
		int n = connected_vertices.size();
		for (int i = 0; i < n; i += 1) // += 3 -> fix! (?)
		{
			e1 = connected_vertices[i];
			e2 = connected_vertices[ Mod(i+1,n) ];
			drawLine(positions[e1], positions[e2]);
				
			if (debugDraw)
			{
				glPointSize(3);
				drawPoint(positions[e1]);
				char c[200];
				sprintf(c, "%i ", i);
				string s = "";
				s += c;
				drawString(s, positions[e1]);
			}
		}

	}

	void drawConnectedEdgeNumbers()
	{

		for (int i = 0; i < connected_edges.size(); i += 1)
		{
			int e1 = connected_edges[i];
			Edge e = edges[e1];
			vec pt = (positions[e.vEnd->id] + positions[e.vStr->id]) * 0.5;
			char c[200];
			sprintf(c, "%i ", i);
			string s = "";
			s += c;
			drawString(s,pt);

		}
	}

	void drawConnectedVertexList()
	{

		for (int i = 0; i < connected_vertices.size(); i += 1)
			drawPoint(positions[connected_vertices[i]]);
	}

	int Mod(int a, int n)
	{
		a = a % n;
		return (a < 0) ? a + n : a;
	}


	void draw_connectedVertices()
	{
		if (!connected_vertices.size() > 0)return;

			for (int i = 0; i < connected_vertices.size(); i += 1)drawPoint(positions[connected_vertices[i]]);
	}

	// ------------- ------------- ------------- -------------------------- UTILITIES
	void writeGraph(float scaleBack, string outFileName = "data/graph.txt")
	{
		printf(" ----------- writing \n ");

		ofstream myfile;
		myfile.open(outFileName.c_str(), ios::out);


		if (myfile.fail())
		{
			myfile << " error in opening file  " << outFileName.c_str() << endl;
			return;
		}

		// vertices
		for (int i = 0; i < n_v; i++)
		{

			char s[200];
			sprintf(s, "%1.4f,%1.4f,%1.4f", positions[i].x * scaleBack, positions[i].y * scaleBack, positions[i].z * scaleBack);//(vertices[i].n_e == 1) ? 1 : 0

			myfile << s << endl;
		}

		for (int i = 0; i < n_e; i++)
		{

			char s[200];
			sprintf(s, "%i,%i", edges[i].vStr->id, edges[i].vEnd->id);

			myfile << s << endl;
		}


		myfile.close();
		return;

	}

	plane getCurrentPlane()
	{
		vec tangent = positions[1] - positions[0];
		vec tangent1 = positions[2] - positions[0];
		vec norm = tangent.cross(tangent1);
		//vec binorm = norm.cross(tangent);
		norm.normalise(); //binorm.normalise();

		vec cen;
		for (int i = 0; i < n_v; i++) cen += positions[i];
		cen /= n_v;


		plane Pl;
		Pl.cen = cen;
		Pl.normal = norm;

		return Pl;
	}

	void writeVerticeToFile( ofstream &myfile)
	{

		plane PL = getCurrentPlane();
		// vertices
		//for (int i = 0; i < n_v; i++)
		//{

		//	char s[200];
		//	sprintf(s, "%1.4f,%1.4f,%1.4f,%1.4f,%1.4f,%1.4f", positions[i].x * 1, positions[i].y * 1, positions[i].z * 1,
		//													PL.normal.x * 1, PL.normal.y * 1, PL.normal.z * 1);//(vertices[i].n_e == 1) ? 1 : 0

		//	myfile << s << endl;
		//}

		vec p, u, v, n;
		n = PL.normal;
		n.normalise();
		double scale = 1.0;

		for (int i = 0; i < n_v; i++)
		{
			p = positions[i];
			
			u = ( positions[(i + 1) % n_v] - positions[i] ).normalise();
			v = n.cross(u).normalise();


			char s[300];
			sprintf(s, "%1.4f,%1.4f,%1.4f,%1.4f,%1.4f,%1.4f,%1.4f,%1.4f,%1.4f,%1.4f,%1.4f,%1.4f", 
					u.x * scale, u.y * scale, u.z * scale,
					v.x * scale, v.y * scale, v.z * scale,
					n.x * scale, n.y * scale, n.z * scale,
					p.x * scale, p.y * scale, p.z * scale
				);//(vertices[i].n_e == 1) ? 1 : 0

			myfile << s << endl;
		}


	}

	vec nearestPointOnGraph( vec inPt )
	{
		vec nearest; vec pt;
		float min = 1e10;
		for (int j = 0; j < n_e; j++)
		{
			int e0, e1;
			e0 = edges[j].vEnd->id;
			e1 = edges[j].vStr->id;
			
			float Di = distanceSquaredAndNearestPointOnEdge(positions[e0], positions[e1], inPt, pt);
			if (Di < min)
			{
				min = Di;
				nearest = pt;
			}
		}

		return nearest;

	}
	// ------------- ------------- ------------- -------------------------- DISPLAY

	void draw_edge( Edge &e )
	{

	}

	void drawTextAtVec( string s, vec &pt)
	{
		unsigned int i;
		glRasterPos3f(pt.x, pt.y, pt.z);

		for (i = 0; i < s.length(); i++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s[i]);
	}

	void draw(bool drawPoints = true)
	{

		//{
		//	char s[200];
		//	itoa(0, s, 10);
		//	drawString(s, positions[0]+ vec(0,0,.1));
		//	drawPoint(positions[0]);
		//	//drawR(positions[i], 0.05, 32);
		//}


		glPointSize(5);
		if(drawPoints)
		for (int i = 0; i < n_v; i++)
		{
			char s[200];
			itoa(i, s, 10);
			drawTextAtVec( string(s), positions[i]+ vec(0,0,0.01));
			drawPoint(positions[i]);
			//drawR(positions[i], 0.05, 32);
		}
		glPointSize(1);

		
		
		for (int i = 0; i < n_e; i++)
		{
			char s[200];
			itoa(i, s, 10);
			//drawString(s, (positions[edges[i].vStr->id] + positions[edges[i].vEnd->id]) * 0.5 + vec(0, 0, .1));
			drawLine(positions[edges[i].vStr->id], positions[edges[i].vEnd->id]);
		}

		//for (auto &v : vertices) drawPoint(positions[v.id]);
		//for (auto &e : edges) drawLine(positions[e.vEnd->id], positions[e.vStr->id]);
	}


	void draw( nvec &dists, vector<vec> &nPts)
	{
		vec4 clr;
		vec pt;
		float minD,maxD;
		minD = dists.min();
		maxD = dists.max();

		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


		for (int i = 0; i < n_e; i++)
		{
			float d = 4;
			glLineWidth(d);
			glBegin(GL_LINES);
			glLineWidth(ofMap(dists.x[edges[i].vStr->id], minD, maxD, 0, 2));

				clr = getColour(dists.x[edges[i].vStr->id], minD, maxD);

				pt = positions[edges[i].vStr->id];
				glColor3f(clr.r, clr.g, clr.b);
				glVertex3f(pt.x, pt.y, pt.z);

				clr = getColour(dists.x[edges[i].vEnd->id], minD, maxD);
				pt = positions[edges[i].vEnd->id];
				glColor3f(clr.r, clr.g, clr.b);
				glVertex3f(pt.x, pt.y, pt.z);
			
			glEnd();


		}


		glDisable(GL_BLEND);

		//////////////////////////////////////////////////////////////////////////
		glLineWidth(1);
		lineStyle(5);
			for (int i = 0; i < n_v; i++)drawLine(positions[i], nPts[i]);

		glDisable(GL_LINE_STIPPLE);

	
	}


};



class toroidalGraph : public Graph
{
public:

	vector<bool> fixed;

	void constructFromGraph( Graph &G , int everyNthVert = 1 )
	{
		reset();
		for (int i = 0; i < G.n_v; i+= everyNthVert ) createVertex(G.positions[i]);
		
		for (int i = 0; i < n_v; i++) createEdge(vertices[Mod(i, n_v)], vertices[Mod(i + 1, n_v)]);
		fixed.assign(n_v,false);
	}

	void constructFromPoints( vec *positions, int n)
	{

		reset();
		for (int i = 0; i < n; i++) createVertex(positions[i]);
		fixed.assign(n_v, false);

		for (int i = 0; i < n_v; i++) createEdge(vertices[Mod(i, n_v)], vertices[Mod(i + 1, n_v)]);
	}

	void fixEnds()
	{
		fixed[0] = fixed[n_v - 1] = true;
	}



	void smoothVertices()
	{
		//if (!vertices.size() > 0)return;
		int n = n_v;

		for (int i = 0; i < n; i += 1)
			if(!fixed[i])
			{

				int Vi = Mod(i, n);
				int Vi_minus = Mod(i - 1, n);
				int Vi_plus = Mod(i + 1, n);
				positions[Vi] = positions[Vi_minus] * 0.3 + positions[Vi] * 0.4 + positions[Vi_plus] * 0.3;
			}
	}

	void moveEachPointTowardNext( double dist = 0.1 )
	{
		for (int i = 0; i < n_v; i++)
			if (!fixed[i])
			{
				vec ed = (positions[Mod(i + 1, n_v)] - positions[i]);
				double d = ed.mag();
				
				if( d < dist )positions[i] += (positions[Mod(i + 1, n_v)] - positions[i]).normalise() * 0.1;
			}
	}

	void getMinMaxEdgeLength( double &minL, double &maxL)
	{
		
		minL = 1e12; maxL = minL * -1.0;
		vec ed;
		for (int i = 0; i < n_e; i++)
		{
			if (fixed[edges[i].vEnd->id] && fixed[edges[i].vStr->id])continue;

			ed = positions[edges[i].vEnd->id] - positions[edges[i].vStr->id];
			double d = ed * ed;
			
			minL = MIN(minL, d);
			maxL = MAX(maxL, d);
		}
	}

	void display()
	{
		double minL, maxL;
		
		vec ed;
		getMinMaxEdgeLength(minL, maxL);
		//printf("%1.4f, %1.4f \n", minL, maxL);
		
		for (int i = 0; i < n_e; i++)
		{
			if (fixed[edges[i].vEnd->id] && fixed[edges[i].vStr->id])continue;

			ed = positions[edges[i].vEnd->id] - positions[edges[i].vStr->id];
			vec4 clr = getColour(ed * ed, minL, minL * 1.1 );
			glColor3f(clr.r, clr.g, clr.b);
			drawLine( positions[ edges[i].vEnd->id ],  positions[ edges[i].vStr->id ] );

		}
	}
};

#endif