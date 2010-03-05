from OpenGL.GLUT import *
from OpenGL.GLU  import *
from OpenGL.GL   import *

from math import pi

# Camera globals
(eyeX, eyeY, eyeZ) = (0.0, 1.0, 3.0)
(lookX, lookY, lookZ) = (0.0, 1.0, 2.0)
(upX, upY, upZ) = (0.0, 1.0, 0.0)

phi = 0.0;
theta = 272.0;

# Lighting Globals
light_pos = [1.0, 20.0, 1.5, 1.0]
light_amb = [1.0, 1.0, 1.0, 1.0]
light_diff = [0.6, 0.6, 0.6, 1.0]
light_spec = [0.8, 0.8, 0.8, 1.0]

def deg2rad(deg):
   return pi * deg / 180.0

def rad2deg(rad):
   return rad * 180.0 / pi

# materials of format dictionary, with values being a tuple of 4 lists  
# the 4 lists are ambient, diffuse, specular and sininess in order

materials = { 'redflat': ( 
		[0.3, 0.0, 0.0, 1.0],
		[0.9, 0.0, 0.0, 1.0],
		[0.0, 0.0, 0.0, 1.0],
		[0.0]
	),
	'greenshiny': (
		[0.0, 0.3, 0.0, 1.0],
		[0.0, 0.9, 0.0, 1.0],
		[0.2, 1.0, 0.2, 1.0],
		[8.0]
	),
	'blueflat': (
		[0.0, 0.0, 0.5, 1.0],
		[0.0, 0.0, 0.8, 1.0],
		[0.2, 0.2, 1.0, 1.0],
		[0.0]
	),
	'yellowflat': (
		[0.9, 1.0, 0.5, 1.0],
		[0.0, 0.0, 0.0, 1.0],
		[0.2, 0.2, 0.0, 1.0],
		[0.0]
	),
	'blackflat': (
		[0.1, 0.1, 0.1, 1.0],
		[0.1, 0.1, 0.1, 1.0],
		[0.1, 0.1, 0.1, 1.0],
		[0.0]
	)
}

# Mouse movement
(lastMouseX, lastMouseY) = (0,0)

void drawAxes();
void drawPlane();
void init_lighting();
void pos_light();
void setMaterial(materialStruct);

// forward declarations of functions I wrote
void drawObjects();
void display();
void drawIceCream();
void drawSnowMan();
void reshape(int, int);
void mouse(int,int,int,int);
void mouseMove(int, int);



#define SCALE_FACTOR .1
void keyboard(unsigned char key, int x, int y) {
	// 90% of keyboard input will be movement, and c++ complains about 
	// declarations within case statements, so calculate zoom and strafe
	// vector here even if we're not going to use it
	
	Vector3D zoom = look - eye;
	zoom.scaleToOne();
	Vector3D strafe = zoom.crossProd(Vector3D(0,1,0));
	Vector3D up = zoom.crossProd(strafe);

	switch( key) {
   case 'w': case 'W':
      // forward
		look += zoom * SCALE_FACTOR;
		eye += zoom * SCALE_FACTOR;
      break;
   case 'a': case 'A':
      // left strafe
		look -= strafe * SCALE_FACTOR;
		eye -= strafe * SCALE_FACTOR;
      break;
   case 's': case 'S':
      // back
		look -= zoom * SCALE_FACTOR;
		eye -= zoom * SCALE_FACTOR;
      break;
   case 'd': case 'D':
      // right strafe
		look += strafe * SCALE_FACTOR;
		eye += strafe * SCALE_FACTOR;
      break;
	case 'r' : case 'R' :
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glPopMatrix();
		eye.set(0.0, 1.0, 3.0);
		look.set(0.0, 0.0, 0.0);
		phi = -15.0;
		theta = 272.0;
		glutPostRedisplay();
		break;
	case 'q': case 'Q':
		exit(0);
		break;
	}
	// Also since 90% of keyboard input is movement, might as well do this
	// all the time and save myself some code lines
	if (eye.y < 1.0) {
		eye.y = 1.0;
		look.y = eye.y + sin(deg2rad(phi));
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

	init_lighting();

	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glutMainLoop();
}

// My 3d models.  Hooray
void drawIceCream() {
   glPushMatrix(); {
      // Rotate the whole cone to be oriented in the negative X direction
      glRotatef(90, 1, 0, 0);
		setMaterial(materials[MATERIAL_YELLOWFLAT]);
      glutSolidCone(.5, 3, 10, 10);
      glPushMatrix(); {
         glTranslatef(0, 0, -.5);
			setMaterial(materials[MATERIAL_GREENSHINY]);
         glutSolidSphere(.6, 10, 10);
      } glPopMatrix();
   } glPopMatrix();
}

void drawSnowMan() {
	setMaterial(materials[MATERIAL_BLUEFLAT]);
   glutSolidSphere(1, 10, 10);
   glPushMatrix(); {
      // Move it bottom sphere and middle sphere, minus a bit
      glTranslatef(0, 1.6, 0);
		setMaterial(materials[MATERIAL_BLACKFLAT]);
      glutSolidSphere(.8, 10, 10);

      glPushMatrix(); {
         // Move it middle sphere and top sphere, minus a bit
         glTranslatef(0, 1.1, 0);
			setMaterial(materials[MATERIAL_BLUEFLAT]);
         glutSolidSphere(.5, 10, 10);
      } glPopMatrix();

   } glPopMatrix();

}


// Helper functions go down here.  That's right, I'm segregationist
void init_lighting() {
	//turn on light0
	glEnable(GL_LIGHT0);
	//set up the diffuse, ambient and specular components for the light
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diff);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_spec);
	//specify our lighting model as 1 normal per face
	glShadeModel(GL_SMOOTH);
}

