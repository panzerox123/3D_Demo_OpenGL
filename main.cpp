#include <GL/freeglut.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define PI 3.14

int mouse_state_x = 0;
int mouse_state_y = 0;
double CAM_X = 1;
double CAM_Y = 1;
double CAM_Z = 1;
float INCLININATION = 0;
float AZIMUTH = 0;
int RADIUS = 1;
float ZOOM = 12;
int WIDTH = 0;
int HEIGHT = 0;
bool MOUSE_RIGHT_HELD_DOWN = false;
int MOUSE_X = 0;
int MOUSE_Y = 0;

float rand_color_func()
{
	return (float)rand() / (float)RAND_MAX;
}

class Cube
{
	int rotate_angle;
	float rand_colors[12][3];
	void displayCube()
	{
		glBegin(GL_TRIANGLES);
		glColor3fv(rand_colors[0]);
		glNormal3f(0, 0, 1);
		glVertex3f(-1.0, -1.0, 1.0);
		glVertex3f(1.0, -1.0, 1.0);
		glVertex3f(1.0, 1.0, 1.0);
		glEnd();
		glBegin(GL_TRIANGLES);
		glColor3fv(rand_colors[1]);
		glNormal3f(0, 0, 1);
		glVertex3f(-1.0, 1.0, 1.0);
		glVertex3f(1.0, 1.0, 1.0);
		glVertex3f(-1.0, -1.0, 1.0);
		glEnd();

		glBegin(GL_TRIANGLES);
		glColor3fv(rand_colors[2]);
		glNormal3f(1, 0, 0);
		glVertex3f(1, -1, 1);
		glVertex3f(1, 1, 1);
		glVertex3f(1, 1, -1);
		glBegin(GL_TRIANGLES);
		glColor3fv(rand_colors[3]);
		glNormal3f(1, 0, 0);
		glVertex3f(1, -1, -1);
		glVertex3f(1, 1, -1);
		glVertex3f(1, -1, 1);
		glEnd();

		glBegin(GL_TRIANGLES);
		glColor3fv(rand_colors[4]);
		glNormal3f(0, 0, -1);
		glVertex3f(-1, -1, -1);
		glVertex3f(1, -1, -1);
		glVertex3f(1, 1, -1);
		glEnd();
		glBegin(GL_TRIANGLES);
		glColor3fv(rand_colors[5]);
		glNormal3f(0, 0, -1);
		glVertex3f(-1, 1, -1);
		glVertex3f(1, 1, -1);
		glVertex3f(-1, -1, -1);
		glEnd();

		glBegin(GL_TRIANGLES);
		glColor3fv(rand_colors[6]);
		glNormal3f(-1, 0, 0);
		glVertex3f(-1, -1, 1);
		glVertex3f(-1, 1, 1);
		glVertex3f(-1, 1, -1);
		glEnd();
		glBegin(GL_TRIANGLES);
		glColor3fv(rand_colors[7]);
		glNormal3f(-1, 0, 0);
		glVertex3f(-1, -1, -1);
		glVertex3f(-1, 1, -1);
		glVertex3f(-1, -1, 1);
		glEnd();

		glBegin(GL_TRIANGLES);
		glColor3fv(rand_colors[8]);
		glNormal3f(0, 1, 0);
		glVertex3f(1, 1, 1);
		glVertex3f(-1, 1, 1);
		glVertex3f(-1, 1, -1);
		glEnd();
		glBegin(GL_TRIANGLES);
		glColor3fv(rand_colors[9]);
		glNormal3f(0, 1, 0);
		glVertex3f(1, 1, -1);
		glVertex3f(1, 1, 1);
		glVertex3f(-1, 1, -1);
		glEnd();

		//bottom face
		glBegin(GL_TRIANGLES);
		glColor3fv(rand_colors[10]);
		glNormal3f(0, -1, 0);
		glNormal3f(0, 1, 0);
		glVertex3f(1, -1, 1);
		glVertex3f(-1, -1, 1);
		glVertex3f(-1, -1, -1);
		glEnd();
		glBegin(GL_TRIANGLES);
		glColor3fv(rand_colors[11]);
		glNormal3f(0, -1, 0);
		glVertex3f(1, -1, -1);
		glVertex3f(1, -1, 1);
		glVertex3f(-1, -1, -1);
		glEnd();
	}

public:
	Cube()
	{
		rotate_angle = 0;
		for (int i = 0; i < 12; i++)
		{
			rand_colors[i][0] = rand_color_func();
			rand_colors[i][1] = rand_color_func();
			rand_colors[i][2] = rand_color_func();
		}
	}

