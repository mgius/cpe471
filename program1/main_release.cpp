/**
 * Code which uses the image library originally from Princeton COS 426
 * main by ZJW for CalPoly CSC 471 assignment 1 - NPR painterly rendering 
 * on image
 **/

#include "image.h"
#include "bmpio.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

/**
 * OS specific from COS 426 code to allow reading binary images from stdin
 **/
#ifdef __unix__
#include <GL/glut.h>
#endif

#ifdef __APPLE__
#include "GLUT/glut.h"
#endif

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#include <glut.h>
#endif

/**
 * prototypes
 **/
static void SetBinaryIOMode(void);

using namespace std;

/*
OpenGl insert ZJW: 
A very simple opengl/GLUT program to read in the image and display it
*/

//global variables - due to GLUT
//global width and height
int GW;
int GH;
//current mouse position in pixel coordinate
int x;
int y;
int mainWin, imgWin;
//global image
Image *img;

// Current stroke settings
#define SHAPE_CIRCLE 1
#define SHAPE_SQUARE 2
#define SHAPE_Q      4
int current_shape = SHAPE_CIRCLE;

bool use_source_color = false;
float cur_r=1.0, cur_g=0, cur_b=0, cur_a=0; //

#define SIZE_LARGE 10
#define SIZE_SMALL 5

int current_size = SIZE_SMALL;



#define WorldW 2 *(float)GW / (float)GH
#define WorldH 2.0

float p2w_x(float w) {
	return WorldW / img->width * w - WorldW / 2;
}

float p2w_y(float h) {
	return -1 * (WorldH / img->height * h - WorldH / 2);
}
float deg2rad(int deg) {
	return M_PI * deg / 180;
}
// #define of stroke types
class stroke {
public:
   float x,y; // location in worldspace
   int shape; // shape of stroke
   float r,g,b,a; // color of stroke (and alpha)
   int size; // size of stroke

	stroke() : x(0), y(0), shape(SHAPE_CIRCLE), r(1.0), 
	           g(0), b(0), a(0), size(SIZE_LARGE) {}
   // Draws a stroke to the current window
   void draw() {
		if (shape == SHAPE_CIRCLE)
			drawCircle();
		else if(shape == SHAPE_SQUARE)
			drawSquare();
		else if(shape == SHAPE_Q) 
			drawQ();
		else 
			printf("Not a valid stroke type, ignoring stroke...\n");
   }

   void drawCircle() {
		glBegin(GL_POLYGON);
		for (int i = 0; i < 360; i++) {
			glVertex2f(cos(deg2rad(i)) + x, sin(deg2rad(i)) + y);
		}
		glEnd();
   }

   void drawSquare() {

   }

   void drawQ() {

   }

};


/* Image scaling upon window reshape. */
void reshape(int w, int h)
{
	GW = w;
	GH = h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-(float)w/h, (float)w/h, -1, 1, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, w, h);
}


void DrawImage() {
  glDrawPixels( img->width, img->height, GL_RGBA, 
                GL_UNSIGNED_BYTE, img->pixels);
}




//the display call back - all drawing should be done in this function
void display() {
  printf("In main display\n");
	glClear(GL_COLOR_BUFFER_BIT);
//.....

	glutSwapBuffers();
}

void displayImage() { 
  printf("image display called\n");
  DrawImage();
  glutSwapBuffers();

}

//the mouse button callback
void mouse(int button, int state, int x, int y) {

  if (button == GLUT_LEFT_BUTTON) {
    if (state == GLUT_DOWN) { /* if the left button is clicked */
      printf("mouse clicked at %d %d, (%f, %f)\n", x, y, p2w_x(x), p2w_y(y));
    }
  }
}

//the mouse move callback
void mouseMove(int x, int y) {
  unsigned char color[3];

  
}



//the keyboard callback
void keyboard(unsigned char key, int x, int y ){
  switch( key ) {

    case 'q': case 'Q' :
      exit( EXIT_SUCCESS );
      break;

  }
}


void doMenus() {
}

/**
 * main
 **/
int main(int argc, char **argv)
{
  /* image library code */
  img = NULL;
  // setup
  SetBinaryIOMode();
  img = BMPReadImage(stdin);

  /* ZJW insert for display */
  //intializations
  //global variable intialization
  GW = img->width*3.0;
  GH = img->height*3.0;

  glutInit( &argc, argv );
  glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE);
  glutInitWindowSize(img->width*3, img->height*3);
  glutInitWindowPosition(100, 100);
  mainWin = glutCreateWindow( "NPR on images" );
  glClearColor(1.0, 1.0, 1.0, 1.0);
  //register the callback functions for the main window
  glutDisplayFunc( display );
  glutKeyboardFunc( keyboard );
  doMenus();

  imgWin = glutCreateSubWindow(mainWin, 0, 0, img->width, img->height);		
	// it is required to register a display func for 
	// each window
  glutDisplayFunc(displayImage);
  glutMouseFunc( mouse );
  glutMotionFunc( mouseMove );

  glutMainLoop();
  stroke myStroke();
  myStroke.draw();

}


/**
 * SetBinaryIOMode
 *
 * In WindowsNT by default stdin and stdout are opened as text files.
 * This code opens both as binary files.
 **/
static void SetBinaryIOMode(void)
{
#ifdef _WIN32
    int result;

    result = _setmode(_fileno(stdin), _O_BINARY);
    if (result == -1)
    {
        perror("Cannot set stdin binary mode");
        exit(EXIT_FAILURE);
    }

    result = _setmode(_fileno(stdout), _O_BINARY);
    if (result == -1)
    {
        perror("Cannot set stdout binary mode");
        exit(EXIT_FAILURE);
    }
#endif
}


