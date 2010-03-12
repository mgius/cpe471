from OpenGL.GLUT import *
from OpenGL.GLU  import *
from OpenGL.GL   import *

from models import *
from util import *
from Vector3D.Vector3D import *

from math import sin,cos

import random

import sys

# gotta be a better way of doing this
reset = False

# window size globals (for p2w_x, p2w_y)
GW = 300
GH = 300

# models list

modelsList = []
obstacleList = []
discList = []

# Camera globals
eye = Vector3D(0.0, 1.0, 10.0)
look = Vector3D(0.0, 1.0, 9.0)
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
    global look, eye, phi, theta, modelsList, discList, obstacleList
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
        eye.set(0.0, 1.0, 10.0)
        look.set(0.0, 1.0, 9.0)
        phi = 0.0
        theta = 272.0
        modelsList = []
        obstacleList = []
        discList = []
        initializeObjects()
        glutPostRedisplay()

    elif key == 'q' or key == 'Q':
        exit(0)

    if eye.y < 1.0:
        eye.y = 1.0
        look.y = eye.y + sin(deg2rad(phi))

    glutPostRedisplay()

grabbed = None

def doPicking(x, y):
    '''
    Determine which objects are in this clicking region
    '''
    global eye, look, GW, GH, modelsList, grabbed
    # get values on the size of the viewport
    viewport = glGetInteger(GL_VIEWPORT)
    # allocate a select buffer long enough to hold 
    glSelectBuffer(len(modelsList) + 3)
    glRenderMode(GL_SELECT)
    glInitNames()
    # load a junk name
    glPushName(0xfffffff)

    glMatrixMode(GL_PROJECTION)
    glPushMatrix() #1
    
    # set up projection zoomed in near the mouse
    glLoadIdentity()
    gluPickMatrix(x, viewport[3]-y, 1,1,viewport)
    gluPerspective(90, float(GW)/float(GH), 1.0, 15.0)

    # draw all the objects
    glMatrixMode(GL_MODELVIEW)
    glPushMatrix() #2

    gluLookAt(eye.x, eye.y, eye.z, look.x, look.y, look.z, 0, 1, 0)
    drawObjects()

    glPopMatrix() #2

    glMatrixMode(GL_PROJECTION)
    glPopMatrix() #1

    glMatrixMode(GL_MODELVIEW)
    glFlush()

    # restore original projection matrix

    buffer = glRenderMode(GL_RENDER)
    print "Number of hits %d\n" % len(buffer)

    for hit_record in buffer:
        min_depth, max_depth, names = hit_record
        print "names:\n" 
        for name in names:
            print "grabbing something\n"
            if modelsList[name].grab():
                grabbed = modelsList[name]

def plinkoMouse(button, state, x, y):
    '''
    Initial mouse click for plinko chip manipulation
    '''
    global lastMouseX, lastMouseY, modelsList, GW, GH, grabbed
    if button == GLUT_LEFT_BUTTON:
        if state == GLUT_DOWN:
            print "left mouse clicked at %d %d\n" % (x, y)
            #for thing in modelsList:
            #    if thing.contains(p2w_x(x, GH, GW), \
            #                      p2w_y(y, GH, GW)):
            #        thing.grab()
            #        grabbed = thing
            lastMouseX = x
            lastMouseY = y
            doPicking(x,y)
        if state == GLUT_UP:
            print "mouse released\n"
            if grabbed != None:
                grabbed.release()
                grabbed = None

def plinkoMouseMove(x, y):
    '''
    Mouse movement for handling moving the plinko chips
    '''
    global lastMouseX, lastMouseY, grabbed, GW, GH
    if grabbed != None:
        xMove = p2w_x(lastMouseX, GH, GW) - p2w_x(x, GH, GW)
        yMove = p2w_y(lastMouseY, GH, GW) - p2w_y(y, GH, GW)
        back = 15.0
        front = 1.0
        zFactor = (back - grabbed.position.z) / (back - front)
        grabbed.move(Vector3D(-xMove * zFactor , -yMove * zFactor, 0))
        lastMouseX = x
        lastMouseY = y
        glutPostRedisplay()

def cameraMouse(button, state, x, y):
    ''' 
    Initial mouse click for camera manipulation.

    Mainly kept for historical purposes and testing
    '''
    global lastMouseX, lastMouseY
    if button == GLUT_LEFT_BUTTON:
        if state == GLUT_DOWN:
            print "left mouse clicked at %d %d\n" % (x, y)
            lastMouseX = x
            lastMouseY = y

def cameraMouseMove(x, y):
    '''
    Mouse movement handling for camera manipulation.

    Mainly kept for historical purposes and testing
    '''
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
    global GW, GH
    GW = w
    GH = h
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    gluPerspective(90, float(w)/float(h), 1.0, 15.0)
    glMatrixMode(GL_MODELVIEW)
    glViewport(0, 0, w, h)

    glutPostRedisplay()

def drawObjects():
    ''' 
    Draw all "clickable" objects
    '''
    global modelsList
    glPushMatrix()
    name = 0
    for thing in modelsList:
        glLoadName(name)
        name += 1
        thing.draw()
    glPopMatrix()

def display():
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

    glMatrixMode(GL_MODELVIEW)

    glPushMatrix() #1
    gluLookAt(eye.x, eye.y, eye.z, look.x, look.y, look.z, 0, 1, 0)
    
    # print "eye at %f, %f, %f looking at %f, %f, %f\n" % ( eye.x, eye.y, eye.z, look.x, look.y, look.z)

    pos_light()

    glPushMatrix() #2
    glTranslatef(0, -.5, 0)
    #Plane().draw()
    glPopMatrix() #2
    #Axes().draw()

    drawObjects()
    glPopMatrix() #1

    glutSwapBuffers()
  
def initializeObjects():
    global modelsList, discList, obstacleList
    disc = PlinkoDisc(position=Vector3D(0,6,0))
    modelsList.append(disc)
    discList.append(disc)

    #for x in range(10):
    #    peg = Peg(position=Vector3D(random.uniform(-3,3), random.uniform(-3,1), 0))
    #    modelsList.append(peg)
    #    obstacleList.append(peg)
    board = PlinkoBoard()
    obstacleList.extend(board.getPegs())
    obstacleList.extend(board.getWalls())
    modelsList.append(board)



def timer(data):
    if not reset:
        for disc in discList:
            disc.gravity(obstacleList)
    glutTimerFunc(100, timer, 0)
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
glutMouseFunc( plinkoMouse )
glutMotionFunc( plinkoMouseMove )

glEnable(GL_DEPTH_TEST)

init_lighting()

glEnable(GL_NORMALIZE)
glEnable(GL_LIGHTING)

initializeObjects()
glutTimerFunc(100, timer, 0)
glutMainLoop()
