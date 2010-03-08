from OpenGL.GLUT import *
from OpenGL.GLU  import *
from OpenGL.GL   import *

from models import *
from util import *
from Vector3D.Vector3D import *

from math import sin,cos

import sys

# models list

modelsList = []

# Camera globals
eye = Vector3D(0.0, 1.0, 3.0)
look = Vector3D(0.0, 1.0, 2.0)
up = Vector3D(0.0, 1.0, 0.0)

phi = 0.0
theta = 272.0

# Lighting Globals
light_pos = [1.0, 20.0, 1.5, 1.0]
light_amb = [1.0, 1.0, 1.0, 1.0]
light_diff = [0.6, 0.6, 0.6, 1.0]
light_spec = [0.8, 0.8, 0.8, 1.0]

def init_lighting():
    global light_diff, light_amb, light_spec
    # turn on light0
    glEnable(GL_LIGHT0)
    # set up the diffuse, ambient and specular components for the light
    glLight(GL_LIGHT0, GL_DIFFUSE, light_diff)
    glLight(GL_LIGHT0, GL_AMBIENT, light_amb)
    glLight(GL_LIGHT0, GL_SPECULAR, light_spec)
    # specify our lighting model as 1 normal per face
    glShadeModel(GL_SMOOTH)

def pos_light():
    global light_pos
    # set the light's position
    glMatrixMode(GL_MODELVIEW)
    glLight(GL_LIGHT0, GL_POSITION, light_pos)


# input handling
(lastMouseX, lastMouseY) = (0,0)

def keyboard(key, x, y):
    global look, eye, phi, theta
    scale_factor = .1
    
    zoom = look - eye
    zoom.scaleToOne()
    strafe = zoom.crossProd(Vector3D(0,1,0))
    up = zoom.crossProd(strafe)

    if key == 'w' or key == 'W':
        look += zoom * scale_factor
        eye += zoom * scale_factor

    elif key == 'a' or key == 'A':
        look -= strafe * scale_factor
        eye -= strafe * scale_factor

    elif key == 's' or key == 'S':
        look -= zoom * scale_factor
        eye -= zoom * scale_factor

    elif key == 'd' or key == 'D':
        look += strafe * scale_factor
        eye += strafe * scale_factor

    elif key == 'r' or key == 'R':
        glMatrixMode(GL_MODELVIEW)
        glPushMatrix()
        glLoadIdentity()
        glPopMatrix()
        eye.set(0.0, 1.0, 3.0)
        look.set(0.0, 0.0, 0.0)
        phi = 0.0
        theta = 272.0
        glutPostRedisplay()
    elif key == 'q' or key == 'Q':
        exit(0)

    if eye.y < 1.0:
        eye.y = 1.0
        look.y = eye.y + sin(deg2rad(phi))

    glutPostRedisplay()

def mouse(button, state, x, y):
    global lastMouseX, lastMouseY
    if button == GLUT_LEFT_BUTTON:
        if state == GLUT_DOWN:
            print "left mouse clicked at %d %d\n" % (x, y)
            lastMouseX = x
            lastMouseY = y

def mouseMove(x, y):
    global theta, phi, look, lastMouseX, lastMouseY
    rotate_scale = .5

    theta -= rotate_scale * (lastMouseX - x)
    phi += rotate_scale * (lastMouseY - y)

    phi = phi if phi < 50.0 else 50.0
    phi = phi if phi > -50.0 else -50.0
    theta = theta if theta > 360 else theta - 360.0
    theta = theta if theta < 360 else theta + 360.0

    look.x = eye.x + cos(deg2rad(phi)) * cos(deg2rad(theta))
    look.y = eye.y + sin(deg2rad(phi))
    look.z = eye.z + cos(deg2rad(phi)) * cos(90 - deg2rad(theta))

    lastMouseX = x
    lastMouseY = y
    glutPostRedisplay()

# Display callbacks
def reshape(w, h):
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    gluPerspective(90, w/h, 1.0, 15.0)
    glMatrixMode(GL_MODELVIEW)
    glViewport(0, 0, w, h)

    glutPostRedisplay()

def display():
    global modelsList
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

    glMatrixMode(GL_MODELVIEW)

    glPushMatrix() #1
    gluLookAt(eye.x, eye.y, eye.z, look.x, look.y, look.z, 0, 1, 0)
    
    print "eye at %f, %f, %f looking at %f, %f, %f\n" % ( eye.x, eye.y, eye.z, look.x, look.y, look.z)

    pos_light()

    glPushMatrix() #2
    glTranslatef(0, -.5, 0)
    #Plane().draw()
    for thing in modelsList:
        thing.gravity()
        thing.draw()

    glPopMatrix() #2
    #Axes().draw()

    glPopMatrix() #1

    glutSwapBuffers()
  
# Snowmen
#for i in range(1,10):
#    modelsList.append(SnowMan(Vector3D(i*2 + 1, 1.0, i * 3)))
## IceCream
#for i in range(-5 , 13):
#    modelsList.append(IceCream(Vector3D(i - 1, 1.0, -i)))

modelsList.append(PlinkoDisc())

def timer(data):
    glutTimerFunc(50, timer, 0)
    glutPostRedisplay()

glutInit(sys.argv)
glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH)
glutInitWindowSize(300, 300); 
glutInitWindowPosition(100, 100)
glutCreateWindow("Program 4 in python")
glClearColor(1.0, 1.0, 1.0, 1.0)

glutDisplayFunc( display )
glutReshapeFunc( reshape )
glutKeyboardFunc( keyboard )
glutMouseFunc( mouse )
glutMotionFunc( mouseMove )

glEnable(GL_DEPTH_TEST)

init_lighting()

glEnable(GL_NORMALIZE)
glEnable(GL_LIGHTING)

glutTimerFunc(100, timer, 0)
glutMainLoop()
