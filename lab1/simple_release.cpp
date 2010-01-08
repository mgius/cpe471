//A very simple opengl/GLUT program
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

#include <vector>

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
//vector2 ten_pts[10];
std::vector<vector2> pts;
//keep track of the number of points stored thus far
//keep track of which drawing mode we are in points or square
//mode ==0 means draw square otherwise draw points
int mode;

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
  for (int i=0; i<pts.size(); i++) {
    glVertex2f(pts[i].x, pts[i].y);
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
      printf("mouse clicked at %d %d (%f, %f)\n", x, y, p2w_x(x), p2w_y(y) );
        vector2 temp;
        temp.x = p2w_x(x);
        temp.y = p2w_y(y);

        pts.push_back(temp);
        glutPostRedisplay();

    }
  }
}

//the mouse move callback
void mouseMove(int x, int y) {
  printf("mouse moved at %d %d\n", x, y);
  vector2 temp;
  temp.x = p2w_x(x);
  temp.y = p2w_y(y);

  pts.push_back(temp);
  glutPostRedisplay();
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
    case 'c' : case 'C' :
      printf("Clearing points\n");
      pts.clear();
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

void menu ( int value) {
   if (value == 1) {
      printf("Switching mode");
      mode = !mode;
      glutPostRedisplay();
   }
   else if (value == 2) {
      printf("clear");
      pts.clear();
      glutPostRedisplay();
   }
   else if (value == 3) {
      printf("quit");
      exit( EXIT_SUCCESS );
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
  mode = 0;
  //register the callback functions
	glutDisplayFunc( display );
  glutMouseFunc( mouse );
  glutMotionFunc( mouseMove );
  glutKeyboardFunc( keyboard );
  glutReshapeFunc( reshape );

  // menu
  int rightMenu = glutCreateMenu(menu);
  glutAddMenuEntry("Toggle Mode", 1);
  glutAddMenuEntry("Clear Points", 2);
  glutAddMenuEntry("Quit", 3);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();
}
