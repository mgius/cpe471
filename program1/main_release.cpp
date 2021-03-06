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

typedef struct Point {
   float x;
   float y;
   Point(float x_, float y_) {
      x = x_;
      y = y_;
   }
} Point;

#define RED 0
#define GREEN 1
#define BLUE 2
typedef struct Color {
   GLfloat rgb[3];
   Color(float r_, float g_, float b_) {
      rgb[RED] = r_;
      rgb[GREEN] = g_;
      rgb[BLUE] = b_;
   }
} Color;

// Current stroke settings
#define SHAPE_CIRCLE 1
#define SHAPE_SQUARE 2
#define SHAPE_BARS   4
#define SHAPE_LINE   8
#define SHAPE_STROKES 16
int current_shape = SHAPE_CIRCLE;

#define COLOR_RED    1
#define COLOR_GREEN  2
#define COLOR_BLUE   4
#define COLOR_SAMPLE 8
#define COLOR_SAMPLE_ONCE 16
bool sample_color = true;
bool sample_once = false;
float cur_r=1.0, cur_g=0, cur_b=0;

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

// There is a bug here.  Somebody could transition stroke types 
// indefinitely and eventually fill up memory without ever drawing a stroke
// Stroke class, contains a vector of points and colors.  When size/shape 
// changes, should spawn a new stroke to contain it.
class stroke {
private:
   float x,y; // location in worldspace
   vector<Point> points; // locations in worldspace
   int shape; // shape of stroke
   vector<Color> colors; // Color of each point in worldspace
   int size; // size of stroke

public:
   stroke() : shape(current_shape), size(current_size) {
   }

   void addPoint(Point aPoint) {
      points.push_back(aPoint);
      colors.push_back(Color(cur_r, cur_g, cur_b));
   }

   // Draws a stroke to the current window
   void draw() {
		if (shape == SHAPE_CIRCLE)
			drawCircles();
		else if(shape == SHAPE_SQUARE)
			drawSquares();
		else if(shape == SHAPE_BARS) 
			drawBars();
      else if(shape == SHAPE_LINE)
         drawLines();
      else if(shape == SHAPE_STROKES) {
         drawStrokes();
      }
		else 
			printf("Not a valid stroke type, ignoring stroke...\n");
   }

private:
   void drawCircles() {
      // Each Point is the center of a circle
      for (int j = 0; j < points.size(); j++) {
         glBegin(GL_TRIANGLE_FAN);
         glColor3fv(colors[j].rgb);
         glVertex2f(points[j].x,points[j].y);
         for (int i = 0; i < 31; i++) {
            glVertex2f((float)size / GW * cos(deg2rad(i*12)) + points[j].x, 
                  (float)size / GH * sin(deg2rad(i*12)) + points[j].y);
         }
         glEnd();

      }
   }

   void drawSquares() {
      // square centered at x,y, 
      // with vertices at x + 1/2width, y + 1/2height, etc
      float width = (float)size / GW * 2;
      float height = (float)size / GH * 2;
      // each point is the center of a square
      for(int i = 0; i < points.size(); i++) {
         glBegin(GL_POLYGON);
            glColor3fv(colors[i].rgb);
            glVertex2f(points[i].x + width / 2, points[i].y + height / 2);
            glVertex2f(points[i].x + width / 2, points[i].y - height / 2);
            glVertex2f(points[i].x - width / 2, points[i].y - height / 2);
            glVertex2f(points[i].x - width / 2, points[i].y + height / 2);
         glEnd();
        
      }
   }

