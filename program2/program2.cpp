/* Very stripped down code to start working on the virtual trackball -ZJW
	creates a 3d window and partial 3d wireframe cube - includes an example of
	resetting a local matrix to the identity using the OpenGL stack (see the keyboard func.)
 */
#include <stdlib.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

#include <iostream>
#include <vector>
#include "Vector3D/Vector3D.h"

using namespace std;

int GW;
int GH;

#define MODE_NONE      0
#define MODE_TRANSLATE 1
#define MODE_ROTATE    2
#define MODE_SCALE     4
int mode;

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

/*an example of a simple data structure to store a 4x4 matrix */
GLfloat objectM[4][4] = {
	{1.0, 0.0, 0.0, 0.0},
	{0.0, 1.0, 0.0, 0.0},
	{0.0, 0.0, 1.0, 0.0},
	{0.0, 0.0, 0.0, 1.0}
};

GLfloat *trackballM = (GLfloat *)objectM;

GLfloat translateM[3] = {0.0, 0.0, 0.0};
GLfloat scaleM[3] = {1.0, 1.0, 1.0};

Vector3D startClick;
Vector3D endClick;

float cubeVertexes[9][3] = {
	{ 0.0,  0.0,  0.0}, // filler
	{-0.5, -0.5, -0.5}, //v1
	{ 0.5, -0.5, -0.5}, //v2
	{ 0.5,  0.5, -0.5}, //v3
	{-0.5,  0.5, -0.5}, //v4
	{-0.5,  0.5,  0.5}, //v5
	{ 0.5,  0.5,  0.5}, //v6
	{ 0.5, -0.5,  0.5}, //v7
	{-0.5, -0.5,  0.5}  //v8
};
float cubeColors[9][3] = {
	{0.0, 0.0, 0.0},  // filler
	{0.5, 0.5, 0.5}, //v1
	{1.0, 0.0, 0.0}, //v2
	{1.0, 1.0, 0.0}, //v3
	{0.0, 1.0, 0.0}, //v4
	{0.0, 1.0, 1.0}, //v5
	{0.95, 0.95, 0.95}, //v6
	{1.0, 0.0, 1.0}, //v7
	{0.0, 0.0, 1.0}  //v8
};


bool wireFrame = true;
/*incomplete drawing of a 3d wireframe cube - needs to be completed */
void drawcube() {
	if (wireFrame) {
		glBegin(GL_LINE_LOOP); {
			// original provided cube cube
			for (int i = 1; i <= 8; i++) {
				glColor3fv(cubeColors[i]); 
				glVertex3fv(cubeVertexes[i]);

			}

		} glEnd();

		glBegin(GL_LINES); {
			// 1 -> 4
			glColor3fv(cubeColors[1]); 
			glVertex3fv(cubeVertexes[1]);
			glColor3fv(cubeColors[4]); 
			glVertex3fv(cubeVertexes[4]);
			// 5 -> 8
			glColor3fv(cubeColors[5]); 
			glVertex3fv(cubeVertexes[5]);
			glColor3fv(cubeColors[8]); 
			glVertex3fv(cubeVertexes[8]);
			// 2 -> 7
			glColor3fv(cubeColors[2]); 
			glVertex3fv(cubeVertexes[2]);
			glColor3fv(cubeColors[7]); 
			glVertex3fv(cubeVertexes[7]);
			// 3 -> 6
			glColor3fv(cubeColors[3]); 
			glVertex3fv(cubeVertexes[3]);
			glColor3fv(cubeColors[6]); 
			glVertex3fv(cubeVertexes[6]);
		} glEnd();
	}
	else {
		glBegin(GL_QUADS); {
			// Front Face
			for (int i = 1; i <= 4; i++) {
				glColor3fv(cubeColors[i]);
				glVertex3fv(cubeVertexes[i]);
			}
			// Back Face
			for (int i = 5; i <= 8; i++) {
				glColor3fv(cubeColors[i]);
				glVertex3fv(cubeVertexes[i]);
			}

			// left face
			for (int i = 1; i <= 5; i+=4) {
				glColor3fv(cubeColors[i]);
				glVertex3fv(cubeVertexes[i]);
				glColor3fv(cubeColors[i+3]);
				glVertex3fv(cubeVertexes[i+3]);
			}
			// right face
			for (int i = 2; i <= 6; i+=4) {
				glColor3fv(cubeColors[i]);
				glVertex3fv(cubeVertexes[i]);
				glColor3fv(cubeColors[i+1]);
				glVertex3fv(cubeVertexes[i+1]);
			}
			//bottom face
			for (int i = 1; i <=7; i+=6) {
				glColor3fv(cubeColors[i]);
				glVertex3fv(cubeVertexes[i]);
				glColor3fv(cubeColors[i+1]);
				glVertex3fv(cubeVertexes[i+1]);
			}
			//top face
			for (int i = 3; i <= 6; i++) {
				glColor3fv(cubeColors[i]);
				glVertex3fv(cubeVertexes[i]);
			}

		} glEnd();
	}	
}

