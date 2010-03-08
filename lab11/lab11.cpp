/*
simple example of code for picking in 3d - 
prints the "name" of the object hit - either cone= 4 or cyl=4
ZJ Wood 
*/

#include <stdlib.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <iostream>

#define CONE 4
#define CYL 3
#define DRAW 1
#define SELECT 2
#define BUFSIZE 512
#define BASE_HEIGHT 2.0
#define BASE_RADIUS 1.5
#define CONE_HEIGHT 4.0

/*globals for picking, etc. */
GLuint selectBuf[BUFSIZE];
float pickbuf;
int mouseX, mouseY;
float fov;
int GW, GH;

typedef struct Vector3 {
    float x;
    float y;
    float z;
    Vector3() : x(0), y(0), z(0) {}
} Vector3;

Vector3 eye, la;

GLUquadricObj  *b; /* pointer to quadric object */
GLUquadricObj  *t; /* pointer to quadric object */

/*forward function declaration */
void processHits(GLint hits, GLuint buffer[]);
void draw_objs();

//globals for lighting 
GLfloat light_pos0[4] = {0.0, 1.0, 1.0, 0.0};
GLfloat light_diff0[4] = {0.7, 0.7, 0.7, 1.0};
GLfloat light_spec0[4] = {0.6, 0.6, 0.6, 1.0};
GLfloat light_amb0[4] = {0.3, 0.3, 0.3, 1.0};

//playing with materials
typedef struct materialStruct {
  GLfloat ambient[4];
  GLfloat diffuse[4];
  GLfloat specular[4];
  GLfloat shininess[1];
} materialStruct;


materialStruct brownMaterials = {
  {0.1, 0.1, 0.0, 1.0},
  {0.6, 0.6, 0.0, 1.0},
  {0.3, 0.3, 0.0,1.0},
  {0.0}
};

materialStruct DarkPMaterials = {
  {0.2, 0.00, 0.2, 1.0},
  {0.1, 0.0, 0.1, 1.0},
  {0.1, 0.0, 0.1, 1.0},
  {0}
};

materialStruct BlackMaterials = {
  {0.0, 0.0, 0, 1.0},
  {0.0, 0.0, 0.0, 1.0},
  {0.0, 0.0, 0.0, 1.0},
  {100.0}
};

//sets up a specific material
void materials(materialStruct materials) {
  glMaterialfv(GL_FRONT, GL_AMBIENT, materials.ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, materials.diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, materials.specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, materials.shininess);
}

//initialization calls for opengl for static light
void init_lighting() {
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glLightfv(GL_LIGHT0, GL_POSITION, light_pos0);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diff0);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_spec0);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb0);

  glShadeModel(GL_SMOOTH);
  glEnable(GL_NORMALIZE); 
}

void pos_light() {
  //set the light's position
  glMatrixMode(GL_MODELVIEW);
  glLightfv(GL_LIGHT0, GL_POSITION, light_pos0);
}

/*routine to set up for picking */
void DoPicking() {

  int hits;
  GLint viewport[4];

  //get values on the size of the viewport
  glGetIntegerv(GL_VIEWPORT,viewport);
  glSelectBuffer(BUFSIZE,selectBuf);

  //set the mode
  glRenderMode(GL_SELECT);

  //initialize the namestack
  glInitNames();
  //load a junk name to start
  glPushName(0xffffffff);

  //store the current viewing as we'll modify it for picking
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();

  //clear the top of the stack for picking projection
  glLoadIdentity();
  //set up projection such that the pixel picking region is near the mouse
  gluPickMatrix((GLdouble)mouseX,(GLdouble)viewport[3]-mouseY,pickbuf, pickbuf,viewport);
  //set up projection - should match what is called in reshape
  gluPerspective(fov, (float)GW/GH, 1, 20);

  //return to model matrix
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
    //draw the objects
    gluLookAt(eye.x, eye.y, eye.z, la.x, la.y, la.z, 0, 1,0);
    draw_objs();
  glPopMatrix();
  // restoring the original projection matrix
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  //return to model matrix
  glMatrixMode(GL_MODELVIEW);
  glFlush();

  // returning to normal rendering mode - check if we have any hits
  hits = glRenderMode(GL_RENDER);
  printf("number of hits %i\n", hits);

  // if there are hits process them
  if (hits != 0)
    processHits(hits,selectBuf);

}


