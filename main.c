#include <stdio.h>
#include <GL/glut.h>

#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 800

/* Deklaracija callback funkcija */
static void on_display(void);
//void on_keyboard(void);

int main(int argc, char** argv) {

  /* GLUT inicijalizacija */
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

  /* Kreiranje prozora */
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutInitWindowPosition(300, 100);
  glutCreateWindow(argv[0]);

  /* Registracija callback funkcija */
  glutDisplayFunc(on_display);

  /* OpenGL inicijalizacija */
  glClearColor(0, 0, 0, 0);
  glEnable(GL_DEPTH_TEST);

  /* Pokretanje glavne petlje */
  glutMainLoop();

  return 0;
}

static void on_display(void) {

  /* Brisanje prethodnog sadrzaja prozora */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* Update slikes */
  glutSwapBuffers();
}
