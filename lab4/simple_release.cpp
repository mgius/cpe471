//A very simple opengl/GLUT program
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include <vector>

using namespace std;
//global variables - necessary evil when using GLUT
//global width and height
int GW;
int GH;
//current mouse position in pixel coordinate
//int x;
//int y;

typedef struct PixelPoint {
   int x;
   int y;
   PixelPoint(int x_, int y_) {
      x = x_;
      y = y_;
   }
} PixelPoint;

typedef struct WorldPoint {
  float x;
  float y;
  WorldPoint(float x_, float y_) {
     x = x_;
     y = y_;
  }
} WorldPoint;

int mode;
int &current_shape = mode;

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

#define SHAPE_MYLINE 1
#define SHAPE_LINE   2

void myglVertex2f(GLfloat x, GLfloat y) {
	//printf("dwawing world point %f, %f\n", p2w_x(x), p2w_y(y));
	glVertex2f(p2w_x(x), p2w_y(y));
}
void myglVertex2fReverse(GLfloat y, GLfloat x) {
	myglVertex2f(x, y);
}

class stroke {
   vector<PixelPoint> points;

public:
   stroke() {
   }

   void addPoint(PixelPoint aPoint) {
      points.push_back(aPoint);
   }
   // Draws a stroke to the current window
   void draw() {
      if (current_shape == SHAPE_MYLINE)
         drawMyLines();
      else if(current_shape == SHAPE_LINE)
         drawLines();
      else
         printf("Not a valid stroke type, ignoring stroke...\n");
   }
private:
	// Takes in startXY, endXY, and uses parametric line equation to
	// fill in GL_POINTS.  Takes in an "myVertex2f" function pointer that 
	// will be glVertex2f, or a proxy to glVertex2f that swaps X and Y first
	void fillLine(int startX, int endX, int startY, int endY,
			void (*myVertex2f)(GLfloat, GLfloat)) {
		// implement parametric 
		float cur_Y = endY;
		float delta = (float)(endY - startY) / (float)(endX - startX);
		printf("startX: %d, endX %d, startY %d, endY %d\n", 
				startX, endX, startY, endY);
		printf("delta %f\n", delta);
		glBegin(GL_POINTS);
		glColor3f(1.0,0.0,0.0);
		for (int cur_X = startX; cur_X < endX; cur_X++) {
			//printf("drawing pixel point %d, %d\n", cur_X, lroundf(cur_Y));
			myVertex2f(cur_X, roundf(cur_Y));
			cur_Y += delta;
		}
		glEnd();

	}
   void drawMyLines() {
		printf("Drawing My Lines\n");
		for (int i = 0; i < (points.size() / 2); i++) {
			int xStart = points[2 * i].x;
			int xEnd = points[2 * i + 1].x;
			int yStart = points[2 * i].y;
			int yEnd = points[2 * i + 1].y;

			// Swap the start / end if they're not small -> large

			// Check for zero slope, infinite slope

			// slope <= 1
			if (1 >= (float)(yEnd - yStart) / (float)(xEnd - xStart)) {
				printf("Small slope\n");
				if (0 > (xEnd - xStart)) {
					int temp = xEnd;
					xEnd = xStart;
					xStart = temp;
					temp = yEnd;
					yEnd = yStart;
					yStart = temp;
				}
				fillLine(xStart, xEnd, yStart, yEnd, myglVertex2f);
			}
			else {
				printf("Large slope\n");
				if (0 > (yEnd - yStart)) {
					printf("swapping Y\n");
					int temp = yEnd;
					yEnd = yStart;
					yStart = temp;
					temp = xEnd;
					xEnd = xStart;
					xStart = temp;
				}
				//fillLine(xStart, xEnd, yStart, yEnd, glVertex2f);
				fillLine(yStart, yEnd, xStart, xEnd, myglVertex2fReverse);
			}

		}
   }

   void drawLines() {
      printf("Drawing Lines\n");
      glBegin(GL_LINES);
      // floor points.size / 2 so if uneven number of points the last
      // gets dropped
      for (int i = 0; i < (points.size() / 2); i++) {
			glColor3f(0.0,1.0,0.0);
         glVertex2f(p2w_x(points[2 * i].x), p2w_y(points[2 * i].y));
			glColor3f(1.0,0.0,0.0);
         glVertex2f(p2w_x(points[2 * i + 1].x), p2w_y(points[2 * i + 1].y));
      }
      glEnd();
   }

};
vector<stroke> strokes;


//the display call back - all drawing should be done in this function
void display() {
	glClear(GL_COLOR_BUFFER_BIT);

   for (int i = 0; i < strokes.size(); i++) {
      strokes[i].draw();
   }

	glFlush();
}

//the mouse button callback
void mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON) {
    if (state == GLUT_DOWN) { /* if the left button is clicked */
      printf("mouse clicked at %d %d (%f, %f)\n", x, y, p2w_x(x), p2w_y(y) );

      strokes.back().addPoint(PixelPoint(x, y));
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
      printf("Clearing points\n");
      strokes.clear();
      strokes.push_back(stroke());
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

#define LINE_MODE 1
#define MYLINE_MODE 2   
#define CLEAR 4
void menu ( int value) {
   if (value == LINE_MODE) {
      printf("Line mode\n");
      mode = SHAPE_LINE;
   }
   else if (value == MYLINE_MODE) {
      printf("myLine mode\n");
      mode = SHAPE_MYLINE;
   }
   else if (value == CLEAR) {
      printf("clear");
      strokes.clear();
		strokes.push_back(stroke());
      glutPostRedisplay();
   }
}

int main( int argc, char** argv ){

	glutInit( &argc, argv );

  //intializations
	glutInitWindowSize(200, 200);
	glutInitWindowPosition(100, 100);
	glutCreateWindow( "My Third Window" );
	glClearColor(1.0, 1.0, 1.0, 1.0);
  //global variable intialization
  GW = GH = 200;
  mode = SHAPE_LINE;
  strokes.push_back(stroke());
  //register the callback functions
	glutDisplayFunc( display );
  glutMouseFunc( mouse );
  glutMotionFunc( mouseMove );
  glutKeyboardFunc( keyboard );
  glutReshapeFunc( reshape );

  // menu
  int rightMenu = glutCreateMenu(menu);
  glutAddMenuEntry("Line Mode", LINE_MODE);
  glutAddMenuEntry("MyLine Mode", MYLINE_MODE);
  glutAddMenuEntry("Clear Points", CLEAR);
  glutAddMenuEntry("Quit", 3);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();
}
