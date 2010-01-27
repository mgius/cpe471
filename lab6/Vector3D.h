class Vector3D {
private:
	float x,y,z;
	float xPos, yPos, zPos;

public:
	Vector3D() : x(0), y(0), z(0), xPos(0), yPos(0), zPos(0) {}
	Vector3D(float _x, float _y, float _z, 
			   float _xPos, float _yPos, float _zPos) : x(_x), y(_y), z(_z),
					xPos(_xPos), yPos(_yPos), zPos(_zPos) {}

	Vector3D(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {
		xPos = yPos = zPos = 0;
	}

	void draw() const;
	float length() const;

	float dotProd(const Vector3D &right) const;

	Vector3D crossProd(const Vector3D &right) const;

	bool operator==(const Vector3D &right) const;
};
