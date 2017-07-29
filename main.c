#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#define COUNT 200
#define TIMER_ID 1

#define TIMER_INTERVAL 20
#define SPEED 0.4
#define JUMPSPEED 0.5
#define NUM_IMP 3
/* deklaracija callback funkcija*/
static void on_display();
static void on_keyboard(unsigned char key,int x,int y);
static void on_timer();
static void on_reshape(int w,int h);


/*deklaracije ostalih funkcija*/
static void draw_mario();
static void lighting();
static void draw_base();
static void draw_impediments();
static void draw_impediments2();
static void check_right();
static void check_left();

/*dekleracije globalnih promenljivih*/
static int in_air = 0;
static float y_coor = 0; /*y koordinata marija*/
static float x_coor = 0; /* x koordinata marija*/
static float speed=JUMPSPEED; /*brzina skoka*/
/* komponente za igraca */
GLfloat ambient_legs[] = {0,0,1,1};
GLfloat diffuse_legs[] = {0,0,1,1};

GLfloat ambient_body[] = {1,0,0,1};
GLfloat diffuse_body[] = {1,0,0,1};

GLfloat ambient_arms[] = {1,1,0,1};
GLfloat diffuse_arms[] = {1,1,0,1};

GLfloat ambient_head[] = {0,0.5,0,1};
GLfloat diffuse_head[] = {0,0.5,0,1};

/*komponente za podlogu*/
GLfloat ambient_base[]={0.7,0.2,0.2,1};
GLfloat diffuse_base[]={0.7,0.2,0.2,1};

int main(int argc, char **argv){
	/* incijalizacija GLUT-a*/
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	
	/*kreiranje prozora*/
	glutInitWindowSize(900,600);
	glutInitWindowPosition(100,100);
	glutCreateWindow(argv[0]);
	
	/*registracija callback funkcija*/
	glutDisplayFunc(on_display);
	glutReshapeFunc(on_reshape);
	glutKeyboardFunc(on_keyboard);
	
	/*OpenGL inicijalizacija*/
	glClearColor(0.75, 0.75, 0.75, 0);
        glEnable(GL_DEPTH_TEST);

	lighting();

	/*program ulazi u glavnu petlju */
	glutMainLoop();
	return 1;
}

