class Vector3D {
private:
	float x,y,z;
	float xPos, yPos, zPos;

public:
	Vector3D(float _x, float _y, float _z, 
			   float _xPos, float _yPos, float _zPos) : x(_x), y(_y), z(_z),
					xPos(_xPos), yPos(_yPos), zPos(_zPos) {}

	Vector3D(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {
		xPos = yPos = zPos = 0;
	}

	void draw();
	void length();

	float dotProd(Vector3D &right);

	Vector3D crossProd(Vector3D &right);

	bool operator==(Vector3D &right);
};
