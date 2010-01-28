//A very simple opengl/GLUT program
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "Vector3D/Vector3D.h"
#include <vector>

using namespace std;
//global variables - necessary evil when using GLUT
//global width and height
int GW;
int GH;
//current mouse position in pixel coordinate
//int x;
//int y;

// current rotation and position of triangle

//#define WorldW 2.0
#define WorldW 2 *(float)GW / (float)GH
#define WorldH 2.0
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

vector<Vector3D> vectors;

//the display call back - all drawing should be done in this function
void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < vectors.size(); i++) {
		vectors[i].draw();
	}

	glutSwapBuffers();
}

int lastMouseX;
int lastMouseY;

//the mouse button callback
void mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON) {
    if (state == GLUT_DOWN) { /* if the left button is clicked */
		 printf("mouse down\n");
		 vectors.push_back(Vector3D(0,0,0,p2w_x(x),p2w_y(y),0));
		 lastMouseX = x;
		 lastMouseY = y;
		 glutPostRedisplay();
    }
	 if (state == GLUT_UP) {
		 printf("Length of last vector: %f\n", vectors.back().length());
	 }
  }
}

//the mouse move callback
void mouseMove(int x, int y) {
  //printf("mouse moved at %d %d\n", x, y);
  vectors.back().operator-=(Vector3D(p2w_x(lastMouseX) - p2w_x(x), 
			                            p2w_y(lastMouseY) - p2w_y(y),
												 0));
  lastMouseX = x;
  lastMouseY = y;
  glutPostRedisplay();
}

//the keyboard callback
void keyboard(unsigned char key, int x, int y ){
  switch( key ) {
    case 'q': case 'Q' :
      exit( EXIT_SUCCESS );
      break;
    case 'c' : case 'C' :
      glutPostRedisplay();
      break;
  }
}

void reshape( GLsizei w, GLsizei h) {
   GW = w;
   GH = h;
   printf("new window size: %d, %d\n", w, h);
   glViewport(0,0,w,h); // set viewport
   glLoadIdentity(); // ?
   gluOrtho2D(-(float)w/h, (float)w/h, -1, 1);
}

int main( int argc, char** argv ){

	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE);
  //intializations
	glutInitWindowSize(200, 200);
	glutInitWindowPosition(100, 100);
	glutCreateWindow( "My Third Window" );
	glClearColor(1.0, 1.0, 1.0, 1.0);
  //global variable intialization
  GW = GH = 200;
  //register the callback functions
	glutDisplayFunc( display );
  glutMouseFunc( mouse );
  glutMotionFunc( mouseMove );
  glutKeyboardFunc( keyboard );
  glutReshapeFunc( reshape );

	glutMainLoop();
}
