//A very simple opengl/GLUT program
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

//global variables - necessary evil when using GLUT
//global width and height
int GW;
int GH;
//current mouse position in pixel coordinate
int x;
int y;
//a structure to store 2D points  - this could also be a class
typedef struct vector2 {
  float x;
  float y;
} vector2;
//an array to store ten 2D points
vector2 ten_pts[10];
//keep track of the number of points stored thus far
int num_pts;
//keep track of which drawing mode we are in points or square
//mode ==0 means draw square otherwise draw points
int mode;

//helper drawing routines
void draw_square() {
  glBegin(GL_POLYGON);
    glColor3f(1.0, 0.0, 0.0);
		glVertex2f(-0.5,-0.5); 
    glColor3f(0.0, 1.0, 0.0); 
		glVertex2f(-0.5,0.5);
	  glColor3f(0.0, 0.0, 1.0);
		glVertex2f(0.5,0.5); 
		glColor3f(0.0, 0.0, 0.0);  
		glVertex2f(0.5,-0.5);
	glEnd();
}

void draw_pts() {
  glPointSize(2.0);
  glBegin(GL_POINTS);
    glColor3f(0.5, 0.2, 0.8);
  for (int i=0; i<num_pts; i++) {
    glVertex2f(ten_pts[i].x, ten_pts[i].y);
    //printf("pt: %f %f\n", ten_pts[i].x, ten_pts[i].y);
  }
  glEnd();
}


//the display call back - all drawing should be done in this function
void display() {
	glClear(GL_COLOR_BUFFER_BIT);

  if (mode == 0)
    draw_square();
  else
    draw_pts();

	glFlush();
}

//the mouse button callback
void mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON) {
    if (state == GLUT_DOWN) { /* if the left button is clicked */
      printf("mouse clicked at %d %d\n", x, y);
      if (num_pts < 10) {
        ten_pts[num_pts].x = x;
        ten_pts[num_pts].y = y;
        num_pts++;
        glutPostRedisplay();

      }
    }
  }
}

//the mouse move callback
void mouseMove(int x, int y) {
  printf("mouse moved at %d %d\n", x, y);
}

//the keyboard callback
void keyboard(unsigned char key, int x, int y ){
  switch( key ) {
    case 'm':
      mode = !mode;
      glutPostRedisplay();
      break;
    case 'q': case 'Q' :
      exit( EXIT_SUCCESS );
      break;
    case 'h' : case 'H' :
      printf("hello!\n");
      break;
  }
}

int main( int argc, char** argv ){

	glutInit( &argc, argv );

  //intializations
	glutInitWindowSize(200, 200);
	glutInitWindowPosition(100, 100);
	glutCreateWindow( "My Second Window" );
	glClearColor(1.0, 1.0, 1.0, 1.0);
  //global variable intialization
  GW = GH = 200;
  num_pts = 0;
  mode = 0;
  //register the callback functions
	glutDisplayFunc( display );
  glutMouseFunc( mouse );
  glutMotionFunc( mouseMove );
  glutKeyboardFunc( keyboard );

	glutMainLoop();
}