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
float ZOOM = 12;
int WIDTH = 0;
int HEIGHT = 0;
bool MOUSE_RIGHT_HELD_DOWN = false;
int MOUSE_X = 0;
int MOUSE_Y = 0;
int UP_FLAG = 1;

bool toggle_wireframe = true;
bool cube_flag = false;
bool dod_flag = false;
bool hex_flag = false;

float rand_color_func()
{
	return (float)rand() / (float)RAND_MAX;
}

double * calc_normal(double * a, double * b, double * c){
	double i1 = b[0]-a[0];
	double j1 = b[1]-a[1];
	double k1 = b[2]-a[2];
	double i2 = c[0]-b[0];
	double j2 = c[1]-b[1];
	double k2 = c[2]-b[2];
	double * d = (double *)malloc(3*sizeof(double));
	d[0] = j1*k2 - k1*j2;
	d[1] = i1*k2 - k1*i2;
	d[2] = i1*j2 - j1*i2;
	return d;
}

class HexagonalPrism
{
	bool pause;
	int rotate_angle;
	float rand_colors[40][3];
	int x_rotate = 0;
	int y_rotate = 0;
	int z_rotate = 0;
	float x_translate = 0;
	float y_translate = 0;
	float z_translate = 0;
	int x_dir = 0;
	int y_dir = 0;
	int z_dir = 0;
	float scale_factor = 1;

	int hex_faces[2][6] =
		{
			{0,1,2,3,4,5},
			{6,7,8,9,10,11},	
		};

	int sqr_faces[6][4] = {
		{0,6,7,1},
		{7,1,2,8},
		{8,2,3,9},
		{9,3,4,10},
		{10,4,5,11},
		{11,6,5,0}
	};

	double hex_points[12][3] =
		{
			{0,2,1},
			{1,1,1},
			{1,-1,1},
			{0,-2,1},
			{-1,-1,1},
			{-1,1,1},
			{0,2,-1},
			{1,1,-1},
			{1,-1,-1},
			{0,-2,-1},
			{-1,-1,-1},
			{-1,1,-1},
	
		};

	void display()
	{
		for(int i = 0; i < 2; i++){
			glBegin(GL_TRIANGLE_FAN);
			glNormal3fv((GLfloat *)calc_normal(hex_points[hex_faces[i][0]],hex_points[hex_faces[i][1]],hex_points[hex_faces[i][2]]));
			for(int j = 0; j < 6; j++){
				glVertex3dv(hex_points[hex_faces[i][j]]);
				glColor3fv(rand_colors[i]);
			}
			glEnd();
		}
		for(int i = 0; i < 6; i++){
			glBegin(GL_TRIANGLE_FAN);
			glNormal3fv((GLfloat *)calc_normal(hex_points[sqr_faces[i][0]],hex_points[sqr_faces[i][1]],hex_points[sqr_faces[i][2]]));
			for(int j = 0; j < 4; j++){
				glVertex3dv(hex_points[sqr_faces[i][j]]);
				glColor3fv(rand_colors[i*j+2]);
			}
			glEnd();
		}
	}

