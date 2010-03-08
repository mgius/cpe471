'''
Models for Plinko.
'''

from OpenGL.GLUT import *
from OpenGL.GLU  import *
from OpenGL.GL   import *
from Vector3D.Vector3D import *

# Any materials I might need in a dictionary
materials = { 
    'redflat': ( 
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

def setMaterial(material):
    ''' 
    Given a tuple that contains the arrays for the four material properties
    unpack the tuple and apply the material properties
    '''
    ambient, diffuse, specular, shininess = material
    glMaterial(GL_FRONT, GL_AMBIENT, ambient)
    glMaterial(GL_FRONT, GL_DIFFUSE, diffuse)
    glMaterial(GL_FRONT, GL_SPECULAR, specular)
    glMaterial(GL_FRONT, GL_SHININESS, shininess)


class drawable():
    '''
    Anything that can be drawn onto an openGL canvas

    The draw() function will draw assuming it is at 0,0,0, but you
    are welcome to translate before calling draw if you'd like.
    '''
    grabbed = False

    #constructor
    def __init__(self, position=Vector3D()):
        self.position=position

    def translate(self):
        '''
        Applies the openGL translate that puts this object into the right place
        '''
        glTranslate(self.position.x, self.position.y, self.position.z)

    def move(self, vector):
        '''
        Moves this object by the amount specified in the passed vector
        '''
        self.position += vector

    def draw(self):
        raise NotImplementedError

    def gravity(self):
        ''' Emulates the effects of gravity on the object '''
        return None

    def contains(self, worldX, worldY):
        ''' 
        True if the world coordinates are "contained" this object. 
        AKA, has this thing been clicked
        '''
        return False
    def grab(self):
        '''
        "Grabs" this object
        '''
        pass
    def release(self):
        '''
        "releases" this object
        '''
        pass


class IceCream(drawable):
    def draw(self):
        glPushMatrix() #1
        self.translate()
        # Rotate the whole cone to be oriented in the negative X direction
        glRotatef(90, 1, 0, 0)
        setMaterial(materials['yellowflat'])
        glutSolidCone(.5, 3, 10, 10)
        glPushMatrix() #2
        glTranslatef(0, 0, -.5)
        setMaterial(materials['greenshiny'])
        glutSolidSphere(.6, 10, 10)
        glPopMatrix() #2
        glPopMatrix() #1

class SnowMan(drawable):
    def draw(self):
        glPushMatrix() #1
        self.translate()
        setMaterial(materials['blueflat'])
        glutSolidSphere(1, 10, 10)
        glPushMatrix() #2
        # Move it bottom sphere and middle sphere, minus a bit
        glTranslatef(0, 1.6, 0)
        setMaterial(materials['blackflat'])
        glutSolidSphere(.8, 10, 10)

        glPushMatrix() #3
        # Move it middle sphere and top sphere, minus a bit
        glTranslatef(0, 1.1, 0)
        setMaterial(materials['blueflat'])
        glutSolidSphere(.5, 10, 10)
        glPopMatrix() #3

        glPopMatrix() #2

        glPopMatrix() #1

class Plane(drawable):
    '''
    Draws a large ground plane at the origin in the X plane
    '''
    def __init__(self, size=100, material='redflat'):
        drawable.__init__(self)
        self.size=size
        self.material=material

    def draw(self):
        setMaterial(materials[self.material])
        glBegin(GL_QUADS)
        glVertex3f(-self.size, 0, -self.size)
        glVertex3f(-self.size, 0,  self.size)
        glVertex3f( self.size, 0,  self.size)
        glVertex3f( self.size, 0, -self.size)
        glEnd()

class Axes(drawable):
    '''
    Draws a big set of XYZ axes at the origin
    '''
    def draw(self):
        oldWidth = glGetFloatv( GL_LINE_WIDTH)
        glLineWidth(5.0)

        # X axis
        glBegin(GL_LINES)
        glColor3f(0.5, 0, 0)
        glVertex3f(-50.0, 0, 0)
        glVertex3f(50.0,0,0)
        glEnd()

        # Y axis
        glBegin(GL_LINES)
        glColor3f(0, 0.5, 0)
        glVertex3f(0, -50.0, 0)
        glVertex3f(0,50.0,0)
        glEnd()

        # Z axis
        glBegin(GL_LINES)
        glColor3f(0, 0, .5)
        glVertex3f(0, 0, -50.0)
        glVertex3f(0,0,50.0)
        glEnd()

        glLineWidth(oldWidth)

class PlinkoDisc(drawable):
    '''
    Game Disc.  Dropped from the top of the board and falls to the bottom

    should eventually be a red disc with a dollar sign on it
    '''
    radius = .5
    height = .1
    slices = 20
    stacks = 20
    velocity = Vector3D()
    def draw(self):
        glPushMatrix() #1
        self.translate()
        setMaterial(materials['redflat'])
        glutSolidCylinder(self.radius, self.height, self.slices, self.stacks)
        glPopMatrix() #1

    def gravity(self):
        if self.grabbed == False:
            self.velocity += Vector3D(0, -.05, 0)
            self.velocity /= 2
            self.position += self.velocity
            print "Disc new position %s" % str(self.position)

    def contains(self, worldX, worldY):
        distance = self.position - Vector3D(worldX, worldY, 0)
        distance.z = 0
        if distance.length() < self.radius:
            return True
        else:
            return False
    def grab(self):
        self.grabbed = True

    def release(self):
        self.grabbed = False
        


class Peg(drawable):
    '''
    Peg object.  The position of the peg should be relative to the board
    '''
    def draw(self):
        pass

class PlinkoBoard(drawable):
    '''
    The gameboard.  Generates its own pattern of pegs if not provided 
    with one.
    '''
    def __init__(self, position=Vector3D(), pegs=None):
        self.position=position
        if pegs == None:
            self.pegs = self.generatePegs()
        else:
            self.pegs = pegs


    def generatePegs(self):
        return None

    def draw():
        # draw the flat board
        # draw the pegs
        for peg in self.pegs:
            peg.draw()
