#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cmath>
#include <gl/glut.h>
using namespace std;
unsigned cx, cy, Ra, Rb;

inline void drawPoint(
		 const unsigned &cx, const unsigned &cy,
		 const unsigned &x, const unsigned &y,
		 const unsigned char &R,
		 const unsigned char &G,
		 const unsigned char &B)
{
    glColor3b(R, G, B);
    glVertex2i( cx + x, cy + y);
}

inline void plot(
		 const unsigned &cx, const unsigned &cy,
		 const unsigned &x, const unsigned &y)
{
	drawPoint(cx, cy, x, y, 0, 0, 0);
	drawPoint(cx, cy, -x, y, 0, 0, 0);
	drawPoint(cx, cy, x, -y, 0, 0, 0);
	drawPoint(cx, cy, -x, -y, 0, 0, 0);
}

static void drawEllipse()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);
    unsigned &a=Ra, &b=Rb;
	unsigned asqr = a*a, bsqr = b*b;
	unsigned int x=a, y = 0u;
	plot(cx, cy, x, y);
	int x0 = asqr/sqrt(asqr+bsqr);
	int P = bsqr*((a<<1)-1)*((a<<1)-1) - ((asqr*(bsqr-1))<<2);
	while(x>=x0)
	{
		if(P>0)
		{
			P += bsqr*(8-(x<<3));
			--x;
		}
		P += asqr*(12+(y<<3));
		++y;
		plot(cx, cy, x, y);
	}
	x=0,y=b;
	P = asqr*((b<<1)-1)*((b<<1)-1)-((bsqr*(asqr-1))<<2);
	while(x<x0)
	{
		if(P>0)
		{
			P += asqr*(8-(y<<3));
			--y;
		}
		P += bsqr * ( 12 + (x<<3) );
		++x;
		plot(cx, cy, x, y);
	}
    glEnd();
    glFlush();

}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
	ios::sync_with_stdio(false);
	
	cout<<"Input the ellipse's cx, cy, Ra, and Rb, which should be seperated by spaces:"<<endl;
	cin>>cx>>cy>>Ra>>Rb;
    const unsigned width = cx + Ra;
	const unsigned height = cy + Rb;
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowSize(width, height);
	glutCreateWindow("Ellipse");
	glutDisplayFunc(&drawEllipse);
    glClearColor(1.0, 1.0, 1.0, 0.0);
    gluOrtho2D(0.0, width, 0.0, height);
	glutMainLoop();
	return 0;
}
