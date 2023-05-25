//https://cloud.tencent.com/developer/article/1731196
#include <stdlib.h>
#include <glut.h>//记住要glut.h 而不是GL/glut.h 另外stdlib.h一定要在glut.h前面标识

GLfloat x = 0.0;
GLfloat y = 0.0;
GLfloat size = 50.0;
GLsizei wh = 500, ww = 500;

void drawSquare(GLint x, GLint y)
{
	y = wh - y;
	glBegin(GL_POLYGON);
	glVertex3f(x + size, y + size, 0);
	glVertex3f(x - size, y + size, 0);
	glVertex3f(x - size, y - size, 0);
	glVertex3f(x + size, y - size, 0);
	glEnd();
}

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	drawSquare(x, y);
	glutSwapBuffers();
	glutPostRedisplay();
}

void init()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
}

void myReshape(GLint w, GLint h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, 0, h, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);

	ww = w;
	wh = h;
}

void myMouse(GLint button, GLint state, GLint wx, GLint wy)
{
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
		exit(0);
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x = wx;
		y = wy;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("click to display square");
	init();
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(myReshape);
	glutMouseFunc(myMouse);

	glutMainLoop();
	return 0;
}