/*process the hits from picking - format of the picking array are nitem, zmin, zmax, list of names
on the name stack when the pick happened...*/
void processHits(GLint hits, GLuint buffer[])
{
  int i;
  unsigned int j;
  GLuint names, *ptr;

  ptr = (GLuint *) buffer;
  //go through the hit list and print the names of the objects hit
  for (i = 0; i < hits; i++) {	
    names = *ptr;  
    //advance the pointer to the 4th element in the array (because we are in 2d no zmin/zmax)
    ptr = ptr+3;
    printf("the name of the object(s) hit: ");
    for (j = 0; j <names; j++) {
      printf("%d ", *ptr); 
      //add code here to modify an object if its name matches a hit
      //... for example:  if (*ptr == TREE_TOP) modify the trees color

      //advance the pointer to next element in the array
      ptr++;
    }
    printf("\n"); 
  }
}

void cyl() {
  glPushMatrix();
    glTranslatef(-2,0,-2);
    /* rotate cylinder to align with y axis */
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    materials(brownMaterials);
    /* cyliner aligned with z axis, render with
    5 slices for base and 10 along length */
    gluCylinder(b, BASE_RADIUS, BASE_RADIUS, BASE_HEIGHT, 15, 10);
  glPopMatrix();
}

void cone(){
  //set the cone material */
  materials(DarkPMaterials);
  glPushMatrix();
    /* rotate cylinder to align with y axis */
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    gluCylinder(t, 2.5,  0, CONE_HEIGHT, 20, 10);
  glPopMatrix();
}

void draw_objs() {
  glPushMatrix();
    glScalef(0.3, 0.3, 0.3);
    //put the cylinders name on the stack
    glLoadName(CYL);
    cyl();
    //now put the cone's name on the stack
    glLoadName(CONE);
    //apply a translation to displace the cone
    glTranslatef(1.5,0,-2.5);
    //draw cone
    cone();
  glPopMatrix();
}

void Draw3DSGrid(){
	float spacing;
	int extents;

	spacing = 0.5;
	extents = 21;
	glLineWidth(1.0);

	glPushMatrix();
	glTranslatef(0,0.01, 0);
	// Draw a grid along the X and Z axis'
	for(float i = -extents; i <= extents; i += spacing )
	{
    materials(BlackMaterials);
		// Start drawing some lines
    glBegin(GL_LINES);

		  // Do the horizontal lines (along the X)
		  glVertex3f(-extents, 0, i);
		  glVertex3f(extents, 0, i);

		  // Do the vertical lines (along the Z)
		  glVertex3f(i, 0, -extents);
		  glVertex3f(i, 0, extents);

		  // Stop drawing lines
		glEnd();
	}
	glPopMatrix();
}

void display() {

  /* Accumulate ModelView Matrix as we traverse tree */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
    gluLookAt(eye.x, eye.y, eye.z, la.x, la.y, la.z, 0, 1,0);
    pos_light();
    Draw3DSGrid();
    draw_objs();
  glPopMatrix();
  glutSwapBuffers();

}


/*reshape function that makes sure matrix stacks are correct */
void reshape(int w, int h) {
  GW = w;
  GH = h;
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(fov, (float)w/h, 1, 20);
  glMatrixMode(GL_MODELVIEW);
  glViewport(0, 0, w, h);
  
}

/*mouse function to get the mouse coordinates */
void mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON) {
    if (state == GLUT_DOWN) { /* if the left button is clicked */
      /* copy mouse values into picking variables */
      mouseX = x;
      mouseY = y;
      DoPicking();
      //display may change
      glutPostRedisplay();
    }
  }
}

int main(int argc, char** argv) {

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(500, 500);
  glutCreateWindow("3d picking");
  
  glEnable(GL_DEPTH_TEST);
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutMouseFunc(mouse);
  
  glClearColor(1.0, 1.0, 1.0, 1.0);
  init_lighting();
  
  b=gluNewQuadric(); /* allocate quadric object */
  gluQuadricDrawStyle(b, GLU_FILL); /* render it as solid */
  gluQuadricNormals(b, GLU_SMOOTH);
  t=gluNewQuadric(); /* allocate quadric object */
  gluQuadricDrawStyle(t, GLU_FILL); /* render it as solid */
  gluQuadricNormals(t, GLU_SMOOTH);

  GW = 500;
  GH = 500;
  eye.x = 0.0; eye.y = 1.0; eye.z = 2.0;
  la.y = 1.0;
  pickbuf = 1.0;
  fov = 80;

  //start the glut main loop
  glutMainLoop();

}


