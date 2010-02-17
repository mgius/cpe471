#include "Vector3D.h"
#include "math.h"
#include "GL/glut.h"
#include <stdio.h>

// Sets the direction/magnitude of the vector.  Doesn't move it

void Vector3D::set(float _x, float _y, float _z) {
	x = _x;
	y = _y;
	z = _z;
}

// Draws the vector.  Currently draws in 2D space
void Vector3D::draw() const {
	//printf("Drawing vector at %f, %f, going to %f, %f\n",
	//		xPos, yPos, xPos + x, yPos + y);

	glColor3f(1.0, 0,0);
	int oldPointSize;
	glGetIntegerv(GL_POINT_SIZE, &oldPointSize);
	glPointSize(5);
	glBegin(GL_POINTS);
	{
		glVertex2f(xPos, yPos);
	}
	glEnd();
	glPointSize(oldPointSize);


	glBegin(GL_LINES);
	{
		glVertex2f(xPos, yPos);
		glVertex2f(xPos + x, yPos + y);
	}
	glEnd();
}


// Returns the dot product of the left and right vector
float Vector3D::dotProd(const Vector3D &right) const {
	return x * right.x + y * right.y + z * right.z;

}


// Returns the cross product of the left and right vector, anchored at 0,0,0
Vector3D Vector3D::crossProd(const Vector3D &right) const {
	return Vector3D(y * right.z - z * right.y,
			          z * right.x - x * right.z,
						 x * right.y - y * right.x);
}

Vector3D &Vector3D::translate(float _x, float _y, float _z) {
	xPos += _x;
	yPos += _y;
	zPos += _z;
	return *this;
}

// Assuming Z is zero, bind a "z" such that this vector has length 1
// TODO: make this handle vectors already 1 or longer
void Vector3D::bindZ() {
	float underTheSqrt = 1.0 - x*x - y*y;
	z = underTheSqrt < 0 ? 0 : sqrtf(underTheSqrt);
}

void Vector3D::normalize(float scale) {
	float len = this->length();
	//printf("Normalizing vector - x: %f, y: %f, z: %f, length: %f\n", x,y,z,le
	x /= (len / scale);
	y /= (len / scale);
	z /= (len / scale);
}

void Vector3D::scaleToOne() {
	if (length() > 1.0) {
		normalize();
	}
}

// Returns the length of the vector (which happens to be the sqrt of dotProd)
float Vector3D::length() const {
	return sqrtf(x *x + y*y + z*z);
}

// Two vectors are the same if their direction, length, and anchor are the same
bool Vector3D::operator==(const Vector3D &right) const {
	return x == right.x and y == right.y and z == right.z and
		xPos == right.xPos and yPos == right.yPos and zPos == right.zPos;
}

// Adds the two vectors together.  Uses the left vectors position
Vector3D Vector3D::operator+(const Vector3D &right) const {
	return Vector3D(x + right.x, y + right.y, z + right.z, xPos, yPos, zPos);
}

// Adds the right vector to this one. Leaves position unmodified
Vector3D &Vector3D::operator+=(const Vector3D &right) {
	x += right.x;
	y += right.y;
	z += right.z;
	return *this;
}

// Subtracts one vector from another  Uses the left vectors position
Vector3D Vector3D::operator-(const Vector3D &right) const {
	return Vector3D(x - right.x, y - right.y, z - right.z, xPos, yPos, zPos);
}

// Adds the right vector to this one. Leaves position unmodified
Vector3D &Vector3D::operator-=(const Vector3D &right) {
	x -= right.x;
	y -= right.y;
	z -= right.z;
	return *this;
}

