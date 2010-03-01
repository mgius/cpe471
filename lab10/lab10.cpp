//for CSC 471 - texture mapping lab
//release texture mapping code - press "c" to toggle thru 2 modes - start with 
//multiple materials then spherical world - add the complete world cube
//add your trackball code to rotate the object and play with the texture coordinates
//ZJ WOOD

#include <stdlib.h>
#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <assert.h>
#include <vector>

using namespace std;

int GW;
int GH;
int startx, starty, endx, endy;
int view;

char textures[3][20] = {"textures/crate.bmp", 
	                     "textures/cloud5.bmp", 
								"textures/earth1.bmp"};

/*data structure for the image used for  texture mapping */
typedef struct Image {
  unsigned long sizeX;
  unsigned long sizeY;
  char *data;
} Image;

Image *TextureImage;

typedef struct RGB {
  GLubyte r;
  GLubyte g; 
  GLubyte b;
} RGB;

RGB myimage[64][64];
RGB* pixel;

//forward declaration of image loading and texture set-up code
int ImageLoad(char *filename, Image *image);
GLvoid LoadTexture(char* image_file, int tex_id);
void init_tex();

//a sphere for drawing a spherical world
GLUquadricObj* mySphere;

//mode to toggle drawing
int cube;

typedef struct Vector3 {
  float x, y, z;
} Vector3;


Vector3 cross(Vector3 a, Vector3 b) {
  Vector3 c;
  c.x = a.y*b.z - a.z*b.y;
  c.y = a.z*b.x - a.x*b.z;
  c.z = a.x*b.y - a.y*b.x;
  
  return(c);
}

float dot(Vector3 a, Vector3 b) {
  return(a.x*b.x + a.y*b.y + a.z*b.z);
}

//code to create a checker board texture...
void makeCheckerBoard ( int nRows, int nCols )
{
  pixel = new RGB[nRows * nCols];
  int c;
  
  long count = 0;
  for ( int i=0; i < nRows; i++ ) {
    for ( int j=0; j < nCols; j++ ) {
      count = j*nCols +i;
      c = (((i/8) + (j/8)) %2) * 255;
      pixel[count].r = c;
      pixel[count].g = c;
      pixel[count].b = c;
      
    }
  }
}

//draw a texture mapped sphere
void drawSphere() {
  
  glBindTexture(GL_TEXTURE_2D, 3);
  gluSphere(mySphere, 1.0, 12, 12);
}


