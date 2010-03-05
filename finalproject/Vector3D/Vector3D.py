from math import sqrt
class Vector3D():
    '''
    3D Vector reprentation class for CPE471
    '''

    # constructor
    def __init__(self, x=0, y=0, z=0, xPos=0, yPos=0, zPos=0):
        self.x = x
        self.y = y
        self.z = z
        self.xPos = xPos
        self.yPos = yPos
        self.zPos = zPos

    # functions
    def set(self, x,y,z):
        (self.x,self.y,self.z) = (x,y,z)

    def length(self):
        return sqrt(self.x * self.x + self.y * self.y + self.z * self.z)

    def dotProd(self, other):
        return self.x * other.x + self.y * other.y + self.z + other.z

    def crossProd(self, other):
        return Vector3D(x = y * right.z - z * right.y,
                        y = z * right.x - x * right.z,
                        z = x * right.y - y * right.x)

    def translate(self, x,y,z):
	    self.xPos += x
	    self.yPos += y
	    self.zPos += z

    def bindZ(self):
        underTheSqrt = 1.0 - self.x * self.x - self.y * self.y
        self.z = underTheSqrt if underTheSqrt > 0 else 0

    def normalize(self, scale = 1.0):
        len = self.length()
        self.x /= (len / scale)
        self.y /= (len / scale)
        self.z /= (len / scale)

    def scaleToOne(self):
        if self.length() > 1.0:
            normalize()

    # operator overloads
    def __add__(self, other):
        self.x += other.x
        self.y += other.y
        self.z += other.z

    def __iadd__(self, other):
        self.x += other.x
        self.y += other.y
        self.z += other.z
        return self

    def __sub__(self, other):
        self.x -= other.x
        self.y -= other.y
        self.z -= other.z

    def __isub__(self, other):
        self.x -= other.x
        self.y -= other.y
        self.z -= other.z
        return self

    def __mul__(self, mult):
        self.x *= mult
        self.y *= mult
        self.z *= mult

    def __imul__(self, mult):
        self.x *= mult
        self.y *= mult
        self.z *= mult
        return self

    def __str__(self):
        return "Vector3D at (%f,%f,%f) with components (%f,%f, %f)" % \
            (self.xPos, self.yPos, self.zPos, self.x, self.y, self.z)
