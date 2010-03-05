from OpenGL.GLUT import *
from OpenGL.GL   import *
from OpenGL.GLU  import *


#global variables - necessary evil when using GLUT
#global width and height

GW = 200
GH = 200

mouseX = 0
mouseY = 0

pts = []
#keep track of which drawing mode we are in points or square
#mode ==0 means draw square otherwise draw points
mode = True

WorldW = 2.0 * GW / GH
WorldH = 2.0

def p2w_x(w):
	global WorldW, GW
	return WorldW / GW * w - WorldW / 2

def p2w_y(h):
   global WorldH, GH
   return -1 * (WorldH / GH * h - WorldH / 2)

#helper drawing routines
def draw_square():
	glBegin(GL_POLYGON)
	glColor3f(1.0, 0.0, 0.0)
	glVertex2f(-0.5,-0.5)
	glColor3f(0.0, 1.0, 0.0)
	glVertex2f(-0.5,0.5)
	glColor3f(0.0, 0.0, 1.0)
	glVertex2f(0.5,0.5) 
	glColor3f(0.0, 0.0, 0.0)
	glVertex2f(0.5,-0.5)
	glEnd()

def draw_pts():
	global pts

	glPointSize(2.0)
	glBegin(GL_POINTS)
	glColor3f(0.5, 0.2, 0.8)
	for (x, y) in pts:
		glVertex2f(x, y)
	glEnd()


def display():
	glClear(GL_COLOR_BUFFER_BIT)

	if (mode == 0):
		draw_square()
	else:
		draw_pts()

	glFlush()

def mouse(button, state, x, y):
	global pts

	if (button == GLUT_LEFT_BUTTON):
		if (state == GLUT_DOWN):
			printf("mouse clicked at %d %d (%f, %f)\n", x, y, p2w_x(x), p2w_y(y) )
			temp = (p2w_x(x), p2w_y(y))
			pts.append(temp)
			glutPostRedisplay()

def mouseMove(x, y):
	global pts
	temp = (pw2_x(x), p2w_y(y))
	pts.push_back(temp)
	glutPostRedisplay()

def keyboard(key, x, y ):
	global pts
	if key == 'm':
		mode = not mode
		glutPostRedisplay()

	elif key == 'q' or key == 'Q':
		exit( 0 )

	elif key == 'h' or key == 'H':
		print "Hello!\n"

	elif key == 'c' or key == 'C':
	  print "Clearing points\n"
	  pts = []
	  glutPostRedisplay()

def reshape( w, h): 
	global GW, GH
	GW = w
	GH = h
	print "new window size: %d, %d\n" %  (w, h)
	glViewport(0,0,w,h)
	glLoadIdentity()
	gluOrtho2D(-w/h, w/h, -1, 1)

def menu(value):
	global mode, pts

	if (value == 1):
		print "Switching mode\n"
		mode = not mode
		glutPostRedisplay()

	elif (value == 2):
		print "clear\n"
		pts = []
		glutPostRedisplay()

	elif (value == 3):
		print "quit\n"
		exit( 0 )

glutInit( sys.argv )

glutInitWindowSize(200, 200)
glutInitWindowPosition(100, 100)
glutCreateWindow( "My Second Window" )
glClearColor(1.0, 1.0, 1.0, 1.0)

glutDisplayFunc( display )
glutMouseFunc( mouse )
glutMotionFunc( mouseMove )
glutKeyboardFunc( keyboard )
glutReshapeFunc( reshape )

rightMenu = glutCreateMenu(menu)
glutAddMenuEntry("Toggle Mode", 1)
glutAddMenuEntry("Clear Points", 2)
glutAddMenuEntry("Quit", 3)
glutAttachMenu(GLUT_RIGHT_BUTTON)

glutMainLoop()