   void drawBars() {
      float width = (float) size / GW * 3;
      float height = (float) size / GH * 3;
      // each point is the center of a set of bars
      for (int i = 0; i < points.size(); i ++) {

         glBegin(GL_QUADS);
            glColor3fv(colors[i].rgb);
            // top bar
            glVertex2f(points[i].x - width / 2, points[i].y + height);
            glVertex2f(points[i].x + width / 2, points[i].y + height);
            glVertex2f(points[i].x + width / 2, points[i].y + height / 2);
            glVertex2f(points[i].x - width / 2, points[i].y + height / 2);

            // bottom bar
            // top bar
            glVertex2f(points[i].x - width / 2, points[i].y - height);
            glVertex2f(points[i].x + width / 2, points[i].y - height);
            glVertex2f(points[i].x + width / 2, points[i].y - height / 2);
            glVertex2f(points[i].x - width / 2, points[i].y - height / 2);
         glEnd();
      }
   }

   void drawLines() {
      float oldLineWidth; 
      glGetFloatv(GL_LINE_WIDTH, &oldLineWidth);

      glLineWidth(size);

      glBegin(GL_LINES);
      // If there are an odd number of points, consider the last point
      // to be an incomplete line and ignore it
      for (int i = 0; i < points.size() / 2; i++) {
         glColor3fv(colors[2 * i].rgb);
         glVertex2f(points[2 * i].x, points[2 * i].y);
         glColor3fv(colors[2 * i + 1].rgb);
         glVertex2f(points[2 * i + 1].x, points[2 * i + 1].y);
      }
      glEnd();
      glPointSize(oldLineWidth);
   }

