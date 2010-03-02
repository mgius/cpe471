//basic program to read in a mesh file (of .m format from H. Hoppe)
//Hh code modified by ZJW for csc 471

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <GL/glut.h>
#include <assert.h>
#include <map>
#include "Vector3D/Vector3D.h"

using namespace std;

#define FLT_MIN 1.1754E-38F
#define FLT_MAX 1.1754E+38F

// These all ripped from program2 for trackball functionality

int GW, GH;

#define WorldW 4.0 *(float)GW / (float)GH
#define WorldH 4.0
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

inline float rad2deg(float rad) {
   return rad * 180.0 / M_PI;
}

float findAngle(Vector3D &a, Vector3D &b) {
   return acos(a.dotProd(b) / (fabs(a.length()) * fabs(b.length())));
}

// this is modified from the simple_light provided files

GLfloat light_pos[4] = {1.0, 1.0, 1.5, 1.0};
GLfloat light_amb[4] = {0.6, 0.6, 0.6, 1.0};
GLfloat light_diff[4] = {0.6, 0.6, 0.6, 1.0};
GLfloat light_spec[4] = {0.8, 0.8, 0.8, 1.0};

#define MATERIAL_REDFLAT 0
#define MATERIAL_GREENSHINY 1

typedef struct materialStruct {
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat shininess[1];
} materialStruct;

#define MATERIAL_COUNT 2
materialStruct materials[MATERIAL_COUNT] = {
	{
		{0.3, 0.0, 0.0, 1.0},
		{0.9, 0.0, 0.0, 1.0},
		{0.0, 0.0, 0.0, 1.0},
		{0.0}
	},
	{
		{0.0, 0.3, 0.0, 1.0},
		{0.0, 0.9, 0.0, 1.0},
		{0.2, 1.0, 0.2, 1.0},
		{8.0}
	}
};

int current_material = MATERIAL_REDFLAT;

/*an example of a simple data structure to store a 4x4 matrix */
GLfloat objectM[4][4] = {
   {1.0, 0.0, 0.0, 0.0},
   {0.0, 1.0, 0.0, 0.0},
   {0.0, 0.0, 1.0, 0.0},
   {0.0, 0.0, 0.0, 1.0}
};


GLfloat *trackballM = (GLfloat *)objectM;

GLfloat translateM[3] = {0.0, 0.0, 0.0};
GLfloat scaleM[3] = {1.0, 1.0, 1.0};

Vector3D startClick, endClick;

//data structure to store triangle - 
//note that v1, v2, and v3 are indexes into the vertex array
typedef struct Tri{
  int v1;
  int v2;
  int v3;
  Vector3D normal;
  Vector3D color;
  Tri(int in_v1, int in_v2, int in_v3) : v1(in_v1), v2(in_v2), v3(in_v3), normal(0, 1, 0){}
  Tri() : normal(0, 1, 0) {}
} Tri;

//stl vector to store all the triangles in the mesh
vector<Tri *> Triangles;
//stl vector to store all the vertices in the mesh
vector<Vector3D *> Vertices;
//stl vector to store the normals per vertice
vector<Vector3D *> verticeNormals;

//for computing the center point and extent of the model
Vector3D center;
float max_x, max_y, max_z, min_x, min_y, min_z;
float max_extent;

// other globals
#define VIEW_ORTHO 1
#define VIEW_PERSP 2
int view_mode = VIEW_PERSP;
bool show_normals = false;
bool light = false;
GLenum display_mode = GL_LINE_LOOP;

int shading_mode = GL_FLAT;

//forward declarations of functions
void readLine(char* str);
void readStream(istream& is);
void drawTri(Tri * t);
void drawObjects();
void ReadFile(char* filename);
void printFirstThree();
void display();
void computeNormals();
void displayNormals();
void drawAxes();

void init_lighting() {
	//turn on light0
	glEnable(GL_LIGHT0);
	//set up the diffuse, ambient and specular components for the light
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diff);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_spec);
	//specify our lighting model as 1 normal per face
	glShadeModel(GL_FLAT);
}

void pos_light() {
	//set the light's position
	glMatrixMode(GL_MODELVIEW);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
}


void setMaterial(materialStruct material) {
	glMaterialfv(GL_FRONT, GL_AMBIENT, material.ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material.diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material.specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, material.shininess);
}


