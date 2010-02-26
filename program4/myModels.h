#include <openglut.h>
void iceCream() {
	glutPushMatrix(); {
		// Rotate the whole cone to be oriented in the negative X direction
		glutRotatef(90, 1, 0, 0);
		glutSolidCone(.5, 3, 10, 10);
		glutPushMatrix(); {
			glutTranslatef(0, 0, -.5);
			glutSolidSphere(.6, 10, 10);
		} glutPopMatrix();
	} glutPopMatrix();
}

void drawSnowMan() {
	glutSolidSphere(1, 10, 10);
	glutPushMatrix(); {
		// Move it bottom sphere and middle sphere, minus a bit
		glTranslatef(0, 1.7, 0);
		glutSolidSphere(.8, 10, 10);

		glutPushMatrix(); {
			// Move it middle sphere and top sphere, minus a bit
			glTranslatef(0, 1.2, 0);
			glutSolidSphere(.5, 10, 10);
		} glutPopMatrix():

	} glutPopMatrix();

}
