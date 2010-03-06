'''
Models for Plinko.
'''

from OpenGL.GLUT import *
from OpenGL.GLU  import *
from OpenGL.GL   import *

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
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient)
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse)
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular)
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess)


class drawable():
def draw(self):
raise NotImplementedError

class IceCream(drawable):
	def draw(self):
		glPushMatrix()
		# Rotate the whole cone to be oriented in the negative X direction
		glRotatef(90, 1, 0, 0)
setMaterial(materials[MATERIAL_YELLOWFLAT])
glutSolidCone(.5, 3, 10, 10)
glPushMatrix()
glTranslatef(0, 0, -.5)
setMaterial(materials[MATERIAL_GREENSHINY])
glutSolidSphere(.6, 10, 10)
glPopMatrix()
glPopMatrix()