//drawing routine to draw triangles as wireframe
void drawTria(Tri* t) {
	glBegin(display_mode); {
		glColor3f(0.0, 0.0, 0.5);
		//note that the vertices are indexed starting at 0, but the triangles
		//index them starting from 1, so we must offset by -1!!!
		if (shading_mode == GL_FLAT) {
			glNormal3f(t->normal.x, t->normal.y, t->normal.z);
		}
		else if (shading_mode == GL_SMOOTH) {
			glNormal3f(verticeNormals[t->v1 - 1]->x,
					     verticeNormals[t->v1 - 1]->y,
					     verticeNormals[t->v1 - 1]->z);

		}
		glVertex3f(Vertices[t->v1 - 1]->x, 
				Vertices[t->v1 - 1]->y,
				Vertices[t->v1 - 1]->z);
		if (shading_mode == GL_SMOOTH) {
			glNormal3f(verticeNormals[t->v2 - 1]->x,
					     verticeNormals[t->v2 - 1]->y,
					     verticeNormals[t->v2 - 1]->z);

		}
		glVertex3f(Vertices[t->v2 - 1]->x, 
				Vertices[t->v2 - 1]->y,
				Vertices[t->v2 - 1]->z);
		if (shading_mode == GL_SMOOTH) {
			glNormal3f(verticeNormals[t->v3 - 1]->x,
					     verticeNormals[t->v3 - 1]->y,
					     verticeNormals[t->v3 - 1]->z);

		}
		glVertex3f(Vertices[t->v3 - 1]->x, 
				Vertices[t->v3 - 1]->y,
				Vertices[t->v3 - 1]->z);
	} glEnd();
}

void drawSphere() {
  glColor3f(1.0, 0.0, 0.0);
  glutWireSphere(0.35, 10, 10);
}

//debugging routine which just draws the vertices of the mesh
void DrawAllVerts() {
  
  glColor3f(1.0, 0.0, 1.0);
  glBegin(GL_POINTS);
  for (unsigned int k=0; k < Vertices.size(); k++) {
    glVertex3f(Vertices[k]->x, Vertices[k]->y, Vertices[k]->z);
  }
  glEnd();
  
}

void reshape(int w, int h) {
  GW = w;
  GH = h;
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (view_mode == VIEW_ORTHO)
    glOrtho( -2.0*(float)w/h, 2.0*(float)w/h, -2.0, 2.0, 1.0, 15.0);
  else if (view_mode == VIEW_PERSP) {
	  gluPerspective(90, (double)w/(double)h, 1.0, 15.0);
  }
  glMatrixMode(GL_MODELVIEW);
  glViewport(0, 0, w, h);
  
  glutPostRedisplay();
}

void drawObjects() {
  
	//transforms for the mesh
	glPushMatrix(); {
      // My transforms for trackball func
		glTranslatef(translateM[0], translateM[1], translateM[2]);
      glScalef(scaleM[0], scaleM[1], scaleM[2]);
      glMultMatrixf(trackballM);

		//leave these transformations in as they center and scale each mesh correctly
		//scale object to window
		glScalef(1.0/(float)max_extent, 1.0/(float)max_extent, 1.0/(float)max_extent);
		//translate the object to the orgin
		glTranslatef(-(center.x), -(center.y), -(center.z));
		//draw the wireframe mesh
		for(unsigned int j = 0; j < Triangles.size(); j++) {
			drawTria(Triangles[j]);
		}
	} glPopMatrix();

	//transforms for the sphere
	glPushMatrix(); {
		//draw the glut sphere behind the mesh
		glTranslatef(1.25, 0.0, -2.0);
		drawSphere();
	} glPopMatrix();
}

Vector3D eye(0.0,1.0,3.0);
Vector3D look(0.0, 1.0, 0.0);
void display() {

	float numV = Vertices.size();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	//set up the camera
	gluLookAt(eye.x, eye.y, eye.z, look.x, look.y, look.z, 0, 1, 0);

	pos_light();

	drawObjects();
	drawAxes();

	glPopMatrix();

	glutSwapBuffers();

}

void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) { /* if the left button is clicked */
			printf("left mouse clicked at %d %d\n", x, y);
			startClick = Vector3D(p2w_x(x), p2w_y(y),0);
			startClick.scaleToOne();
			startClick.bindZ();
			endClick = Vector3D(p2w_x(x), p2w_y(y),0);
			endClick.scaleToOne();
			endClick.bindZ();
		}
		if (state == GLUT_UP) {
		}
	}
	if (button == GLUT_RIGHT_BUTTON) {
		if (state == GLUT_DOWN) { /* if the left button is clicked */
			printf("right mouse clicked at %d %d\n", x, y);
		}
		if (state == GLUT_UP) {
		}
	}
	if (button == GLUT_MIDDLE_BUTTON) {
		if (state == GLUT_DOWN) { /* if the middle button is clicked */
			printf("middle mouse clicked at %d %d\n", x, y);
		}
		if (state == GLUT_UP) {
		}
	}
}

