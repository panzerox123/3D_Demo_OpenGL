#include <GL/freeglut.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>

int THETA = 0;

float rand_color_func(){
	return (float)rand()/(float)RAND_MAX;
}

class Cube
{
	void displayCube()
	{
		glBegin(GL_TRIANGLES);
		glColor3f(rand_color_func(), rand_color_func(), rand_color_func());
		glNormal3f(0, 0, 1);
		glVertex3f(-1.0, -1.0, 1.0);
		glVertex3f(1.0, -1.0, 1.0);
		glVertex3f(1.0, 1.0, 1.0);
		glEnd();
		glBegin(GL_TRIANGLES);
		glColor3f(rand_color_func(), rand_color_func(), rand_color_func());
		glNormal3f(0, 0, 1);
		glVertex3f(-1.0, 1.0, 1.0);
		glVertex3f(1.0, 1.0, 1.0);
		glVertex3f(-1.0, -1.0, 1.0);
		glEnd();

		glBegin(GL_TRIANGLES);
		glColor3f(rand_color_func(), rand_color_func(), rand_color_func());
		glNormal3f(1, 0, 0);
		glVertex3f(1, -1, 1);
		glVertex3f(1, 1, 1);
		glVertex3f(1, 1, -1);
		glBegin(GL_TRIANGLES);
		glColor3f(rand_color_func(), rand_color_func(), rand_color_func());
		glNormal3f(1, 0, 0);
		glVertex3f(1, -1, -1);
		glVertex3f(1, 1, -1);
		glVertex3f(1, -1, 1);
		glEnd();

		glBegin(GL_TRIANGLES);
		glColor3f(rand_color_func(), rand_color_func(), rand_color_func());
		glNormal3f(0, 0, -1);
		glVertex3f(-1, -1, -1);
		glVertex3f(1, -1, -1);
		glVertex3f(1, 1, -1);
		glEnd();
		glBegin(GL_TRIANGLES);
		glColor3f(rand_color_func(), rand_color_func(), rand_color_func());
		glNormal3f(0, 0, -1);
		glVertex3f(-1, 1, -1);
		glVertex3f(1, 1, -1);
		glVertex3f(-1, -1, -1);
		glEnd();

		glBegin(GL_TRIANGLES);
		glColor3f(rand_color_func(), rand_color_func(), rand_color_func());
		glNormal3f(-1, 0, 0);
		glVertex3f(-1, -1, 1);
		glVertex3f(-1, 1, 1);
		glVertex3f(-1, 1, -1);
		glEnd();
		glBegin(GL_TRIANGLES);
		glColor3f(rand_color_func(), rand_color_func(), rand_color_func());
		glNormal3f(-1, 0, 0);
		glVertex3f(-1, -1, -1);
		glVertex3f(-1, 1, -1);
		glVertex3f(-1, -1, 1);
		glEnd();

		glBegin(GL_TRIANGLES);
		glColor3f(rand_color_func(), rand_color_func(), rand_color_func());
		glNormal3f(0, 1, 0);
		glVertex3f(1, 1, 1);
		glVertex3f(-1, 1, 1);
		glVertex3f(-1, 1, -1);
		glEnd();
		glBegin(GL_TRIANGLES);
		glColor3f(rand_color_func(), rand_color_func(), rand_color_func());
		glNormal3f(0, 1, 0);
		glVertex3f(1, 1, -1);
		glVertex3f(1, 1, 1);
		glVertex3f(-1, 1, -1);
		glEnd();

		//bottom face
		glBegin(GL_TRIANGLES);
		glColor3f(rand_color_func(), rand_color_func(), rand_color_func());
		glNormal3f(0, -1, 0);
		glNormal3f(0, 1, 0);
		glVertex3f(1, -1, 1);
		glVertex3f(-1, -1, 1);
		glVertex3f(-1, -1, -1);
		glEnd();
		glBegin(GL_TRIANGLES);
		glColor3f(rand_color_func(), rand_color_func(), rand_color_func());
		glNormal3f(0, -1, 0);
		glVertex3f(1, -1, -1);
		glVertex3f(1, -1, 1);
		glVertex3f(-1, -1, -1);
		glEnd();
	}

public:
	void transform()
	{
		glPushMatrix();
		displayCube();
		glRotatef(1, 1, 1, 0);
		GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
		GLfloat mat_shininess[] = {3.0};
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
		glPopMatrix();
	}
} cube;

void lighting_init(void)
{
	GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
}

void reshape_function(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-3, 3, -3 * (GLfloat)h / (GLfloat)w,
				3 * (GLfloat)h / (GLfloat)w, -10.0, 10.0);
	else
		glOrtho(-3 * (GLfloat)w / (GLfloat)h,
				3 * (GLfloat)w / (GLfloat)h, -3, 3, -10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void display_function()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//gluLookAt(1, 0, 0, 1, 2, 1, 0, 1, 0);
	cube.transform();
	glFlush();
}

void timer_function(int t)
{
	THETA = (THETA+1)%360;
	glutPostRedisplay();
	glutTimerFunc(17, timer_function, t);
}

int main(int argc, char *argv[])
{
	srand(time(NULL));
	glutInit(&argc, argv);
	glutInitWindowSize(800, 800);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("OpenGL Assignment");
	lighting_init();
	glutDisplayFunc(display_function);
	glutReshapeFunc(reshape_function);
	glutTimerFunc(17, timer_function, 1);
	glutMainLoop();
	return 0;
}