#include "Vector3D.h"
#include "math.h"

// Draws the vector.  Currently draws in 2D space
void Vector3D::draw() const {
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

// Returns the length of the vector (which happens to be the sqrt of dotProd)
float Vector3D::length() const {
	return sqrt(dotProd(*this));
}

// Two vectors are the same if their direction, length, and anchor are the same
bool Vector3D::operator==(const Vector3D &right) const {
	return x == right.x and y == right.y and z == right.z and
		xPos == right.xPos and yPos == right.yPos and zPos == right.zPos;
}