void mouseMove(int x, int y) {
	endClick = Vector3D(p2w_x(x), p2w_y(y), 0);
	endClick.scaleToOne();
	endClick.bindZ();
	Vector3D axisRot = startClick.crossProd(endClick);
	float angleInDeg = rad2deg(findAngle(startClick, endClick));
	//cout << "start: " << startClick <<  " length: " << startClick.length() <<
	//	cout << "end: " << endClick <<  " length: " << endClick.length() << endl;
	//cout << "axis: " << axisRot << endl;
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); {
		glLoadIdentity();
		glRotatef(angleInDeg, axisRot.getX(), axisRot.getY(), axisRot.getZ());
		glMultMatrixf(trackballM);
		glGetFloatv(GL_MODELVIEW_MATRIX, trackballM);
	} glPopMatrix();
	startClick = endClick;
	glutPostRedisplay();
}



void keyboard(unsigned char key, int x, int y) {
	switch( key) {
   case 'w': case 'W':
      // forward
		eye.z -= .05;
		look.z -= .05;
      break;
   case 'a': case 'A':
      // left strafe
		eye.x -= .05;
		look.x -= .05;
      break;
   case 's': case 'S':
      // back
		eye.z += .05;
		look.z += .05;
      break;
   case 'd': case 'D':
      // right strafe
		eye.x += .05;
		look.x += .05;
      break;
	case 'n': case 'N':
		show_normals = !show_normals;
		break;
	case 'e': case 'E':
		display_mode = display_mode == GL_LINE_LOOP ? GL_TRIANGLES : GL_LINE_LOOP;
		break;
	case 'r' : case 'R' :
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glGetFloatv(GL_MODELVIEW_MATRIX, trackballM);
		glPopMatrix();
		scaleM[0] = scaleM[1] = scaleM[2] = 1.0;
		translateM[0] = translateM[1] = translateM[2] = 0.0;
		eye = Vector3D(0.0, 1.0, 3.0);
		look = Vector3D(0.0, 1.0, 0.0);
		glutPostRedisplay();
		break;
	case 'l': case 'L':
		light = !light;
		if (light)
			glEnable(GL_LIGHTING);
		else
			glDisable(GL_LIGHTING);
		break;
		//simple way to toggle the materials
	case 'm': case 'M':
		static int mat = 0;
		mat++;
		setMaterial(materials[mat%MATERIAL_COUNT]);
		break;
	case 'v': case 'V':
		view_mode = view_mode == VIEW_ORTHO ? VIEW_PERSP : VIEW_ORTHO;
		printf("%s mode\n", view_mode == VIEW_ORTHO ? "Ortho" : "Persp");
		reshape(GW, GH);
		break;
	case 'h': case 'H':
		shading_mode = shading_mode == GL_FLAT ? GL_SMOOTH : GL_FLAT;
		printf("%s mode\n", shading_mode == GL_FLAT ? "Flat" : "Smooth");
		glShadeModel(shading_mode);
		break;
	case 'q': case 'Q':
		exit(0);
		break;
	}
	glutPostRedisplay();
}

//void mouseModeHandler(int value) {
//	mouse_mode = value;
//}
//
//void materialHandler(int value) {
//	setMaterial(materials[value]);
//}