	void increment_rotate_angle()
	{
		rotate_angle = (rotate_angle + 1) % 360;
	}

	void transform()
	{
		glPushMatrix();
		glRotatef(this->rotate_angle, 1, 0, 0);
		GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
		GLfloat mat_shininess[] = {3.0};
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
		displayCube();
		glPopMatrix();
	}
} cube;

void drag_camera()
{
	std::cout << MOUSE_RIGHT_HELD_DOWN << std::endl;
	if (MOUSE_RIGHT_HELD_DOWN)
	{
		AZIMUTH += (float)(MOUSE_X - mouse_state_x) / WIDTH;
		INCLININATION += (float)(MOUSE_Y - mouse_state_y) / HEIGHT;
	}
}

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
	WIDTH = w / 2;
	HEIGHT = h / 2;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-ZOOM, ZOOM, -ZOOM * (GLfloat)h / (GLfloat)w,
				ZOOM * (GLfloat)h / (GLfloat)w, -10.0, 10.0);
	else
		glOrtho(-ZOOM * (GLfloat)w / (GLfloat)h,
				ZOOM * (GLfloat)w / (GLfloat)h, -ZOOM, ZOOM, -10.0, 10.0);
	glPointSize(3);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void display_function()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(CAM_X, CAM_Y, CAM_Z, 0, 0, 0, 0, 1, 0);
	cube.transform();
	glutWireSphere(10,25,25);
	glFlush();
}

void timer_function(int t)
{
	// Camera Control
	float Y_DIS = RADIUS * cos((double)INCLININATION * PI / 180);
	CAM_Y = RADIUS * sin((double)INCLININATION * PI / 180);
	CAM_X = Y_DIS * cos((double)AZIMUTH * PI / 180);
	CAM_Z = Y_DIS * sin((double)AZIMUTH * PI / 180);
	drag_camera();
	glutPostRedisplay();
	//cube.increment_rotate_angle();
	glutTimerFunc(17, timer_function, t);
}

void keyboard_control_function(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_SHIFT_L:
		mouse_state_x = x - WIDTH / 2;
		mouse_state_y = y - HEIGHT / 2;
		break;

	case GLUT_KEY_UP:
		INCLININATION = (INCLININATION + 1);
		break;
	case GLUT_KEY_DOWN:
		INCLININATION = (INCLININATION - 1);
		break;
	case GLUT_KEY_LEFT:
		AZIMUTH = (AZIMUTH + 1);
		break;
	case GLUT_KEY_RIGHT:
		AZIMUTH = (AZIMUTH - 1);
		break;
	default:
		break;
	}
}

void mouse_control_function(int button, int action, int x, int y)
{
	if (button == GLUT_RIGHT_BUTTON && action == GLUT_DOWN)
	{
		mouse_state_x = x;
		mouse_state_y = y;
		MOUSE_RIGHT_HELD_DOWN = true;
	}
	else if (button == GLUT_RIGHT_BUTTON && action == GLUT_UP)
	{
		MOUSE_RIGHT_HELD_DOWN = false;
	}
}

void mouse_drag_motion_function(int x, int y)
{
	MOUSE_X = x;
	MOUSE_Y = y;
	std::cout << MOUSE_X << " " << MOUSE_Y << std::endl;
}

void mouse_zoom_function(int wheel, int direction, int x, int y)
{
	if (direction == 1)
	{
		if (ZOOM > 0.1)
			ZOOM -= 0.1;
	}
	else if (direction == -1)
	{
		ZOOM += 0.1;
	}
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
	glutMouseFunc(mouse_control_function);
	//glutPassiveMotionFunc(mouse_motion_function);
	glutMotionFunc(mouse_drag_motion_function);
	//glutMouseWheelFunc(mouse_zoom_function);
	glutSpecialFunc(keyboard_control_function);
	glutTimerFunc(17, timer_function, 1);
	glutMainLoop();
	return 0;
}