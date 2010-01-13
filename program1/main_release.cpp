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

#define COLOR_RED    1
#define COLOR_GREEN  2
#define COLOR_BLUE   4
#define COLOR_SAMPLE 8
bool sample_color = true;
float cur_r=1.0, cur_g=0, cur_b=0, cur_a=0;

#define SIZE_LARGE 10
#define SIZE_SMALL 5

int current_size = SIZE_SMALL;

//#define WorldW 2 *(float)GW / (float)GH
#define WorldW 2.0
#define WorldH 2.0

float p2w_x(float w) {
	return WorldW / (float)img->width * w - WorldW / 2;
}

float p2w_y(float h) {
	return -1 * (WorldH / (float)img->height * h - WorldH / 2);
}
float deg2rad(int deg) {
	return M_PI * deg / 180.0;
}

class stroke {
public:
   float x,y; // location in worldspace
   int shape; // shape of stroke
   float r,g,b,a; // color of stroke (and alpha)
   int size; // size of stroke

   stroke(float x_, float y_) : shape(current_shape), r(cur_r),
              g(cur_g), b(cur_b), a(cur_a), size(current_size) {
      x = x_;
      y = y_;
   }

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

private:
   void drawCircle() {
      //printf("Drawing a circle at %f, %f\n", x, y);
		glBegin(GL_TRIANGLE_FAN);
      glColor3f(r,g,b);
      glVertex2f(x,y);
		for (int i = 0; i < 180; i++) {
			glVertex2f((float)size / GW * cos(deg2rad(i*2)) + x, 
                    (float)size / GH * sin(deg2rad(i*2)) + y);
		}
		glEnd();
   }

   void drawSquare() {
      //square centered at x,y, with vertices at x + 1/2width, y + 1/2height, etc
      glBegin(GL_POLYGON);
         float width = (float)size / GW * 2;
         float height = (float)size / GH * 2;
         glColor3f(r,g,b);
         glVertex2f(x + width / 2, y + height / 2);
         glVertex2f(x + width / 2, y - height / 2);
         glVertex2f(x - width / 2, y - height / 2);
         glVertex2f(x - width / 2, y + height / 2);
      glEnd();

   }

   void drawQ() {

   }

};
vector<stroke> strokes;


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

   for (int i = 0; i < strokes.size(); i++) {
      strokes[i].draw();
   }

	glutSwapBuffers();
}

void displayImage() { 
  printf("image display called\n");
  DrawImage();
  glutSwapBuffers();
}

inline void sample(int x, int y) {
   float pixBuf[3];
   glReadPixels(x,img->height - y, 1,1, GL_RGB, GL_FLOAT, &pixBuf);
   //printf("found color %f, %f, %f\n", pixBuf[0], pixBuf[1], pixBuf[2]);
   cur_r = pixBuf[0];
   cur_g = pixBuf[1];
   cur_b = pixBuf[2];
   cur_a = 0;
}

// iterates over the whole image, sampling the image at 5 pixel intervals
// 
void backFill() {

}

//the mouse button callback
void mouse(int button, int state, int x, int y) {

  if (button == GLUT_LEFT_BUTTON) {
    if (state == GLUT_DOWN) { /* if the left button is clicked */
      printf("mouse clicked at %d %d, (%f, %f)\n", x, y, p2w_x(x), p2w_y(y));
      if (sample_color) {
         sample(x,y);
      }
      strokes.push_back(stroke(p2w_x(x), p2w_y(y)));
      glutSetWindow(mainWin);
      glutPostRedisplay();
    }
  }
}

//the mouse move callback
void mouseMove(int x, int y) {
   if (sample_color) {
      sample(x,y);
   }
   strokes.push_back(stroke(p2w_x(x), p2w_y(y)));
   glutSetWindow(mainWin);
   glutPostRedisplay();
}



//the keyboard callback
void keyboard(unsigned char key, int x, int y ){
  switch( key ) {

    case 'q': case 'Q' :
      exit( EXIT_SUCCESS );
      break;

  }
}

void shapeMenuHandler(int value) {
   current_shape = value;
}

void colorMenuHandler(int value) {
   sample_color = false;
   if (value == COLOR_SAMPLE) {
      sample_color = true;
   }
   else if (value == COLOR_RED) {
      cur_r=1.0;
      cur_g=cur_b=cur_a=0;
   }
   else if (value == COLOR_GREEN) {
      cur_g=1.0;
      cur_r=cur_b=cur_a=0;
   }
   else if (value == COLOR_BLUE) {
      cur_b=1.0;
      cur_r=cur_g=cur_a=0;
   }
   else {
      printf("invalid color");
   }

}

void sizeMenuHandler(int value) {
   current_size = value;
}

void mainMenuHandler(int value) {
   if (value == 1) {
      printf("TODO: backfill");
   }
   else {
      printf("Invalid selection in mainMenuHandler");
   }

}

void doMenus() {
   int shapeMenu = glutCreateMenu(shapeMenuHandler);
   glutAddMenuEntry("circle", SHAPE_CIRCLE);
   glutAddMenuEntry("square", SHAPE_SQUARE);
   glutAddMenuEntry("q", SHAPE_Q);

   int colorMenu = glutCreateMenu(colorMenuHandler);
   glutAddMenuEntry("red", COLOR_RED);
   glutAddMenuEntry("green", COLOR_GREEN);
   glutAddMenuEntry("blue", COLOR_BLUE);
   glutAddMenuEntry("sample from image", COLOR_SAMPLE);

   int sizeMenu = glutCreateMenu(sizeMenuHandler);
   glutAddMenuEntry("small", SIZE_SMALL);
   glutAddMenuEntry("large", SIZE_LARGE);

   int mainMenu = glutCreateMenu(mainMenuHandler);
   glutAddSubMenu("shapes", shapeMenu);
   glutAddSubMenu("colors", colorMenu);
   glutAddSubMenu("size", sizeMenu);
   glutAddMenuEntry("backfill", 1);

   glutAttachMenu(GLUT_RIGHT_BUTTON);

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