int main( int argc, char** argv ) {

	//set up my window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(300, 300); 
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Mesh display");
	glClearColor(1.0, 1.0, 1.0, 1.0);

	//register glut callback functions
	glutDisplayFunc( display );
	glutReshapeFunc( reshape );
	glutKeyboardFunc( keyboard );
	glutMouseFunc( mouse );
	glutMotionFunc( mouseMove );

	//// set up right click menu

	//int materialMenu = glutCreateMenu(materialHandler);
	//glutAddMenuEntry("RedFlat", MATERIAL_REDFLAT);
	//glutAddMenuEntry("GreenShiny", MATERIAL_GREENSHINY);

	//int rightMenu = glutCreateMenu(mouseModeHandler);
	//glutAddMenuEntry("Trackball input", MODE_TRACKBALL);
	//glutAddMenuEntry("Light input", MODE_LIGHT);
	//glutAddSubMenu("Materials", materialMenu);

	//glutAttachMenu(GLUT_RIGHT_BUTTON);

	//enable z-buffer
	glEnable(GL_DEPTH_TEST);

	//initialization
	max_x = max_y = max_z = FLT_MIN;
	min_x = min_y = min_z = FLT_MAX;
	center.x = 0;
	center.y = 0;
	center.z = 0;
	max_extent = 1.0;

	//make sure a file to read is specified
	if (argc > 1) {
		cout << "file " << argv[1] << endl;
		//read-in the mesh file specified
		ReadFile(argv[1]);
		//only for debugging
		if (Vertices.size() > 4)
			printFirstThree();

		//once the file is parsed find out the maximum extent to center and scale mesh
		max_extent = max_x - min_x;
		if (max_y - min_y > max_extent) max_extent = max_y - min_y;
		//cout << "max_extent " << max_extent << " max_x " << max_x << " min_x " << min_x << endl;
		//cout << "max_y " << max_y << " min_y " << min_y << " max_z " << max_z << " min_z " << min_z << endl;

		center.x = center.x/Vertices.size();
		center.y = center.y/Vertices.size();
		center.z = center.z/Vertices.size();
		//cout << "center " << center.x << " " << center.y << " " << center.z << endl;
		//cout << "scale by " << 1.0/(float)max_extent << endl;
	} else {
		cout << "format is: meshparser filename" << endl;
		exit(1);
	}

	computeNormals();
	setMaterial(materials[MATERIAL_REDFLAT]);
	init_lighting();


	glEnable(GL_NORMALIZE);
	glutMainLoop();
}



// Ripped from lab7

void computeNormals() {
	for (int i = 0; i < Vertices.size(); i++) {
		verticeNormals.push_back(new Vector3D());
	}
   for (int i = 0; i < Triangles.size(); i++) {
      Tri *current = Triangles[i];
      Vector3D *top = Vertices[current->v1 - 1];
      Vector3D *left = Vertices[current->v2 - 1];
      Vector3D *right = Vertices[current->v3 - 1];

      Vector3D leftEdge = Vector3D(top->x - left->x,
                                  top->y - left->y,
                                  top->z - left->z);
      Vector3D rightEdge = Vector3D(top->x - right->x,
                                  top->y - right->y,
                                  top->z - right->z);

      current->normal = leftEdge.crossProd(rightEdge);
      current->normal.normalize();
		*verticeNormals[current->v1 - 1] += current->normal;
		*verticeNormals[current->v2 - 1] += current->normal;
		*verticeNormals[current->v3 - 1] += current->normal;
      //printf("normalized: %f, %f, %f, %f\n", current->normal.x, current->norma
   }
	for(int i = 0; i < Vertices.size(); i++) {
		verticeNormals[i]->normalize();
	}
}
void drawNormals() {
   glBegin(GL_LINES); {
      glColor3f(1.0, 0, 0);
      for (int i = 0; i < Triangles.size(); i++) {
         Tri *current = Triangles[i];
         Vector3D *top = Vertices[current->v1 - 1];
         Vector3D *left = Vertices[current->v2 - 1];
         Vector3D *right = Vertices[current->v3 - 1];

         float xBegin, xEnd, yBegin, yEnd, zBegin, zEnd;
         xBegin = 1.0 / 3.0 * (top->x + left->x + right->x);
         yBegin = 1.0 / 3.0 * (top->y + left->y + right->y);
         zBegin = 1.0 / 3.0 * (top->z + left->z + right->z);
         xEnd = xBegin + (current->normal).x * 0.02;
         yEnd = yBegin + (current->normal).y * 0.02;
         zEnd = zBegin + (current->normal).z * 0.02;
         //printf("Drawing normal at %f, %f, %f\n", xBegin, yBegin, zBegin);
         //printf("to %f, %f, %f\n", xEnd, yEnd, zEnd);
         glVertex3f(xBegin, yBegin, zBegin);
         glVertex3f(xEnd, yEnd, zEnd);
      }
   } glEnd();
}














// Provided functions

//open the file for reading
void ReadFile(char* filename) {
  
  printf("Reading coordinates from %s\n", filename);
  
  ifstream in_f(filename);
  if (!in_f)  {
    printf("Could not open file %s\n", filename);
  } else {
    readStream(in_f);
  }
  
}

//process the input stream from the file
void readStream(istream& is)
{
  char str[256];
  for (;is;) {
    is >> ws;
    is.get(str,sizeof(str));
    if (!is) break;
    is.ignore(9999,'\n');
    readLine(str);
  }
}