static void on_reshape(int w,int h){
	/* postavljamo viewport*/
	glViewport(0,0,w,h);
	/* postavljamo perspektivu*/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60,w*1.0/h,1,200);
}
static void on_keyboard(unsigned char key,int x,int y){
	if(key == 27){
		/*izlazimo iz programa-esc dugme*/
		exit(0);
	}
	else if(key == ' '){
		if(!in_air){
			glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
			in_air=1;
		}
	}
	else if (key == 'd' || key == 'D'){
		check_right();
		glutPostRedisplay();
	}
	else if (key == 'a' || key == 'A'){
		check_left();
		glutPostRedisplay();
	}

	
}
static void lighting(){
        
        /* ukljucujemo osvetljenje */
        glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	/* komponente */
	GLfloat light_ambient[] = {0.1, 0.1, 0.1, 1};
	GLfloat light_diffuse[] = {1, 1, 1, 1};
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

	GLfloat shiness = 20;
	glMaterialf(GL_FRONT, GL_SHININESS, shiness);

	/* pozicija izvora svetlosti */
	GLfloat light_position[] = {10, 8, 20, 0};
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}
static void on_display(){
	/* brisemo prethodni sadrzaj prozora */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	/*podesavamo vidnu tacku */
	glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0,1,50,0,0,0,0,1,0);
	
	printf("x: %f\n", x_coor);
	printf("y: %f\n", y_coor);

	/* poziva se funkcija koja iscrtava marija*/
	glPushMatrix();
		glTranslatef(0,y_coor,0);
		draw_mario();
	glPopMatrix();
	/*poziva se funkcija koja iscrtava podlogu*/
	glPushMatrix();
		glTranslatef(-x_coor, -20 , 0);
		draw_base();
	glPopMatrix();
	/*poziva se funkcija koja iscrtava zelene prepreke na zemlji */
	glTranslatef(5,7,0);
	glPushMatrix();
		glTranslatef(-x_coor,-20,0);
		glTranslatef(10, 0, 0);
		draw_impediments();
		glTranslatef(30, 0, 0);
		draw_impediments();
		glTranslatef(60, 0, 0);
		draw_impediments();
		glTranslatef(15,0,0);
		draw_impediments();
		glTranslatef(30,0,0);
		draw_impediments();
		glTranslatef(40,0,0);
	 	draw_impediments();
		glTranslatef(50, 0, 0);
		draw_impediments();
	glPopMatrix();
	/* poziva se funkcija koja iscrtava cigle, to jest prepreke u vazduhu*/
	glPushMatrix();
		glTranslatef(20 - x_coor, -8, 0);
		glTranslatef(5, 0, 0);
		draw_impediments2();
		glTranslatef(30, 0, 0);
		draw_impediments2();
		glTranslatef(3, 0, 0);
		draw_impediments2();
		glTranslatef(3, 0, 0);
		draw_impediments2();
		glTranslatef(15, 0, 0);
		draw_impediments2();
		glTranslatef(3, 0, 0);
		draw_impediments2();
		glTranslatef(3, 0, 0);
		draw_impediments2();
		glTranslatef(3, 0, 0);
		draw_impediments2();
		glTranslatef(45, 0, 0);
		draw_impediments2();
		glTranslatef(38, 0, 0);
		draw_impediments2();
		glTranslatef(3, 0, 0);
		draw_impediments2();
		glTranslatef(3, 0, 0);
		draw_impediments2();
		glTranslatef(17, 0, 0);
		draw_impediments2();
		int i;
		for (i = 1; i < 14 ; i++){
			glTranslatef(3, 0, 0);
			draw_impediments2();
		}
		
	glPopMatrix();
	glutSwapBuffers();
}    

static void draw_mario(){
	glTranslatef(-20,-20,0);
	/*iscrtavamo noge*/
	glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_legs);
       		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_legs);
		glColor3f(0,0,1);
		glScalef(1,2,1);		
		glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_legs);
        	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_legs);
		glColor3f(0,0,1);
		glTranslatef(2,0,0);
		glScalef(1,2,1);		
		glutSolidCube(1);
	glPopMatrix();
	
	/*iscrtavamo telo */
	glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_body);
      		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_body);
		glTranslatef(1,2,0);
		glColor3f(1,0,0);
		glScalef(3,2.4,1);
		glutSolidCube(1);
	glPopMatrix();

	/*iscrtavamo ruke*/
	glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_arms);
        	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_arms);
		glTranslatef(-0.6,2,0);
		glColor3f(1,1,0);
		glScalef(1,2.4,1);
		glutSolidCube(1);
	glPopMatrix();
	
	glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_arms);
      		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_arms);
		glTranslatef(3.2,2,0);
		//glRotatef(60,0,0,1);
		glColor3f(1,1,0);
		glScalef(1,2.4,1);
		glutSolidCube(1);
	glPopMatrix();
	
	/*iscrtavamo glavu*/
	glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_head);
        	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_head);
		glTranslatef(1.3,3.9,0);
		glColor3f(0,0.5,0);
		glScalef(1,2,1);
		glutSolidCube(1);
	glPopMatrix();
}
static void draw_base(){
	//glTranslatef(0,-20,0);
	int i;
	for(i=0;i<COUNT;i++){
		glPushMatrix();
			glMaterialfv(GL_FRONT,GL_AMBIENT,ambient_base);
			glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse_base);
			glTranslatef(i*2,-2,0);
			glutSolidCube(2);
			glTranslatef(i*2,-2,0);
			glutSolidCube(2);
		glPopMatrix();
		glPushMatrix();
			glMaterialfv(GL_FRONT,GL_AMBIENT,ambient_base);
			glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse_base);
			glTranslatef(-i*2,-2,0);
			glutSolidCube(2);
			glTranslatef(-i*2,-2,0);
			glutSolidCube(2);
		glPopMatrix();
	}

}

