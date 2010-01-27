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
//current mouse position in pixel coordinate
//int x;
//int y;

// current rotation and position of triangle
float theta = 0.0;
float xPos = 0.0, yPos = 0.0;

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

void idle(int) {
	theta += 1.0;
	if (theta >= 360.0)
		theta-=360.0;
	glutTimerFunc(100, idle, 0);
	glutPostRedisplay();
}


//the display call back - all drawing should be done in this function
#define TRIANGLE_SIDE_LENGTH 1.0
void display() {
	printf("display %f\n", theta);
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	glRotatef(theta, 0, 0, 1);
	glBegin(GL_TRIANGLES);
		glColor3f(1.0, 0.0, 0.0);
		glVertex2f(cos(deg2rad(0)), sin(deg2rad(0)));
		glVertex2f(cos(deg2rad(120)), sin(deg2rad(120)));
		glVertex2f(cos(deg2rad(240)), sin(deg2rad(240)));
	glEnd();
	glPopMatrix();

	glutSwapBuffers();
}

//the mouse button callback
void mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON) {
    if (state == GLUT_DOWN) { /* if the left button is clicked */
      printf("mouse clicked at %d %d (%f, %f)\n", x, y, p2w_x(x), p2w_y(y) );

      glutPostRedisplay();

    }
  }
}

//the mouse move callback
void mouseMove(int x, int y) {
  printf("mouse moved at %d %d\n", x, y);
  // shouldn't do anything on move

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
  glutTimerFunc( 1, idle, 0 );

	glutMainLoop();
}