void pos_light() {
	//set the light's position
	glMatrixMode(GL_MODELVIEW);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
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

void drawPlane() {
	setMaterial(materials[MATERIAL_REDFLAT]);
	glBegin(GL_QUADS); {
		glVertex3f(-100.0, 0, -100.0);
		glVertex3f(-100.0, 0,  100.0);
		glVertex3f( 100.0, 0,  100.0);
		glVertex3f( 100.0, 0, -100.0);
	} glEnd();

}


void setMaterial(materialStruct material) {
	glMaterialfv(GL_FRONT, GL_AMBIENT, material.ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material.diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material.specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, material.shininess);
}

void reshape(int w, int h) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(90, (double)w/(double)h, 1.0, 15.0);
  glMatrixMode(GL_MODELVIEW);
  glViewport(0, 0, w, h);
  
  glutPostRedisplay();
}

void drawObjects() {
  
	// Snowmen
	for (int i = -3; i < 10; i++) {
		glPushMatrix(); {
			glTranslatef(i*2 + 1, 1.0, i * 3 );
			drawSnowMan();
		} glPopMatrix();
	}
	// IceCream
	for (int i = -5; i < 13; i++) {
		glPushMatrix(); {
			glTranslatef(i -1 , 1.0, -i);
			drawIceCream();
		} glPopMatrix();
	}

}

void display() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix(); {
	//set up the camera
	gluLookAt(eye.x, eye.y, eye.z, look.x, look.y, look.z, 0, 1, 0);
	
	printf("eye at %f, %f, %f looking at %f, %f, %f\n", eye.x, eye.y, eye.z, look.x, look.y, look.z);

	pos_light();

	glPushMatrix(); {
		glTranslatef(0, -.5, 0);
		drawPlane();
		drawObjects();
	} glPopMatrix();
	//drawAxes();

	} glPopMatrix();

	glutSwapBuffers();

}

void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) { /* if the left button is clicked */
			printf("left mouse clicked at %d %d\n", x, y);
			lastMouseX = x;
			lastMouseY = y;
		}
	}
}

#define ROTATE_SCALE .5
void mouseMove(int x, int y) {
	
	theta -= ROTATE_SCALE * (lastMouseX - x);
	phi += ROTATE_SCALE * (lastMouseY - y);
	//printf("theta %f, phi %f\n", theta, phi);

	if (phi > 50.0) { phi = 50.0; }
	if (theta > 360.0) { theta -= 360.0; }
	if (phi < -50.0) { phi = -50.0; }
	if (theta < 0.0) { theta += 360.0; }

	look.x = eye.x + cos(deg2rad(phi)) * cos(deg2rad(theta));
	look.y = eye.y + sin(deg2rad(phi));
	look.z = eye.z + cos(deg2rad(phi)) * cos(90 - deg2rad(theta));

	lastMouseX = x;
	lastMouseY = y;
	glutPostRedisplay();
}
#undef ROTATE_SCALE
