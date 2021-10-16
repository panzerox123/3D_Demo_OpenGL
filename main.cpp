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
int RADIUS = 5;
float ZOOM = 10;
int WIDTH = 0;
int HEIGHT = 0;
bool MOUSE_RIGHT_HELD_DOWN = false;
int MOUSE_X = 0;
int MOUSE_Y = 0;
int UP_FLAG = 1;

bool cube_flag = false;

float rand_color_func()
{
	return (float)rand() / (float)RAND_MAX;
}

class Cube
{
	bool pause;
	int rotate_angle;
	float rand_colors[12][3];
	int x_rotate = 0;
	int y_rotate = 0;
	int z_rotate = 0;
	float x_translate = 0;
	float y_translate = 0;
	float z_translate = 0;
	int x_dir = 0;
	int y_dir = 0;
	int z_dir = 0;
	void display()
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

	void randomize()
	{
		srand(time(NULL));
		x_rotate = rand() % 2;
		y_rotate = rand() % 2;
		z_rotate = rand() % 2;
		x_dir = rand() % 3 - 1;
		y_dir = rand() % 3 - 1;
		z_dir = rand() % 3 - 1;
	}

	void transforms()
	{
		glRotatef(rotate_angle, x_rotate, y_rotate, z_rotate);
		glTranslatef(x_translate, y_translate, z_translate);
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
		pause = false;
		randomize();
	}

	void pause_trans()
	{
		pause = true;
	}

	void unpause_trans()
	{
		pause = false;
	}

	void increment_trans()
	{
		if(pause) return;
		rotate_angle = (rotate_angle + 1) % 360;
		if (abs(x_translate) >= RADIUS || abs(y_translate) >= RADIUS || abs(z_translate) >= RADIUS)
		{
			if (abs(x_translate) >= 5)
			{
				x_translate = x_dir*RADIUS;
				x_dir = -x_dir;
			}
			if (abs(y_translate) >= 5)
			{
				y_translate = y_dir*RADIUS;
				y_dir = -y_dir;
			}
			if (abs(z_translate) >= 5)
			{
				z_translate = z_dir*RADIUS;
				z_dir = -z_dir;
			}
			//randomize();
		}
		if(x_dir) x_translate += x_dir*0.1;
		if(y_dir) y_translate += y_dir*0.1;
		if(z_dir) z_translate += z_dir*0.1;
	}

	void run()
	{
		glPushMatrix();
		//glRotatef(this->rotate_angle, 1, 0, 0);
		transforms();
		GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
		GLfloat mat_shininess[] = {3.0};
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
		display();
		glPopMatrix();
	}
} cube;

void drag_camera()
{
	//std::cout << MOUSE_RIGHT_HELD_DOWN << std::endl;
	if (MOUSE_RIGHT_HELD_DOWN)
	{
		AZIMUTH += (float)(MOUSE_X - mouse_state_x) / WIDTH;
		INCLININATION += (float)(MOUSE_Y - mouse_state_y) / HEIGHT;
	}
}

void buffer_init(void)
{
	GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearStencil(0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (int(INCLININATION) % 360 >= 90 && int(INCLININATION) % 360 <= 270)
		UP_FLAG = -1;
	else
		UP_FLAG = 1;
	//std::cout << UP_FLAG << std::endl;
	// Print camera/up_flag to fix gimball lock
	gluLookAt(CAM_X, CAM_Y, CAM_Z, 0, 0, 0, 0, UP_FLAG, 0);
	glStencilFunc(GL_ALWAYS, 1, 0xFFFF);
	cube.run();
	glutWireSphere(10, 25, 25);
	glFlush();
}

void timer_function(int t)
{
	// Camera Control
	float Y_DIS = 1 * cos((double)INCLININATION * PI / 180);
	CAM_Y = 1 * sin((double)INCLININATION * PI / 180);
	CAM_X = Y_DIS * cos((double)AZIMUTH * PI / 180);
	CAM_Z = Y_DIS * sin((double)AZIMUTH * PI / 180);
	drag_camera();
	glutPostRedisplay();
	cube.increment_trans();
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
	else if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
	{
		GLuint index;
		glReadPixels(x, y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);
		//std::cout << index << std::endl;
		switch (index)
		{
		case 1:
			if (cube_flag)
				cube.unpause_trans();
			else
				cube.pause_trans();
			cube_flag = !cube_flag;
		default:
			break;
		}
	}
}

void mouse_drag_motion_function(int x, int y)
{
	MOUSE_X = x;
	MOUSE_Y = y;
	//std::cout << MOUSE_X << " " << MOUSE_Y << std::endl;
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
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
	glutCreateWindow("OpenGL Assignment");
	buffer_init();
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