//process each line of input save vertices and faces appropriately
void readLine(char* str) {
  
  int indx = 0, vi;
  float x, y, z;
  float r, g, b;
  int mat;
  
  if (str[0]=='#') return;
  //read a vertex or face
  if (str[0]=='V' && !strncmp(str,"Vertex ",7)) {
    Vector3D* v;
    if (sscanf(str,"Vertex %d %g %g %g",&vi,&x,&y,&z) !=4)
    {
      printf("an error occurred in reading vertices\n");
#ifdef _DEBUG
      exit(EXIT_FAILURE);
#endif
    }
    v = new Vector3D(x, y, z);
    //store the vertex
    Vertices.push_back(v);
    //house keeping to display in center of the scene
    center.x += v->x;
    center.y += v->y;
    center.z += v->z;
    if (v->x > max_x) max_x = v->x; if (v->x < min_x) min_x = v->x;
    if (v->y > max_y) max_y = v->y; if (v->y < min_y) min_y = v->y;
    if (v->z > max_z) max_z = v->z; if (v->z < min_z) min_z = v->z;
  } else if (str[0]=='F' && !strncmp(str,"Face ",5)) {
    Tri* t;
    t = new Tri();
    char* s=str+4;
    int fi=-1;
    for (int t_i = 0;;t_i++) {
      while (*s && isspace(*s)) s++;
      //if we reach the end of the line break out of the loop
      if (!*s) break;
      //save the position of the current character
      char* beg=s;
      //advance to next space
      while (*s && isdigit(*s)) s++;
      //covert the character to an integer
      int j=atoi(beg);
      //the first number we encounter will be the face index, don't store it
      if (fi<0) { fi=j; continue; }
      //otherwise process the digit we've grabbed in j as a vertex index
      //the first number will be the face id the following are vertex ids
      if (t_i == 1)
        t->v1 = j;
      else if (t_i == 2)
        t->v2 = j;
      else if (t_i == 3)
        t->v3 = j;
      //if there is more data to process break out
      if (*s =='{') break;
    }
    //possibly process colors if the mesh has colors
    if (*s && *s =='{'){
      char *s1 = s+1;
      cout << "trying to parse color " << !strncmp(s1,"rgb",3) << endl;
      //if we're reading off a color
      if (!strncmp(s1,"rgb=",4)) {
        //grab the values of the string
        if (sscanf(s1,"rgb=(%g %g %g) matid=%d",&r,&g,&b,&mat)!=4)
        {
           printf("error during reading rgb values\n");
#ifdef _DEBUG
           exit(EXIT_FAILURE);
#endif
        }
        t->color.x = r; t->color.x = g; t->color.x = b;
        cout << "set color to: " << r << " " << g << " " << b << endl;
      }
    }
    //store the triangle read in
    Triangles.push_back(t);
  }
}

//testing routine for parser - left in just as informative code about accessing data
void printFirstThree() {
  printf("first vertex: %f %f %f \n", Vertices[0]->x, Vertices[0]->y, Vertices[0]->z);
  printf("first face: %d %d %d \n", Triangles[0]->v1, Triangles[0]->v2, Triangles[0]->v3);    
  printf("second vertex: %f %f %f \n", Vertices[1]->x, Vertices[1]->y, Vertices[1]);
  printf("second face: %d %d %d \n", Triangles[1]->v1, Triangles[1]->v2, Triangles[1]->v3);
  printf("third vertex: %f %f %f \n", Vertices[2]->x, Vertices[2]->y, Vertices[2]->z);
  printf("third face: %d %d %d \n", Triangles[2]->v1, Triangles[2]->v2, Triangles[2]->v3);
}
void drawAxes() {
   GLfloat oldWidth;
   glGetFloatv(GL_LINE_WIDTH, &oldWidth);
   glLineWidth(5.0);
   // X axis
   glBegin(GL_LINES); {
      glColor3f(0.5, 0, 0);
      glVertex3f(-50.0, 0, 0);
      glVertex3f(50.0,0,0);
   } glEnd();

   // Y axis
   glBegin(GL_LINES); {
      glColor3f(0, 0.5, 0);
      glVertex3f(0, -50.0, 0);
      glVertex3f(0,50.0,0);
   } glEnd();

   // Z axis
   glBegin(GL_LINES); {
      glColor3f(0, 0, .5);
      glVertex3f(0, 0, -50.0);
      glVertex3f(0,0,50.0);
   } glEnd();

   glLineWidth(oldWidth);
}

