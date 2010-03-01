//basic program to read in a mesh file (of .m format from H. Hoppe)
//Hh code modified by ZJW for csc 471

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <GL/glut.h>
#include "Vector3D/Vector3D.h"

#include "myModels.h"

using namespace std;

#define FLT_MIN 1.1754E-38F
#define FLT_MAX 1.1754E+38F

// These all ripped from program2 for trackball functionality

int GW, GH;

#define WorldW 4.0 *(float)GW / (float)GH
#define WorldH 4.0
inline float p2w_x(float w) {
   //float WorldW = 2 * (float) GW / (float) GH;
   return WorldW / GW * w - WorldW / 2;
}

inline float p2w_y(float h) {
   return -1 * (WorldH / GH * h - WorldH / 2);
}

inline float deg2rad(int deg) {
   return M_PI * deg / 180.0;
}

inline float rad2deg(float rad) {
   return rad * 180.0 / M_PI;
}

float findAngle(Vector3D &a, Vector3D &b) {
   return acos(a.dotProd(b) / (fabs(a.length()) * fabs(b.length())));
}

// this is modified from the simple_light provided files

GLfloat light_pos[4] = {1.0, 1.0, 1.5, 1.0};
GLfloat light_amb[4] = {0.6, 0.6, 0.6, 1.0};
GLfloat light_diff[4] = {0.6, 0.6, 0.6, 1.0};
GLfloat light_spec[4] = {0.8, 0.8, 0.8, 1.0};


typedef struct materialStruct {
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat shininess[1];
} materialStruct;

#define MATERIAL_COUNT 2
#define MATERIAL_REDFLAT 0
#define MATERIAL_GREENSHINY 1
materialStruct materials[MATERIAL_COUNT] = {
	{
		{0.3, 0.0, 0.0, 1.0},
		{0.9, 0.0, 0.0, 1.0},
		{0.0, 0.0, 0.0, 1.0},
		{0.0}
	},
	{
		{0.0, 0.3, 0.0, 1.0},
		{0.0, 0.9, 0.0, 1.0},
		{0.2, 1.0, 0.2, 1.0},
		{8.0}
	}
};

int current_material = MATERIAL_REDFLAT;

/*an example of a simple data structure to store a 4x4 matrix */
// Now to be used to determine the current direction of the camera?
GLfloat objectM[4][4] = {
   {1.0, 0.0, 0.0, 0.0},
   {0.0, 1.0, 0.0, 0.0},
   {0.0, 0.0, 1.0, 0.0},
   {0.0, 0.0, 0.0, 1.0}
};
GLfloat *trackballM = (GLfloat *)objectM;

Vector3D startClick, endClick;

int shading_mode = GL_FLAT;

//forward declarations of functions
void drawObjects();
void display();

void init_lighting() {
	//turn on light0
	glEnable(GL_LIGHT0);
	//set up the diffuse, ambient and specular components for the light
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diff);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_spec);
	//specify our lighting model as 1 normal per face
	glShadeModel(GL_FLAT);
}

void pos_light() {
	//set the light's position
	glMatrixMode(GL_MODELVIEW);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
}


void setMaterial(materialStruct material) {
	glMaterialfv(GL_FRONT, GL_AMBIENT, material.ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material.diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material.specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, material.shininess);
}

void reshape(int w, int h) {
  GW = w;
  GH = h;
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(90, (double)w/(double)h, 1.0, 15.0);
  glMatrixMode(GL_MODELVIEW);
  glViewport(0, 0, w, h);
  
  glutPostRedisplay();
}

void drawObjects() {
	drawPlane();
	drawAxes();
}

void display() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	//set up the camera
	//gluLookAt(1, 1, 3.0, 0, 1, 1, 0, 1, 0);
	gluLookAt(0, 1, 3.0, 0, 0, 0, 0, 1, 0);
	

	pos_light();

	drawObjects();

	glPopMatrix();

	glutSwapBuffers();

}

int lastMouseX, lastMouseY;

void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) { /* if the left button is clicked */
			printf("left mouse clicked at %d %d\n", x, y);
			startClick = Vector3D(p2w_x(x), p2w_y(y),0);
			startClick.scaleToOne();
			startClick.bindZ();
			endClick = Vector3D(p2w_x(x), p2w_y(y),0);
			endClick.scaleToOne();
			endClick.bindZ();
		}
		if (state == GLUT_UP) {
		}
	}
	// Move (translate)
	if (button == GLUT_RIGHT_BUTTON) {
	}
	if (state == GLUT_UP) {
	}

}

void mouseMove(int x, int y) {
	endClick = Vector3D(p2w_x(x), p2w_y(y), 0);
	endClick.scaleToOne();
	endClick.bindZ();
	Vector3D axisRot = startClick.crossProd(endClick);
	float angleInDeg = rad2deg(findAngle(startClick, endClick));
	//cout << "start: " << startClick <<  " length: " << startClick.length() <<
	//	cout << "end: " << endClick <<  " length: " << endClick.length() << endl;
	//cout << "axis: " << axisRot << endl;
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); {
		glLoadIdentity();
		glRotatef(angleInDeg, axisRot.getX(), axisRot.getY(), axisRot.getZ());
		glMultMatrixf(trackballM);
		glGetFloatv(GL_MODELVIEW_MATRIX, trackballM);
	} glPopMatrix();
	startClick = endClick;
	glutPostRedisplay();
}



void keyboard(unsigned char key, int x, int y) {
	switch( key) {
	case 'r' : case 'R' :
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glGetFloatv(GL_MODELVIEW_MATRIX, trackballM);
		glPopMatrix();
		glutPostRedisplay();
		break;
	case 'w': case 'W':
		// forward
		break;
	case 'a': case 'A':
		// left strafe
		break;
	case 's': case 'S':
		// back
		break;
	case 'd': case 'D':
		// right strafe
		break;
	case 'q': case 'Q':
		exit(0);
		break;
	}
	glutPostRedisplay();
}

int main( int argc, char** argv ) {

	//set up my window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(300, 300); 
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Mesh display");
	glClearColor(1.0, 1.0, 1.0, 1.0);

	//register glut callback functions
	glutDisplayFunc( display );
	glutReshapeFunc( reshape );
	glutKeyboardFunc( keyboard );
	glutMouseFunc( mouse );
	glutMotionFunc( mouseMove );

	//enable z-buffer
	glEnable(GL_DEPTH_TEST);

	init_lighting();

	glEnable(GL_NORMALIZE);
	glutMainLoop();
}


