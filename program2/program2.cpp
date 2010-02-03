/* Very stripped down code to start working on the virtual trackball -ZJW
	creates a 3d window and partial 3d wireframe cube - includes an example of
	resetting a local matrix to the identity using the OpenGL stack (see the keyboard func.)
 */
#include <stdlib.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

#include <vector>
#include "Vector3D/Vector3D.h"

int GW;
int GH;

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

/*an example of a simple data structure to store a 4x4 matrix */
GLfloat objectM[4][4] = {
	{1.0, 0.0, 0.0, 0.0},
	{0.0, 1.0, 0.0, 0.0},
	{0.0, 0.0, 1.0, 0.0},
	{0.0, 0.0, 0.0, 1.0}
};

GLfloat *trackballM = (GLfloat *)objectM;

Vector3D startClick;
Vector3D endClick;

/*incomplete drawing of a 3d wireframe cube - needs to be completed */
void drawcube() {
	glBegin(GL_LINE_LOOP);
	glColor3f(0.5, 0.5, 0.5); 
	glVertex3f(-0.5, -0.5, -0.5); //v1

	glColor3f(1.0, 0.0, 0.0); //red
	glVertex3f(0.5, -0.5, -0.5); //v2

	glColor3f(1.0, 1.0, 0.0); //r-g
	glVertex3f(0.5, 0.5, -0.5); //v3

	glColor3f(0.0, 1.0, 0.0); //green
	glVertex3f(-0.5, 0.5, -0.5); //v4

	glColor3f(0.0, 1.0, 1.0); //g-b
	glVertex3f(-0.5, 0.5, 0.5); //v5

	glColor3f(0.95, 0.95, 0.95); 
	glVertex3f(0.5, 0.5, 0.5); //v6

	glColor3f(1.0, 0.0, 1.0);
	glVertex3f(0.5, -0.5, 0.5); //v7

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(-0.5, -0.5, 0.5); //v8
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0.0, 0.0, 0.0); //white
	glVertex3f(-0.5, -0.5, -0.5); //v1
	glColor3f(0.0, 1.0, 0.0); //green
	glVertex3f(-0.5, 0.5, -0.5);  //v4
	glEnd();

}

void display() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	//add any transforms
	//delete the two rotation transforms - they are included 
	// just to show that the cube is partial and 3D
	glLoadIdentity();
	glRotatef(-45,  0, 1, 0);
	glRotatef(45, 1, 0, 0);
	drawcube();

	glutSwapBuffers();

}

/*note that we have made our world 2ce as big */
void reshape(int w, int h) {
	GW = w;
	GH = h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho( -2*(float)w/h, (float)2*w/h, -2, 2, -2, 2); 
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, w, h);

}

int lastMouseX;
int lastMouseY;


void mouse(int button, int state, int x, int y) {
	// Rotate
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) { /* if the left button is clicked */
			printf("left mouse clicked at %d %d\n", x, y);
			lastMouseX = x;
			lastMouseY = y;
			startClick = Vector3D(p2w_x(x), p2w_y(y),0);
			endClick = Vector3D(p2w_x(x), p2w_y(y),0);
		} 
		if (state == GLUT_UP) {

		}
	}
	// Move (translate)
	if (button == GLUT_RIGHT_BUTTON) {
		if (state == GLUT_DOWN) { /* if the left button is clicked */
			printf("right mouse clicked at %d %d\n", x, y);
			lastMouseX = x;
			lastMouseY = y;
			startClick = Vector3D(p2w_x(x), p2w_y(y),0);
			endClick = Vector3D(p2w_x(x), p2w_y(y),0);
		} 
		if (state == GLUT_UP) {

		}

	}
	// Zoom (scale)
	if (button == GLUT_MIDDLE_BUTTON) {
		if (state == GLUT_DOWN) { /* if the left button is clicked */
			printf("middle mouse clicked at %d %d\n", x, y);
			lastMouseX = x;
			lastMouseY = y;
			startClick = Vector3D(p2w_x(x), p2w_y(y),0);
			endClick = Vector3D(p2w_x(x), p2w_y(y),0);
		} 
		if (state == GLUT_UP) {

		}
	}
}

void mouseMove(int x, int y) {
	printf("mouse moved at %d %d\n", x, y);
	endClick = Vector3D(p2w_x(x), p2w_y(y), 0);
	glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y )
{
	switch( key ) {
	case 'q': case 'Q' :
		exit( EXIT_SUCCESS );
		break;
		/*how to reset our local matrix to the identity using the matrix stacks */
	case 'r' : case 'R' :
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glGetFloatv(GL_MODELVIEW_MATRIX, trackballM);
		glPopMatrix();
		glutPostRedisplay();
		break;
	}
}


int main(int argc, char** argv) {

	//initialize  globals
	GW = 400;
	GH = 400;
	glutInit(&argc, argv);
	glutInitWindowSize(400, 400);
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("My First 3d Window");
	/*enable depth testing for rendering 3d polygons */
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	//register callback function */
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMove);
	glutKeyboardFunc(keyboard);
	//start the main loop
	glutMainLoop();

}