   void drawStrokes() {
      float oldLineWidth; 
      glGetFloatv(GL_LINE_WIDTH, &oldLineWidth);

      glLineWidth(size);

      glBegin(GL_LINES);
      // If there are an odd number of points, consider the last point
      // to be an incomplete line and ignore it
      for (int i = 0; i < points.size(); i++) {
         glColor3fv(colors[i].rgb);
			// Start the stroke some arbitrary distance off the screen
         glVertex2f(1.5, 0);
         glVertex2f(points[i].x, points[i].y);
      }
      glEnd();
      glPointSize(oldLineWidth);

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

   // Generic Draw function makes this trivial
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

// Samples the image at a particular location. Assumes that x/y passed in
// has not been manipulated
inline void sample(int x, int y) {
   float pixBuf[3];
   glReadPixels(x,img->height - y, 1,1, GL_RGB, GL_FLOAT, &pixBuf);
   //printf("found color %f, %f, %f\n", pixBuf[0], pixBuf[1], pixBuf[2]);
   cur_r = pixBuf[RED];
   cur_g = pixBuf[GREEN];
   cur_b = pixBuf[BLUE];
}

// iterates over the whole image, sampling the image at 5 pixel intervals
// 
void backFill(int skip) {
   printf("Backfill\n");
   glutSetWindow(imgWin);
   for (int x = 0; x < img->width / skip; x++) {
      for (int y = 0; y < img->height / skip; y++) {
         sample(x * skip, y * skip);
         strokes.back().addPoint(Point(p2w_x(x * skip), p2w_y(y * skip)));
      }
   }
   glutSetWindow(mainWin);
   glutPostRedisplay();
}

//the mouse button callback
void mouse(int button, int state, int x, int y) {

  if (button == GLUT_LEFT_BUTTON) {
    if (state == GLUT_DOWN) { /* if the left button is clicked */
      printf("mouse clicked at %d %d, (%f, %f)\n", x, y, p2w_x(x), p2w_y(y));
      if (sample_color) {
         sample(x,y);
      }
      if (sample_once) {
         sample_color = sample_once = false;
         return;
      }
      strokes.back().addPoint(Point(p2w_x(x), p2w_y(y)));
      glutSetWindow(mainWin);
      glutPostRedisplay();
    }
  }
}

//the mouse move callback
void mouseMove(int x, int y) {
   // don't do anything if line
   if (current_shape == SHAPE_LINE)
      return;

   if (sample_color) {
      sample(x,y);
   }
   strokes.back().addPoint(Point(p2w_x(x), p2w_y(y)));
   glutSetWindow(mainWin);
   glutPostRedisplay();
}



//the keyboard callback
void keyboard(unsigned char key, int x, int y ){
  switch( key ) {

    case 'c': case 'C' :
      strokes.clear();
      strokes.push_back(stroke());
      glutPostWindowRedisplay(mainWin);
      break;
    case 'q': case 'Q' :
      exit( EXIT_SUCCESS );
      break;

  }
}

// Changes shape.  I'm using the same value for the menu and 
// stroke class.  Create a new stroke for points to be added to
void shapeMenuHandler(int value) {
   current_shape = value;
   strokes.push_back(stroke());
}

// Changes color.  No need to create a new stroke.
void colorMenuHandler(int value) {
   sample_color = false;
   if (value == COLOR_SAMPLE) {
      sample_color = true;
   }
   else if (value == COLOR_SAMPLE_ONCE) {
      sample_color = sample_once = true;
   }
   else if (value == COLOR_RED) {
      cur_r=1.0;
      cur_g=cur_b=0;
   }
   else if (value == COLOR_GREEN) {
      cur_g=1.0;
      cur_r=cur_b=0;
   }
   else if (value == COLOR_BLUE) {
      cur_b=1.0;
      cur_r=cur_g=0;
   }
   else {
      printf("invalid color");
   }


}

// changes size.  Need to create a new stroke
void sizeMenuHandler(int value) {
   current_size = value;
   strokes.push_back(stroke());
}

// base menu options.  Currently supports backfill and backfill sparse
void mainMenuHandler(int value) {
   if (value == 1) {
      backFill(3);
   }
   else if (value == 2) {
      backFill(8);
   }
   else {
      printf("Invalid selection in mainMenuHandler");
   }

}

// Creates the menus and attaches them to both the main and img window
void doMenus() {
   int shapeMenu = glutCreateMenu(shapeMenuHandler);
   glutAddMenuEntry("circle", SHAPE_CIRCLE);
   glutAddMenuEntry("square", SHAPE_SQUARE);
   glutAddMenuEntry("bars", SHAPE_BARS);
   glutAddMenuEntry("line", SHAPE_LINE);
   glutAddMenuEntry("strokes", SHAPE_STROKES);

   int colorMenu = glutCreateMenu(colorMenuHandler);
   glutAddMenuEntry("red", COLOR_RED);
   glutAddMenuEntry("green", COLOR_GREEN);
   glutAddMenuEntry("blue", COLOR_BLUE);
   glutAddMenuEntry("sample from image", COLOR_SAMPLE);
   glutAddMenuEntry("sample once from image", COLOR_SAMPLE_ONCE);

   int sizeMenu = glutCreateMenu(sizeMenuHandler);
   glutAddMenuEntry("small", SIZE_SMALL);
   glutAddMenuEntry("large", SIZE_LARGE);

   int mainMenu = glutCreateMenu(mainMenuHandler);
   glutAddSubMenu("shapes", shapeMenu);
   glutAddSubMenu("colors", colorMenu);
   glutAddSubMenu("size", sizeMenu);
   glutAddMenuEntry("backfill", 1);
   glutAddMenuEntry("backfill sparse", 2);

   glutSetWindow(mainWin);
   glutAttachMenu(GLUT_RIGHT_BUTTON);
   glutSetWindow(imgWin);
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
  //glutReshapeFunc( reshape );

  //imgWin = glutCreateSubWindow(mainWin, 0, 0, img->width, img->height);		
  glutInitWindowSize(img->width, img->height);
  glutInitWindowPosition(50, 50);
  imgWin = glutCreateWindow( "image" );		
	// it is required to register a display func for 
	// each window
  glutDisplayFunc(displayImage);
  // I want the keyboard callback for the image window
  glutKeyboardFunc( keyboard );

  glutMotionFunc( mouseMove );
  glutMouseFunc( mouse );

  doMenus();
  // Create a default stroke to add points to, otherwise we'll segfault first
  // time we call strokes.back()
  strokes.push_back(stroke());
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