//original texture mapped cube drawing
void drawcube() {
  
  //set the current texture to be the crate
  glBindTexture(GL_TEXTURE_2D, 1);
  glBegin(GL_POLYGON);
  glColor3f(1.0, 1.0, 1.0); //white
  glTexCoord2f(0.0, 0.0);
  glVertex3f(-0.5, 0.5, 0.5); //v5
  glTexCoord2f(0.0, 1.0);
  glVertex3f(0.5, 0.5, 0.5); //v6
  glTexCoord2f(1.0, 1.0);
  glVertex3f(0.5, -0.5, 0.5); //v7
  glTexCoord2f(1.0, 0.0);
  glVertex3f(-0.5, -0.5, 0.5); //v8
  glEnd();
  
  //set the current texture to be the checkerboard image
  glBindTexture(GL_TEXTURE_2D, 0);     
  glBegin(GL_POLYGON);
  glColor3f(1.0, 1.0, 1.0); //white
  glTexCoord2f(0.0, 0.0);
  glVertex3f(-0.5, -0.5, -0.5); //v1
  glTexCoord2f(0.0, 1.0);
  glVertex3f(0.5, -0.5, -0.5); //v2
  glTexCoord2f(1.0, 1.0);
  glVertex3f(0.5, 0.5, -0.5); //v3
  glTexCoord2f(1.0, 0.0);
  glVertex3f(-0.5, 0.5, -0.5); //v4
  glEnd();
  
   //set the rest of the textures to be the earth
  glBindTexture(GL_TEXTURE_2D, 3);
  glBegin(GL_POLYGON);
  glColor3f(1.0, 1.0, 1.0); //white
  glTexCoord2f(0.0, 0.0);
  glVertex3f(-0.5, -0.5, -0.5); //v1
  glTexCoord2f(0.0, 1.0);
  glVertex3f(-0.5, 0.5, -0.5); //v4
  glTexCoord2f(1.0, 1.0);
  glVertex3f(-0.5, 0.5, 0.5); //v5
  glTexCoord2f(1.0, 0.0);
  glVertex3f(-0.5, -0.5, 0.5); //v8
  glEnd();
   
     //set the rest of the textures to be clouds
  glBindTexture(GL_TEXTURE_2D, 2);
  glBegin(GL_POLYGON);
  glColor3f(1.0, 1.0, 1.0); //white
  glTexCoord2f(0.0, 0.0);
  glVertex3f(-0.5, 0.5, -0.5); //v4
  glTexCoord2f(0.0, 1.0);
  glVertex3f(0.5, 0.5, -0.5); //v3
  glTexCoord2f(1.0, 1.0);
  glVertex3f(0.5, 0.5, 0.5); //v6
  glTexCoord2f(1.0, 0.0);
  glVertex3f(-0.5, 0.5, 0.5); //v5
  glEnd();
  
  glBegin(GL_POLYGON);
  glColor3f(1.0, 1.0, 1.0); //white
  glTexCoord2f(0.0, 0.0);
  glVertex3f(0.5, 0.5, -0.5); //v3
  glTexCoord2f(0.0, 1.0);
  glVertex3f(0.5, -0.5, -0.5); //v2
  glTexCoord2f(1.0, 1.0);
  glVertex3f(0.5, -0.5, 0.5); //v7
  glTexCoord2f(1.0, 0.0);
  glVertex3f(0.5, 0.5, 0.5); //v6
  glEnd();
  
 
  glBegin(GL_POLYGON);
  glColor3f(1.0, 1.0, 1.0); //white
  glTexCoord2f(0.0, 0.0);
  glVertex3f(-0.5, -0.5, -0.5); //v1
  glTexCoord2f(0.0, 1.0);
  glVertex3f(0.5, -0.5, -0.5); //v2
  glTexCoord2f(1.0, 1.0);
  glVertex3f(0.5, -0.5, 0.5); //v7
  glTexCoord2f(1.0, 0.0);
  glVertex3f(-0.5, -0.5, 0.5); //v8
  glEnd();
  
}

void display() {
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  //set up the camera
  gluLookAt(-1.0,1.0,2.0, 0.0,0.0,0.0, 0.0,1.0,0.0);

  if (cube == 0)
    drawcube();
  else if (cube == 1)
    drawSphere();
  //else
    //add 3rd case
  glPopMatrix();
  
  glutSwapBuffers();
}

void reshape(int w, int h) {
  GW = w;
  GH = h;
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (view == 0)
    glOrtho(-1.5*(float)w/h, 1.5*(float)w/h, -1.5, 1.5, 0.5, 4.0);
  else
    gluPerspective(60.0, (float)w/h, 0.5, 15.0);
  glMatrixMode(GL_MODELVIEW);
  glViewport(0, 0, w, h);
  
}



void keyboard(unsigned char key, int x, int y )
{
  switch( key ) {
  case 'q': case 'Q' :
    exit( EXIT_SUCCESS );
    break;
  case 'c':
    cube++;
    cube= cube%3;
    break;
  case 'v':
    view = !view;
    reshape(GW, GH);
  }
  glutPostRedisplay();
}

//general intialization for opengl for depth and texture mapping
void init() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);
}

//initialization for texture mapping
void init_tex() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, 3, 64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, pixel);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

int main(int argc, char** argv) {
  
  //initialize the window
  glutInit(&argc, argv);
  glutInitWindowSize(400, 400);
  glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow("My First texture maps");
  glClearColor(0.0, 0.0, 0.0, 1.0);
  
  //set up the opengl call backs
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  
  //initialize globals
  GW = GH = 400;
  cube = 0;
  view = 0;

  //opengl initializations
  init();

  //make the checker board image
  makeCheckerBoard(64, 64);
  //set up the checkerboard texture
  init_tex();
  //load in the other image textures
  LoadTexture(textures[0], 1);
  LoadTexture(textures[1], 2);
  LoadTexture(textures[2], 3);
  
  //create a sphere
  mySphere = gluNewQuadric();
  gluQuadricDrawStyle(mySphere, GLU_FILL);
  //make opengl generate the texture coordinates for the sphere
  gluQuadricTexture(mySphere, GL_TRUE);
  
  glutMainLoop();
  
}

