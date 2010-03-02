//A very simple opengl/GLUT program
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;
//global variables - necessary evil when using GLUT
//global width and height
int GW;
int GH;

// Location of the shoulder
float armPosX = 0, armPosY = 0;
// How far off perfectly straight the forearm should be roatated
float foreArmRotate = 30.0;


#define RED 0
#define GREEN 1
#define BLUE 2

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

void idle(int x) {
	if (x == 0) {
		foreArmRotate += 1.0;
		if (foreArmRotate > 100.0) {
			x = 1;
		}
	}
	else {
		foreArmRotate -= 1.0;
		if (foreArmRotate < 30.0) {
			x = 0;
		}
	}

	printf("Hello %d\n", x);
	glutTimerFunc( 24, idle, x);
	glutPostRedisplay();
}

// Draws a two part arm using rectangles
void drawArm() {
	glPushMatrix(); {
		glColor3f(1.0, 0.0, 0.0);
		glRotatef(45, 0,0,1);
		glRectf( -.05, -.25, .05, .25);
		glPushMatrix(); { // ForeArm
			glTranslatef(0, -.20, .1);
			glRotatef(foreArmRotate, 0,0,-1);
			glColor3f(0.0,1.0, 0.0);
			glRectf(-.05, 0, .05, .50);
		} glPopMatrix();
	} glPopMatrix();
}



//the display call back - all drawing should be done in this function
void display() {
	glClear(GL_COLOR_BUFFER_BIT);
//	glPushMatrix();
//	{
//		glTranslatef(xPos, yPos, 0);
//		glRotatef(theta, 0, 0, 1);
//		glBegin(GL_TRIANGLES);
//		{
//			glColor3f(1.0, 0.0, 0.0);
//			glVertex2f(0.2 * cos(deg2rad(0)), 0.2 * sin(deg2rad(0)));
//			glVertex2f(0.2 * cos(deg2rad(120)), 0.2 * sin(deg2rad(120)));
//			glVertex2f(0.2 * cos(deg2rad(240)), 0.2 * sin(deg2rad(240)));
//		}
//		glEnd();
//	}
//	glPopMatrix();

	glPushMatrix(); {
		glTranslatef(.2, .2, 0);
		drawArm();
	} glPopMatrix();

	glutSwapBuffers();
}

int lastMouseX, lastMouseY;

//the mouse button callback
void mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON) {
    if (state == GLUT_DOWN) { /* if the left button is clicked */
	 }
	 if (state == GLUT_UP) {
	 }
  }
}

//the mouse move callback
void mouseMove(int x, int y) {

}

//the keyboard callback
void keyboard(unsigned char key, int x, int y ){
  switch( key ) {
    case 'q': case 'Q' :
      exit( EXIT_SUCCESS );
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
  glutTimerFunc( 100, idle, 0);

	glutMainLoop();
}
