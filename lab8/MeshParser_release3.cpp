//basic program to read in a mesh file (of .m format from H. Hoppe)
//Hh code modified by ZJW for csc 471

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <GL/glut.h>
#include <assert.h>
#include <string.h>
#include <map>

using namespace std;

#define FLT_MIN 1.1754E-38F
#define FLT_MAX 1.1754E+38F

inline float deg2rad(int deg) {
	   return M_PI * deg / 180.0;
}

//very simple data structure to store 3d points
typedef struct Vector3
{
  float x;
  float y;
  float z;
  Vector3(float in_x, float in_y, float in_z) : x(in_x), y(in_y), z(in_z) {}
  Vector3() {}
  Vector3 crossProd(const Vector3 &right) const {
	  return Vector3(y * right.z - z * right.y,
			            z * right.x - x * right.z,
							x * right.y - y * right.x);
  }
  float length() {
	  return sqrtf(x * x + y * y + z * z);
  }
  void normalize(float scale = 1.0) {
	  float len = this->length();
	  //printf("Normalizing vector - x: %f, y: %f, z: %f, length: %f\n", x,y,z,len);
	  x /= (len / scale);
	  y /= (len / scale);
	  z /= (len / scale);
  }
  Vector3 &operator*=(int mult) {
	  x *= mult;
	  y *= mult;
	  z *= mult;
	  return *this;
  }

  Vector3 operator*(float mult) {
	  return Vector3(x * mult, y * mult, z * mult);
  }
} Vector3;

//data structure to store triangle - 
//note that v1, v2, and v3 are indexes into the vertex array
typedef struct Tri{
  int v1;
  int v2;
  int v3;
  Vector3 normal;
  Vector3 color;
  Tri(int in_v1, int in_v2, int in_v3) : v1(in_v1), v2(in_v2), v3(in_v3), normal(0, 1, 0){}
  Tri() : normal(0, 1, 0) {}
} Tri;

//stl vector to store all the triangles in the mesh
vector<Tri *> Triangles;
//stl vector to store all the vertices in the mesh
vector<Vector3 *> Vertices;

//for computing the center point and extent of the model
Vector3 center;
float max_x, max_y, max_z, min_x, min_y, min_z;
float max_extent;

//other globals
int GW;
int GH;
int display_mode = GL_LINE_LOOP;
#define ORTHO 1
#define PERS  2
int view_mode = ORTHO;
bool show_normals = false;

#define WorldW 2.0 *(float)GW / (float)GH
#define WorldH 2.0
inline float p2w_x(float w) {
   //float WorldW = 2 * (float) GW / (float) GH;
   return WorldW / GW * w - WorldW / 2;
}

inline float p2w_y(float h) {
   return -1 * (WorldH / GH * h - WorldH / 2);
}


// a bunch of lighting stuff I jacked from simple_light
int light = 0;
//globals for lighting - use a white light and apply materials
//light position
GLfloat light_pos[4] = {1.0, 1.0, 1.5, 1.0};
//light color (ambiant, diffuse and specular)
GLfloat light_amb[4] = {0.6, 0.6, 0.6, 1.0};
GLfloat light_diff[4] = {0.6, 0.6, 0.6, 1.0};
GLfloat light_spec[4] = {0.8, 0.8, 0.8, 1.0};
int mat = 0;
//set up some materials
typedef struct materialStruct {
  GLfloat ambient[4];
  GLfloat diffuse[4];
  GLfloat specular[4];
  GLfloat shininess[1];
} materialStruct;

materialStruct RedFlat = {
  {0.3, 0.0, 0.0, 1.0},
  {0.9, 0.0, 0.0, 1.0},
  {0.0, 0.0, 0.0, 1.0},
  {0.0}
};
materialStruct GreenShiny = {
  {0.0, 0.3, 0.0, 1.0},
  {0.0, 0.9, 0.0, 1.0},
  {0.2, 1.0, 0.2, 1.0},
  {8.0}
};