//routines to load in a bmp files - must be 2^nx2^m and a 24bit bmp
GLvoid LoadTexture(char* image_file, int texID) { 
  
  TextureImage = (Image *) malloc(sizeof(Image));
  if (TextureImage == NULL) {
    printf("Error allocating space for image");
    exit(1);
  }
  cout << "trying to load " << image_file << endl;
  if (!ImageLoad(image_file, TextureImage)) {
    exit(1);
  }  
  /*  2d texture, level of detail 0 (normal), 3 components (red, green, blue),            */
  /*  x size from image, y size from image,                                              */    
  /*  border 0 (normal), rgb color data, unsigned byte data, data  */ 
  glBindTexture(GL_TEXTURE_2D, texID);
  glTexImage2D(GL_TEXTURE_2D, 0, 3,
    TextureImage->sizeX, TextureImage->sizeY,
    0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST); /*  cheap scaling when image bigger than texture */    
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); /*  cheap scaling when image smalled than texture*/
  
}


/* BMP file loader loads a 24-bit bmp file only */

/*
* getint and getshort are help functions to load the bitmap byte by byte
*/
static unsigned int getint(FILE *fp) {
  int c, c1, c2, c3;
  
  /*  get 4 bytes */ 
  c = getc(fp);  
  c1 = getc(fp);  
  c2 = getc(fp);  
  c3 = getc(fp);
  
  return ((unsigned int) c) +   
    (((unsigned int) c1) << 8) + 
    (((unsigned int) c2) << 16) +
    (((unsigned int) c3) << 24);
}

static unsigned int getshort(FILE *fp){
  int c, c1;
  
  /* get 2 bytes*/
  c = getc(fp);  
  c1 = getc(fp);
  
  return ((unsigned int) c) + (((unsigned int) c1) << 8);
}

/*  quick and dirty bitmap loader...for 24 bit bitmaps with 1 plane only.  */

int ImageLoad(char *filename, Image *image) {
  FILE *file;
  unsigned long size;                 /*  size of the image in bytes. */
  unsigned long i;                    /*  standard counter. */
  unsigned short int planes;          /*  number of planes in image (must be 1)  */
  unsigned short int bpp;             /*  number of bits per pixel (must be 24) */
  char temp;                          /*  used to convert bgr to rgb color. */
  
  /*  make sure the file is there. */
  if ((file = fopen(filename, "rb"))==NULL) {
    printf("File Not Found : %s\n",filename);
    return 0;
  }
  
  /*  seek through the bmp header, up to the width height: */
  fseek(file, 18, SEEK_CUR);
  
  /*  No 100% errorchecking anymore!!! */
  
  /*  read the width */    image->sizeX = getint (file);
  
  /*  read the height */ 
  image->sizeY = getint (file);
  
  /*  calculate the size (assuming 24 bits or 3 bytes per pixel). */
  size = image->sizeX * image->sizeY * 3;
  
  /*  read the planes */    
  planes = getshort(file);
  if (planes != 1) {
    printf("Planes from %s is not 1: %u\n", filename, planes);
    return 0;
  }
  
  /*  read the bpp */    
  bpp = getshort(file);
  if (bpp != 24) {
    printf("Bpp from %s is not 24: %u\n", filename, bpp);
    return 0;
  }
  
  /*  seek past the rest of the bitmap header. */
  fseek(file, 24, SEEK_CUR);
  
  /*  read the data.  */
  image->data = (char *) malloc(size);
  if (image->data == NULL) {
    printf("Error allocating memory for color-corrected image data");
    return 0; 
  }
  
  if ((i = fread(image->data, size, 1, file)) != 1) {
    printf("Error reading image data from %s.\n", filename);
    return 0;
  }
  
  for (i=0;i<size;i+=3) { /*  reverse all of the colors. (bgr -> rgb) */
    temp = image->data[i];
    image->data[i] = image->data[i+2];
    image->data[i+2] = temp;
  }
  
  fclose(file); /* Close the file and release the filedes */
  
  /*  we're done. */
  return 1;
}




