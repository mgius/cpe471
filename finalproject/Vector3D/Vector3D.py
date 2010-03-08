'''
Module level comment bwahahaha
'''
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
        return self.x * other.x + self.y * other.y + self.z * other.z

    def crossProd(self, other):
        return Vector3D(x = self.y * other.z - self.z * other.y,
                        y = self.z * other.x - self.x * other.z,
                        z = self.x * other.y - self.y * other.x)

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
            self.normalize()

    # operator overloads
    def __add__(self, other):
        return Vector3D(self.x + other.x, self.y + other.y, self.z + other.z)

    def __iadd__(self, other):
        self.x += other.x
        self.y += other.y
        self.z += other.z
        return self

    def __sub__(self, other):
        return Vector3D(self.x - other.x, self.y - other.y, self.z - other.z)

    def __isub__(self, other):
        self.x -= other.x
        self.y -= other.y
        self.z -= other.z
        return self

    def __mul__(self, mult):
        return Vector3D(self.x * mult, self.y * mult, self.z * mult)

    def __imul__(self, mult):
        self.x *= mult
        self.y *= mult
        self.z *= mult
        return self

    def __div__(self, mult):
        return Vector3D(self.x / mult, self.y / mult, self.z / mult)

    def __idiv(self, mult):
        self.x /= mult
        self.y /= mult
        self.z /= mult
        return self

    def __eq__(self, other):
        if self.x == other.x and \
           self.y == other.y and \
           self.z == other.z and \
           self.xPos == other.xPos and \
           self.yPos == other.yPos and \
           self.zPos == other.zPos:
            return True
        else:
            return False
    def __ne__(self, oth):
        return not self == oth

    def __str__(self):
        return "Vector3D at (%f,%f,%f) with components (%f,%f, %f)" % \
            (self.xPos, self.yPos, self.zPos, self.x, self.y, self.z)
