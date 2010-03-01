#ifndef MYMODELS
#define MYMODELS
#include <GL/glut.h>
void iceCream() {
	glPushMatrix(); {
		// Rotate the whole cone to be oriented in the negative X direction
		glRotatef(90, 1, 0, 0);
		glutSolidCone(.5, 3, 10, 10);
		glPushMatrix(); {
			glTranslatef(0, 0, -.5);
			glutSolidSphere(.6, 10, 10);
		} glPopMatrix();
	} glPopMatrix();
}

void drawSnowMan() {
	glutSolidSphere(1, 10, 10);
	glPushMatrix(); {
		// Move it bottom sphere and middle sphere, minus a bit
		glTranslatef(0, 1.7, 0);
		glutSolidSphere(.8, 10, 10);

		glPushMatrix(); {
			// Move it middle sphere and top sphere, minus a bit
			glTranslatef(0, 1.2, 0);
			glutSolidSphere(.5, 10, 10);
		} glPopMatrix();

	} glPopMatrix();

}

void drawPlane() {
	glPushMatrix(); {
		glColor3f(1.0, 1.0, 0.0);
		glScalef(1000, 1, 1000);
		glBegin(GL_QUADS); {
			glVertex3f(-1, -1, -1);
			glVertex3f(-1, -1,  1);
			glVertex3f( 1, -1,  1);
			glVertex3f(-1, -1,  1);
		} glEnd();
	} glPopMatrix();
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
#endif
