#define BRESENHAM 1
#define CIRCLE 2
#define FLOODFILL 3
#define SCANLINEFILL 4
#define COHEN_SUTHERLAND 5
#define SUTHERLAND_HOGMAN 6
#define GETWATER 7
#include"Bresenham.h"
#include"FloodFill.h"
#include"CircleAndSquare.h"
#include"ScanFill.h"
#include"CohenSutherland.h"
#include"SutherlandHodgman.h"
#include"Bezier.h"
using namespace std;


int main(int argc, char** argv)
{
	cout << "请输入点的数量：" << endl;
	cin >> inputVertexNum;
	//cout << "请输入多边形边数：" << endl;
	//cin >> inputEdgeNumSH;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("OpenGL");
	glMatrixMode(GL_PROJECTION);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	gluOrtho2D(0, 800.0, 0, 800.0);
	//glutDisplayFunc(displayBresenham);
	//glutMouseFunc(mouseHitBH);

	//glutDisplayFunc(displayCircle);

	//cnt = 0;
	//cout << "请输入多边形边数：" << endl;
	//cin >> inputEdgeNum;
	//glutDisplayFunc(displayFloodFill);
	//glutMouseFunc(mouseHitFF);


	//k = 0;
	//cout << "请输入多边形边数：" << endl;
	//cin >> inputEdgeNum;
	//glutDisplayFunc(displayScanFill);
	//glutMouseFunc(mouseHitSF);

			
	//glutDisplayFunc(displayCohenSutherland);
	//glutMouseFunc(mouseHitCS);

			
	//glutDisplayFunc(displaySuthlandHodgman);
	//glutMouseFunc(mouseHitSH);
			
	glutDisplayFunc(displayBezier);
	glutMouseFunc(mouseHitBZ);

	//solve();

	glutMainLoop();
	return 0;
}