bool axesOn = true;
// Draws the axes of the world.  Should be run before any transformations
void drawAxes() {
	GLfloat oldWidth;
	glGetFloatv(GL_LINE_WIDTH, &oldWidth);
	glLineWidth(3.0);
	// X axis
	glBegin(GL_LINES); {
		glColor3f(0.5, 0, 0);
		glVertex3f(-1.0, 0, 0);
		glVertex3f(1.0,0,0);
	} glEnd();

	// Y axis
	glBegin(GL_LINES); {
		glColor3f(0, 0.5, 0);
		glVertex3f(0, -1.0, 0);
		glVertex3f(0,1.0,0);
	} glEnd();

	// Z axis
	glBegin(GL_LINES); {
		glColor3f(0, 0, .5);
		glVertex3f(0, 0, -1.0);
		glVertex3f(0,0,1.0);
	} glEnd();

	glLineWidth(oldWidth);
}

void display() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	//add any transforms
	//delete the two rotation transforms - they are included 
	// just to show that the cube is partial and 3D
	glLoadIdentity();
	if (axesOn)
		drawAxes();


	glPushMatrix(); {
		glTranslatef(translateM[0], translateM[1], translateM[2]);
		glScalef(scaleM[0], scaleM[1], scaleM[2]);
		glMultMatrixf(trackballM);
		drawcube();
	} glPopMatrix();

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
			mode = MODE_ROTATE;
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
		if (state == GLUT_DOWN) { /* if the left button is clicked */
			printf("right mouse clicked at %d %d\n", x, y);
			mode = MODE_TRANSLATE;

			lastMouseX = x;
			lastMouseY = y;
		} 
		if (state == GLUT_UP) {

		}

	}
	// Zoom (scale)
	if (button == GLUT_MIDDLE_BUTTON) {
		if (state == GLUT_DOWN) { /* if the middle button is clicked */
			printf("middle mouse clicked at %d %d\n", x, y);
			mode = MODE_SCALE;
			lastMouseX = x;
		} 
		if (state == GLUT_UP) {

		}
	}
}

void mouseMove(int x, int y) {
	//printf("mouse moved at %d %d\n", x, y);
	if (mode == MODE_TRANSLATE) {
		translateM[0] -= p2w_x(lastMouseX) - p2w_x(x);
		translateM[1] -= p2w_y(lastMouseY) - p2w_y(y);
		lastMouseX = x;
		lastMouseY = y;
		glutPostRedisplay();
	}
	if (mode == MODE_SCALE) {
		if (lastMouseX > x) {
			// moving left, shrink
			scaleM[0] = scaleM[1] -= .01;
			if (scaleM[0] <= 0.1) {
				scaleM[0] = scaleM[1] = .1;
			}
		}
		else {
			//moving right, grow
			scaleM[0] = scaleM[1] += .01;
			if (scaleM[0] >= 3.0) {
				scaleM[0] = scaleM[1] = 3.0;
			}
		}
		lastMouseX = x;
		glutPostRedisplay();
	}
	if (mode == MODE_ROTATE) {
		endClick = Vector3D(p2w_x(x), p2w_y(y), 0);
		endClick.scaleToOne();
		endClick.bindZ();
		Vector3D axisRot = startClick.crossProd(endClick);
		float angleInDeg = rad2deg(findAngle(startClick, endClick));
		cout << "start: " << startClick <<  " length: " << startClick.length() << endl;
		cout << "end: " << endClick <<  " length: " << endClick.length() << endl;
		cout << "axis: " << axisRot << endl;
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
}


void keyboard(unsigned char key, int x, int y )
{
	switch( key ) {
	case 'a': case 'A' :
		axesOn = !axesOn;
		glutPostRedisplay();
		break;

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
		scaleM[0] = scaleM[1] = scaleM[2] = 1.0;
		translateM[0] = translateM[1] = translateM[2] = 0.0;
		glutPostRedisplay();
		break;
	case 'm': case 'M' :
		wireFrame = !wireFrame;
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



