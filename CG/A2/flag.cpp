#include <gl/glut.h>
#include <iostream>

const int width = 1000, height = 500;

void drawFlag()
{
	static GLfloat leaf[][2]=
	{
        {0.012,-0.872},
        {0.008,-0.452},
        {0.210,-0.496},
        {0.188,-0.36},
        {0.388,-0.036},
        {0.336,0.020},
        {0.372,0.284},
        {0.246,0.236},
        {0.230,0.360},
        {0.106,0.092},
        {0.150,0.600},
        {0.076,0.524},
		{0.000,0.808}
	};
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	for(int i = 1; i<13; ++i)
	{
		glVertex2f(leaf[i][0], leaf[i][1]);
	}
    for(int i = 12; i>0; --i)
	{
		glVertex2f(-leaf[i][0], leaf[i][1]);
	}
	glEnd();
	glBegin(GL_POLYGON);
    glVertex2f(leaf[0][0], leaf[0][1]);
    glVertex2f(leaf[1][0], leaf[1][1]);
    glVertex2f(-leaf[1][0], leaf[1][1]);
    glVertex2f(-leaf[0][0], leaf[0][1]);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(-1.0, -1.0);
    glVertex2f(-0.5, -1.0);
    glVertex2f(-0.5, 1.0);
    glVertex2f(-1.0, 1.0);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(1.0, -1.0);
    glVertex2f(0.5, -1.0);
    glVertex2f(0.5, 1.0);
    glVertex2f(1.0, 1.0);
    glEnd();
	glFlush();
}

int main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(0.0, 0.0);
	glutCreateWindow("Flag");
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glutDisplayFunc(&drawFlag);
	glutMainLoop();
	return 0;
}