static void draw_impediments(){
	GLUquadric* quadric = gluNewQuadric();
   	gluQuadricDrawStyle(quadric, GLU_FILL);
    	
	GLfloat ambient_imp2[]={0,0.7,0,1};
	GLfloat diffuse_imp2[]={0,0.7,0,1};
	
	glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_imp2);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_imp2);
		glRotatef(90,1,0,0);
		gluCylinder(quadric,1,1,5,32,8);
	glPopMatrix();
	
	glPushMatrix();
        	glTranslatef(0,0.5,0); 
        	glRotatef(90,1,0,0);
        	gluCylinder(quadric,2.2,2.2,8,32,8);
        	gluDisk(quadric,0,1,15,15);
    	glPopMatrix();

}

static void draw_impediments2(){
	
	GLfloat ambient_imp1[]={0.6,0.1,0,1};
	GLfloat diffuse_imp1[]={0.6,0.1,0,1};
		
	glMaterialfv(GL_FRONT,GL_AMBIENT,ambient_imp1);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse_imp1);
	glutSolidCube(3);
		
}


static void check_right(){
	float width = 9.57;
	float height = 8;
	float currImp = 29.19;
	/*proveravamo da li se ispred nas nalazi zelena prepreka */
	if (x_coor >= currImp && x_coor < currImp + width && y_coor <= height)
		return;
	currImp = 59.6;
	if (x_coor >= currImp && x_coor < currImp + width && y_coor <= height)
		return;
	currImp = 119.1;
	if (x_coor >= currImp && x_coor < currImp + width && y_coor <= height)
		return;
	currImp = 133.8;
	if (x_coor >= currImp && x_coor < currImp + width && y_coor <= height)
		return;
	currImp += 30;
	if (x_coor >= currImp && x_coor < currImp + width && y_coor <= height)
		return;
	currImp += 40;
	if (x_coor >= currImp && x_coor < currImp + width && y_coor <= height)
		return;
	currImp += 50;
	if (x_coor >= currImp && x_coor < currImp + width && y_coor <= height)
		return;

	/* sada proveravamo prepreke u vazduhu*/
	width = 3;
	height = 11;
	currImp = 43.59;
	if (x_coor >= currImp && x_coor < currImp + width && y_coor >= height)
		return;
	x_coor += SPEED;
}
static void check_left(){
	float width = 9.8;
	float height = 8;
	float currImp = 29.3 + width;
	/*proveravamo da li se iza nas nalazi zelena prepreka */
	if (x_coor > currImp - width && x_coor <= currImp && y_coor <= height){
		return;
	}
	currImp = 59.7 + width;
	if (x_coor > currImp - width && x_coor <= currImp && y_coor <= height)
		return;
	currImp = 119.3 + width;
	if (x_coor > currImp - width && x_coor <= currImp && y_coor <= height)
		return;
	currImp = 134.01 + width ;
	if (x_coor > currImp - width  && x_coor <= currImp && y_coor <= height)
		return;
	currImp += 30;
	if (x_coor > currImp - width && x_coor <= currImp && y_coor <= height)
		return;
	currImp += 40;
	if (x_coor > currImp - width && x_coor <= currImp  && y_coor <= height)
		return;
	currImp += 50;
	if (x_coor > currImp - width && x_coor <= currImp  && y_coor <= height)
		return;
	x_coor -= SPEED;
}
static void on_timer(int value){
		/*proveravamo da li dolazi sa tajmera za skok*/
		if(value!=TIMER_ID){
			exit(0);
		}	
		y_coor+=speed;
		if(y_coor>=14){
			speed=-speed*2;
		}
		if(y_coor<=0){
			in_air=0;
			speed=JUMPSPEED;
			y_coor=0;
		}
		glutPostRedisplay();
		if(in_air)
           		 glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
}        

