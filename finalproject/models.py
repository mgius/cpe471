'''
Models for Plinko.
'''

from OpenGL.GLUT import *
from OpenGL.GLU  import *
from OpenGL.GL   import *
from Vector3D.Vector3D import *

''' Any materials I might need in a dictionary '''
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
	ambient, diffuse, specular, shininess = material
	glMaterial(GL_FRONT, GL_AMBIENT, ambient)
	glMaterial(GL_FRONT, GL_DIFFUSE, diffuse)
	glMaterial(GL_FRONT, GL_SPECULAR, specular)
	glMaterial(GL_FRONT, GL_SHININESS, shininess)


class drawable():

   # constructor
	def __init__(position=Vector3D()):
		self.position=position

	def draw(self):
		raise NotImplementedError
	
class IceCream(drawable):
	def draw(self):
		glPushMatrix() #1
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
		setMaterial(materials['blueflat'])
		glutSolidSphere(1, 10, 10)
		glPushMatrix() #1
		# Move it bottom sphere and middle sphere, minus a bit
		glTranslatef(0, 1.6, 0)
		setMaterial(materials['blackflat']);
		glutSolidSphere(.8, 10, 10)

		glPushMatrix() #2
		# Move it middle sphere and top sphere, minus a bit
		glTranslatef(0, 1.1, 0)
		setMaterial(materials['blueflat'])
		glutSolidSphere(.5, 10, 10)
		glPopMatrix() #2

		glPopMatrix() #1