//sets up a specific material
void materials(materialStruct materials) {
  glMaterialfv(GL_FRONT, GL_AMBIENT, materials.ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, materials.diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, materials.specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, materials.shininess);
}
//initialization calls for opengl for static light
//note that we still need to enable lighting in order for it to work
//see keyboard 'l' event
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


//forward declarations of functions
void readLine(char* str);
void readStream(istream& is);
void drawTri(Tri * t);
void drawObjects();
void computeNormals();
void drawNormals();
void display();

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
    Vector3* v;
    if (sscanf(str,"Vertex %d %g %g %g",&vi,&x,&y,&z) !=4)
    {
      printf("an error occurred in reading vertices\n");
#ifdef _DEBUG
      exit(EXIT_FAILURE);
#endif
    }
    v = new Vector3(x, y, z);
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

//drawing routine to draw triangles as wireframe
void drawTria(Tri* t) {
	glBegin(display_mode); {
		glColor3f(0.0, 0.0, 0.5);
		//note that the vertices are indexed starting at 0, but the triangles
		//index them starting from 1, so we must offset by -1!!!
		glNormal3f(t->normal.x, t->normal.y, t->normal.z);
		glVertex3f(Vertices[t->v1 - 1]->x, 
				Vertices[t->v1 - 1]->y,
				Vertices[t->v1 - 1]->z);
		glVertex3f(Vertices[t->v2 - 1]->x, 
				Vertices[t->v2 - 1]->y,
				Vertices[t->v2 - 1]->z);
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
  if (view_mode == ORTHO)
    glOrtho( -2.0*(float)w/h, 2.0*(float)w/h, -2.0, 2.0, 1.0, 15.0);
  else if (view_mode == PERS)
	  gluPerspective(90, w/h, 1.0, 15.0);
  //else... fill in
  glMatrixMode(GL_MODELVIEW);
  glViewport(0, 0, w, h);
  
  glutPostRedisplay();
}

void drawObjects() {
  
  //transforms for the mesh
  glPushMatrix();
  //leave these transformations in as they center and scale each mesh correctly
  //scale object to window
  glScalef(1.0/(float)max_extent, 1.0/(float)max_extent, 1.0/(float)max_extent);
  //translate the object to the orgin
  glTranslatef(-(center.x), -(center.y), -(center.z));
  //draw the wireframe mesh
  for(unsigned int j = 0; j < Triangles.size(); j++) {
    drawTria(Triangles[j]);
  }
  if (show_normals)
	  drawNormals();
  glPopMatrix();
  
  //transforms for the sphere
  glPushMatrix();
  //draw the glut sphere behind the mesh
  glTranslatef(1.25, 0.0, -2.0);
  drawSphere();
  glPopMatrix();
}

void computeNormals() {
	for (int i = 0; i < Triangles.size(); i++) {
		Tri *current = Triangles[i];
		Vector3 *top = Vertices[current->v1 - 1];
		Vector3 *left = Vertices[current->v2 - 1];
		Vector3 *right = Vertices[current->v3 - 1];

		Vector3 leftEdge = Vector3(top->x - left->x,
				                     top->y - left->y,
				                     top->z - left->z);
		Vector3 rightEdge = Vector3(top->x - right->x,
				                      top->y - right->y,
				                      top->z - right->z);

		current->normal = leftEdge.crossProd(rightEdge);
		current->normal.normalize();
		//printf("normalized: %f, %f, %f, %f\n", current->normal.x, current->normal.y, current->normal.z, current->normal.length());
	}
}
void drawNormals() {
	glBegin(GL_LINES); {
		glColor3f(1.0, 0, 0);
		for (int i = 0; i < Triangles.size(); i++) {
			Tri *current = Triangles[i];
			Vector3 *top = Vertices[current->v1 - 1];
			Vector3 *left = Vertices[current->v2 - 1];
			Vector3 *right = Vertices[current->v3 - 1];

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
GLdouble eyeX = 0, eyeY = 0, eyeZ = 3.0;
GLdouble lookX, lookY, lookZ;
void display() {
  
  float numV = Vertices.size();
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glMatrixMode(GL_MODELVIEW);
    
  glPushMatrix();
  //set up the camera
  gluLookAt(eyeX, eyeY, eyeZ, lookX, lookY, lookZ, 0, 1, 0);
  printf("now looking from %f, %f, %f at %f, %f, %f\n", eyeX, eyeY, eyeZ, lookX, lookY, lookZ);

  pos_light();
    
  drawObjects();

  glPopMatrix();
    
  glutSwapBuffers();
    
}

#define MODE_TRANSLATE 1
#define MODE_ZOOM      2

int mouse_mode = MODE_TRANSLATE;
int lastMouseX, lastMouseY;
void mouse(int button, int state, int x, int y) {
	// Move camera in X,Y
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			mouse_mode = MODE_TRANSLATE;

			lastMouseX = x;
			lastMouseY = y;
		}
	}
	if (button == GLUT_RIGHT_BUTTON) {
		if (state == GLUT_DOWN) {
			mouse_mode = MODE_ZOOM;

			lastMouseY = y;
		}
	}
}

#define MOVE_AMT .05
void mouseMove(int x, int y) {
	if (mouse_mode == MODE_TRANSLATE) {
		eyeX -= p2w_x(lastMouseX) - p2w_x(x);
		eyeY -= p2w_y(lastMouseY) - p2w_y(y);
		lookX -= p2w_x(lastMouseX) - p2w_x(x);
		lookY -= p2w_y(lastMouseY) - p2w_y(y);
		lastMouseX = x;
		lastMouseY = y;
		glutPostRedisplay();
	}
	else if (mouse_mode == MODE_ZOOM) {
		eyeZ += p2w_x(lastMouseX) - p2w_x(x);
		lookZ += p2w_x(lastMouseX) - p2w_x(x);
		eyeZ += p2w_y(lastMouseY) - p2w_y(y);
		lookZ += p2w_y(lastMouseY) - p2w_y(y);
		lastMouseY = y;
		lastMouseX = x;
		glutPostRedisplay();
	}
}

void keyboard(unsigned char key, int x, int y) {
	switch( key) {
	case 'n': case 'N':
		show_normals = !show_normals;
		break;
	case 'e': case 'E':
		display_mode = display_mode == GL_LINE_LOOP ? GL_TRIANGLES : GL_LINE_LOOP;
		break;
	case 'l':
		light = !light;
		if (light)
			glEnable(GL_LIGHTING);
		else
			glDisable(GL_LIGHTING);
		break;
		//simple way to toggle the materials
	case 'm':
		mat++;
		if (mat%2 == 0)
			materials(RedFlat);
		else if (mat%2 == 1)
			materials(GreenShiny);
		break;
	case 'v': case 'V':
		view_mode = view_mode == ORTHO ? PERS : ORTHO;
		reshape(GW,GH);
		break;
	case 'r': case 'R':
		lookX = center.x;
		lookY = center.y;
		lookZ = center.z;
		eyeX = 0, eyeY = 0, eyeZ = 3.0;
		break;


	case 'q': case 'Q':
		exit(0);
		break;
	}
	glutPostRedisplay();
}



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
  //enable z-buffer
  glEnable(GL_DEPTH_TEST);
  
  //initialization
  max_x = max_y = max_z = FLT_MIN;
  min_x = min_y = min_z = FLT_MAX;
  center.x = 0;
  center.y = 0;
  center.z = 0;
//  display_mode = 0;
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
    
    lookX = center.x = center.x/Vertices.size();
    lookY = center.y = center.y/Vertices.size();
    lookZ = center.z = center.z/Vertices.size();
    //cout << "center " << center.x << " " << center.y << " " << center.z << endl;
    //cout << "scale by " << 1.0/(float)max_extent << endl;
  } else {
    cout << "format is: meshparser filename" << endl;
	 exit(0);
  }

  computeNormals();
  init_lighting();
  glEnable(GL_NORMALIZE);
  glutMainLoop();
}

