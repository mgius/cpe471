#include <GL/gl.h>
#include <GL/glut.h>

void display() {

  glClear(GL_COLOR_BUFFER_BIT);
  glBegin(GL_POLYGON);
  glColor3f(1.0, 0.0, 0.0);
  glVertex2f(-1,0);
  glVertex2f(0,-1);
  glVertex2f(1,0);
  glVertex2f(0, 1);

  glEnd();
  glFlush();
}

int main( int argc, char** argv) {
   glutInit(&argc, argv);
   glutInitWindowSize(100, 200);
   glutInitWindowPosition(100,100);
   glutCreateWindow( "My First Window:");
   glutDisplayFunc(display);
   glClearColor(1.0, 1.0, 1.0, 1.0);
   glutMainLoop();

}