	void randomize()
	{
		srand(time(NULL)+10);
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
		glScalef(scale_factor, scale_factor, scale_factor);
	}

public:
	HexagonalPrism()
	{
		rotate_angle = 0;
		for (int i = 0; i < 40; i++)
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

	void increment_x()
	{
		if (x_translate < RADIUS)
			x_translate += 0.1;
	}

	void decrement_x()
	{
		if (x_translate > -RADIUS)
			x_translate -= 0.1;
	}

	void increment_y()
	{
		if (y_translate < RADIUS)
			y_translate += 0.1;
	}

	void decrement_y()
	{
		if (y_translate > -RADIUS)
			y_translate -= 0.1;
	}

	void increment_z()
	{
		if (z_translate < RADIUS)
			z_translate += 0.1;
	}

	void decrement_z()
	{
		if (z_translate > -RADIUS)
			z_translate -= 0.1;
	}

	void increment_r()
	{
		rotate_angle = (rotate_angle + 1) % 360;
	}

	void decrement_r()
	{
		rotate_angle = (rotate_angle - 1) % 360;
	}

	void increment_s()
	{
		if (scale_factor < 5)
			scale_factor += 0.1;
	}

	void decrement_s()
	{
		if (scale_factor > 0.1)
			scale_factor -= 0.1;
	}

	void increment_trans()
	{
		if (pause)
			return;
		rotate_angle = (rotate_angle + 1) % 360;
		if (abs(x_translate) >= RADIUS || abs(y_translate) >= RADIUS || abs(z_translate) >= RADIUS)
		{
			if (abs(x_translate) >= 5)
			{
				x_translate = x_dir * RADIUS;
				x_dir = -x_dir;
			}
			if (abs(y_translate) >= 5)
			{
				y_translate = y_dir * RADIUS;
				y_dir = -y_dir;
			}
			if (abs(z_translate) >= 5)
			{
				z_translate = z_dir * RADIUS;
				z_dir = -z_dir;
			}
			//randomize();
		}
		if (x_dir)
			x_translate += x_dir * 0.1;
		if (y_dir)
			y_translate += y_dir * 0.1;
		if (z_dir)
			z_translate += z_dir * 0.1;
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
} hex;

class Dodecahedron
{
	bool pause;
	int rotate_angle;
	float rand_colors[40][3];
	int x_rotate = 0;
	int y_rotate = 0;
	int z_rotate = 0;
	float x_translate = 0;
	float y_translate = 0;
	float z_translate = 0;
	int x_dir = 0;
	int y_dir = 0;
	int z_dir = 0;
	float scale_factor = 1;

	int dod_faces[12][5] =
		{
			{0, 16, 2, 10, 8},
			{0, 8, 4, 14, 12},
			{16, 17, 1, 12, 0},
			{1, 9, 11, 3, 17},
			{1, 12, 14, 5, 9},
			{2, 13, 15, 6, 10},
			{13, 3, 17, 16, 2},
			{3, 11, 7, 15, 13},
			{4, 8, 10, 6, 18},
			{14, 5, 19, 18, 4},
			{5, 19, 7, 11, 9},
			{15, 7, 19, 18, 6}};

	double dod_points[20][3] =
		{
			{1, 1, 1},
			{1, 1, -1},
			{1, -1, 1},
			{1, -1, -1},
			{-1, 1, 1},
			{-1, 1, -1},
			{-1, -1, 1},
			{-1, -1, -1},
			{0, 0.618, 1.618},
			{0, 0.618, -1.618},
			{0, -0.618, 1.618},
			{0, -0.618, -1.618},
			{0.618, 1.618, 0},
			{0.618, -1.618, 0},
			{-0.618, 1.618, 0},
			{-0.618, -1.618, 0},
			{1.618, 0, 0.618},
			{1.618, 0, -0.618},
			{-1.618, 0, 0.618},
			{-1.618, 0, -0.618}};

	void display()
	{
		for (int i = 0; i < 12; i++)
		{
			//glColor3f(0.0, 0.1, 0.1);
			glBegin(GL_TRIANGLE_FAN);
			glNormal3fv((GLfloat *)calc_normal(dod_points[dod_faces[i][0]],dod_points[dod_faces[i][1]],dod_points[dod_faces[i][2]]));
			for (int j = 0; j < 5; j++){
				glVertex3dv(dod_points[dod_faces[i][j]]);
				glColor3fv(rand_colors[j*i]);
			}
			glEnd();
			//glColor3f(0.0, 1.0, 1.0);

		}
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
		glScalef(scale_factor, scale_factor, scale_factor);
	}

public:
	Dodecahedron()
	{
		rotate_angle = 0;
		for (int i = 0; i < 40; i++)
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

	void increment_x()
	{
		if (x_translate < RADIUS)
			x_translate += 0.1;
	}

	void decrement_x()
	{
		if (x_translate > -RADIUS)
			x_translate -= 0.1;
	}

	void increment_y()
	{
		if (y_translate < RADIUS)
			y_translate += 0.1;
	}

	void decrement_y()
	{
		if (y_translate > -RADIUS)
			y_translate -= 0.1;
	}

	void increment_z()
	{
		if (z_translate < RADIUS)
			z_translate += 0.1;
	}

	void decrement_z()
	{
		if (z_translate > -RADIUS)
			z_translate -= 0.1;
	}

	void increment_r()
	{
		rotate_angle = (rotate_angle + 1) % 360;
	}

	void decrement_r()
	{
		rotate_angle = (rotate_angle - 1) % 360;
	}

	void increment_s()
	{
		if (scale_factor < 5)
			scale_factor += 0.1;
	}

	void decrement_s()
	{
		if (scale_factor > 0.1)
			scale_factor -= 0.1;
	}

	void increment_trans()
	{
		if (pause)
			return;
		rotate_angle = (rotate_angle + 1) % 360;
		if (abs(x_translate) >= RADIUS || abs(y_translate) >= RADIUS || abs(z_translate) >= RADIUS)
		{
			if (abs(x_translate) >= 5)
			{
				x_translate = x_dir * RADIUS;
				x_dir = -x_dir;
			}
			if (abs(y_translate) >= 5)
			{
				y_translate = y_dir * RADIUS;
				y_dir = -y_dir;
			}
			if (abs(z_translate) >= 5)
			{
				z_translate = z_dir * RADIUS;
				z_dir = -z_dir;
			}
			//randomize();
		}
		if (x_dir)
			x_translate += x_dir * 0.1;
		if (y_dir)
			y_translate += y_dir * 0.1;
		if (z_dir)
			z_translate += z_dir * 0.1;
	}

	void run()
	{
		glPushMatrix();
		//glRotatef(this->rotate_angle, 1, 0, 0);
		transforms();
		GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
		GLfloat mat_shininess[] = {50.0};
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
		display();
		glPopMatrix();
	}
} dod;

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
	float scale_factor = 1;
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
		srand(time(NULL)+5);
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
		glScalef(scale_factor, scale_factor, scale_factor);
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

	void increment_x()
	{
		if (x_translate < RADIUS)
			x_translate += 0.1;
	}

	void decrement_x()
	{
		if (x_translate > -RADIUS)
			x_translate -= 0.1;
	}

	void increment_y()
	{
		if (y_translate < RADIUS)
			y_translate += 0.1;
	}

	void decrement_y()
	{
		if (y_translate > -RADIUS)
			y_translate -= 0.1;
	}

	void increment_z()
	{
		if (z_translate < RADIUS)
			z_translate += 0.1;
	}

	void decrement_z()
	{
		if (z_translate > -RADIUS)
			z_translate -= 0.1;
	}

	void increment_r()
	{
		rotate_angle = (rotate_angle + 1) % 360;
	}

	void decrement_r()
	{
		rotate_angle = (rotate_angle - 1) % 360;
	}

	void increment_s()
	{
		if (scale_factor < 5)
			scale_factor += 0.1;
	}

	void decrement_s()
	{
		if (scale_factor > 0.1)
			scale_factor -= 0.1;
	}

	void increment_trans()
	{
		if (pause)
			return;
		rotate_angle = (rotate_angle + 1) % 360;
		if (abs(x_translate) >= RADIUS || abs(y_translate) >= RADIUS || abs(z_translate) >= RADIUS)
		{
			if (abs(x_translate) >= 5)
			{
				x_translate = x_dir * RADIUS;
				x_dir = -x_dir;
			}
			if (abs(y_translate) >= 5)
			{
				y_translate = y_dir * RADIUS;
				y_dir = -y_dir;
			}
			if (abs(z_translate) >= 5)
			{
				z_translate = z_dir * RADIUS;
				z_dir = -z_dir;
			}
			//randomize();
		}
		if (x_dir)
			x_translate += x_dir * 0.1;
		if (y_dir)
			y_translate += y_dir * 0.1;
		if (z_dir)
			z_translate += z_dir * 0.1;
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
	glShadeModel(GL_FLAT);
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
	if (abs(int(INCLININATION)) % 360 >= 90 && abs(int(INCLININATION)) % 360 <= 270)
		UP_FLAG = -1;
	else
		UP_FLAG = 1;
	//std::cout << UP_FLAG << std::endl;
	// Print camera/up_flag to fix gimball lock
	gluLookAt(CAM_X, CAM_Y, CAM_Z, 0, 0, 0, 0, UP_FLAG, 0);
	glStencilFunc(GL_ALWAYS, 1, 0xFFFF);
	cube.run();
	glStencilFunc(GL_ALWAYS, 2, 0xFFFF);
	dod.run();
	glStencilFunc(GL_ALWAYS, 3, 0xFFFF);
	hex.run();
	if (toggle_wireframe)
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
	dod.increment_trans();
	hex.increment_trans();
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
			break;
		case 2:
			if(dod_flag)
				dod.unpause_trans();
			else
				dod.pause_trans();
			dod_flag = !dod_flag;
			break;
		case 3:
			if(hex_flag)
				hex.unpause_trans();
			else
				hex.pause_trans();
			hex_flag = !hex_flag;
			break;
		default:
			break;
		}
	}
}

void custom_control_function(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'x':
		if (cube_flag)
			cube.increment_x();
		if (dod_flag)
			dod.increment_x();
		if (hex_flag)
			hex.increment_x();
		break;
	case 'X':
		if (cube_flag)
			cube.decrement_x();
		if (dod_flag)
			dod.decrement_x();
		if (hex_flag)
			hex.decrement_x();
		break;
	case 'y':
		if (cube_flag)
			cube.increment_y();
		if (dod_flag)
			dod.increment_y();
		if (hex_flag)
			hex.increment_y();
		break;
	case 'Y':
		if (cube_flag)
			cube.decrement_y();
		if (dod_flag)
			dod.decrement_y();
		if (hex_flag)
			hex.decrement_y();
		break;
	case 'z':
		if (cube_flag)
			cube.increment_z();
		if (dod_flag)
			dod.increment_z();
		if (hex_flag)
			hex.increment_z();
		break;
	case 'Z':
		if (cube_flag)
			cube.decrement_z();
		if (dod_flag)
			dod.decrement_z();
		if (hex_flag)
			hex.decrement_z();
		break;
	case 'r':
		if (cube_flag)
			cube.increment_r();
		if (dod_flag)
			dod.increment_r();
		if (hex_flag)
			hex.increment_r();
		break;
	case 'R':
		if (cube_flag)
			cube.decrement_r();
		if (dod_flag)
			dod.decrement_r();
		if (hex_flag)
			hex.decrement_r();
		break;
	case 's':
		if (cube_flag)
			cube.increment_s();
		if (dod_flag)
			dod.increment_s();
		if (hex_flag)
			hex.increment_s();
		break;
	case 'S':
		if (cube_flag)
			cube.decrement_s();
		if (dod_flag)
			dod.decrement_s();
		if (hex_flag)
			hex.decrement_s();
		break;
	case 't':
		toggle_wireframe = !toggle_wireframe;
		break;
	default:
		break;
	}
}

void mouse_drag_motion_function(int x, int y)
{
	MOUSE_X = x;
	MOUSE_Y = y;
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
	glutKeyboardFunc(custom_control_function);
	glutMouseFunc(mouse_control_function);
	//glutPassiveMotionFunc(mouse_motion_function);
	glutMotionFunc(mouse_drag_motion_function);
	//glutMouseWheelFunc(mouse_zoom_function);
	glutSpecialFunc(keyboard_control_function);
	glutTimerFunc(17, timer_function, 1);
	glutMainLoop();
	return 0;
}