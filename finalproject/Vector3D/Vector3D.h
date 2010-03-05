#include <ostream>
class Vector3D {
public:
	float x,y,z;
	float xPos, yPos, zPos;

	Vector3D() : x(0), y(0), z(0), xPos(0), yPos(0), zPos(0) {}
	Vector3D(float _x, float _y, float _z, 
			   float _xPos, float _yPos, float _zPos) : x(_x), y(_y), z(_z),
					xPos(_xPos), yPos(_yPos), zPos(_zPos) {}

	Vector3D(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {
		xPos = yPos = zPos = 0;
	}

	void set(float _x, float _y, float _z);

	float getX() { return x; }
	float getY() { return y; }
	float getZ() { return z; }

	void draw() const;
	float length() const;


	float dotProd(const Vector3D &right) const;

	Vector3D crossProd(const Vector3D &right) const;

	Vector3D &translate(float, float, float);

	void bindZ();
	void normalize(float scale = 1.0);
	void scaleToOne();

	Vector3D &rotateX(float);
	Vector3D &rotateY(float);
	Vector3D &rotateZ(float);

	bool operator==(const Vector3D &right) const;


	Vector3D operator+(const Vector3D &right) const;
	Vector3D &operator+=(const Vector3D &right);
	Vector3D operator-(const Vector3D &right) const;
	Vector3D &operator-=(const Vector3D &right);
	Vector3D operator*(float) const;
	Vector3D &operator*=(float);
	friend std::ostream& operator<< (std::ostream &out, const Vector3D &v) {
		out << "X: " << v.x << " Y: " << v.y << " Z: " << v.z;

		return out;
	}


};

