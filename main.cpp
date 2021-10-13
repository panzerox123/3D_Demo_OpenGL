#include<gl/freeglut.h>
#include<iostream>

float THETA = 0;
class Cube {
	void displayCube() {
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_POLYGON);
		glNormal3f(0, 0, 1);
		glVertex3f(-1.0, -1.0, 1.0);
		glVertex3f(1.0, -1.0, 1.0);
		glVertex3f(1.0, 1.0, 1.0);
		glVertex3f(-1.0, 1.0, 1.0);
		glEnd();

		//right face
		glColor3f(0.0f, 1.0f, 0.0f);
		glBegin(GL_POLYGON);
		glNormal3f(1, 0, 0);
		glVertex3f(1, -1, 1);
		glVertex3f(1, 1, 1);
		glVertex3f(1, 1, -1);
		glVertex3f(1, -1, -1);
		glEnd();

		//back face
		glColor3f(0.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		glNormal3f(0, 0, -1);
		glVertex3f(-1, -1, -1);
		glVertex3f(1, -1, -1);
		glVertex3f(1, 1, -1);
		glVertex3f(-1, 1, -1);
		glEnd();

		//back face
		glColor3f(1.0f, 1.0f, 0.0f);
		glBegin(GL_POLYGON);
		glNormal3f(-1, 0, 0);
		glVertex3f(-1, -1, 1);
		glVertex3f(-1, 1, 1);
		glVertex3f(-1, 1, -1);
		glVertex3f(-1, -1, -1);
		glEnd();

		//topface
		glColor3f(0.0f, 1.0f, 1.0f);
		glBegin(GL_POLYGON);
		glNormal3f(0, 1, 0);
		glVertex3f(1, 1, 1);
		glVertex3f(-1, 1, 1);
		glVertex3f(-1, 1, -1);
		glVertex3f(1, 1, -1);
		glEnd();

		//bottom face
		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		glNormal3f(0, -1, 0);
		glNormal3f(0, 1, 0);
		glVertex3f(1, -1, 1);
		glVertex3f(-1, -1, 1);
		glVertex3f(-1, -1, -1);
		glVertex3f(1, -1, -1);
		glEnd();
	}
public:
	void transform() {
		displayCube();
		glRotatef(THETA, 1, 1, 0);
	}
} cube;

void opengl_init() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2, 2, -2, 2, -2, 4);
	glPointSize(3);
}

void display_function() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cube.transform();
	glFlush();
}

void timer_func(int t) {
	if (THETA < 360) THETA = THETA + 0.01;
	else THETA = 0;
	std::cout << THETA << std::endl;
	glutPostRedisplay();
	glutTimerFunc(34, timer_func, t);
}

int main(int argc, char * argv[]) {
	glutInit(&argc, argv);
	glutInitWindowSize(800, 800);
	glutInitDisplayMode(GLUT_RGB);
	glutCreateWindow("OpenGL Assignment");
	glClearColor(1, 1, 1, 0);
	glutDisplayFunc(display_function);
	glutTimerFunc(17, timer_func, 1);
	opengl_init();
	glutMainLoop();
	return 0;
}