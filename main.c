#include <stdio.h>
#include <GL/glut.h>

#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 600

/* Deklaracija callback funkcija */
static void on_display(void);


int main(int argc, char** argv) {

  // GLUT inicijalizacija 
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

  // kreiranje prozora 
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutInitWindowPosition(325, 125);
  glutCreateWindow(argv[0]);

  // registracija callback funkcija 
  glutDisplayFunc(on_display);

  // OpenGL inicijalizacija 
  glClearColor(1, 1, 1, 0);
  glEnable(GL_DEPTH_TEST);

  // pokretanje glavne petlje 
  glutMainLoop();

  return 0;
}

static void on_display(void) {

  // Brisanje prethodnog sadrzaja prozora 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* Update slikes */
  glutSwapBuffers();
}
