#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <GL/glut.h>
#include <unistd.h>

#define COUNT 200
#define TIMER_ID 1
#define XMOVE 19.2
#define TIMER_INTERVAL 20
#define SPEED 0.4
#define JUMPSPEED 0.3
#define NUM_IMP 3
#define COUNTIMP 7
#define MAXREACH 15
#define GIP 10.51
#define GIPSECOND 17.4
#define GREENIMPSIZE 9.5
#define AIRIMPSIZE 3
#define BLUEIMPSIZE 4
#define MARIOSIZE 5
#define NUMBER_OF_LEVELS 2

/* struktura koja pamti koordinate prepreka */
typedef struct imp{
	float begin;
	float end;
}imp;

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
static void draw_impediments3();
static void check_right();
static void check_left();
static void check_jump();
static int checkImp();
static int checkImp2();
static int check_right_green_imp();
static int check_right_air_imp();
static int check_left_air_imp();
static int check_left_green_imp();
static void check_jump_green();
static void draw_text(char* s);
/*dekleracije globalnih promenljivih*/
static int in_air = 0;
static float y_coor = 0; /*y koordinata marija*/
static float x_coor = 0; /* x koordinata marija*/
static float speed=JUMPSPEED; /*brzina skoka*/
static int on_imp[COUNTIMP]; /*niz koji pokazuje na kojoj prepreci stojimo  */
static int on_imp2[COUNTIMP]; /*pomocni niz koji pomaze prilikom skoka sa prepreke */
static int on_imp_air[COUNT]; /* niz koji pokazaju na kojoj vazdusnoj prepreci stojimo */
static int on_imp_air2[COUNT]; /* pomocni niz koji pomaze prilikom skoka sa visece prepreke */
static imp air_imp[COUNT]; /*niz koordinata prepreka u vazduhu */
static imp green_imp[COUNT]; /* niz koordinata zelenih prepreka */
static imp blue_imp[COUNT]; /* niz koordinata plavih prepreka */
static int down = 0; /* indikator koji pokazuje da li padamo ili ne*/
static float extraJump = 0; /* povecava maksimalni domet skoka u vis kada se nalazimo na prepreci */ 
static int flag = 1; /*pokazuje da li treba da skocimo u vis ili da padnemo sa prepreke */
static int num_of_green = 0; /* broj zelenih prepreka */
static int num_of_air = 0; /* broj prepreka u vazduhu */
static int num_of_blue = 0; /* broj plavih prepreka*/
static int ind = 1; /* indikator koji nam govori da li mozemo da se krecemo */


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
	glClearColor(0.529412, 0.807843, 0.921569, 1);
        glEnable(GL_DEPTH_TEST);

	lighting();
	srand(time(NULL));
	char s[20];
	s[0] = rand() % NUMBER_OF_LEVELS + 1  + '0';
	s[1] = 0;
	strcat(s, ".txt");

	FILE* f = fopen(s, "r");
	
	int i = 0;
	float x;
	while ( fscanf(f, "%f", &x)==1){
		if (x == -1)
			break;
		green_imp[i].begin = x;
		fscanf(f, "%f", &green_imp[i++].end);
	}
	num_of_green = i;
	i = 0;
	
	while ( fscanf(f, "%f", &x)==1){
		if (x == -1)
			break;
		air_imp[i].begin = x;
		fscanf(f, "%f", &air_imp[i++].end);
	}
	num_of_air = i;
	i = 0;
	while ( fscanf(f, "%f", &x)==1){
		if (x == -1)
			break;
		blue_imp[i].begin = x;
		fscanf(f, "%f", &blue_imp[i++].end);
	}
	num_of_blue = i;

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
		if(!ind)
			return;
		if(!in_air){
			glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
			in_air=1;
		}
	}
	else if (key == 'd' || key == 'D'){
		if(!ind)
			return;
		check_right();
		glutPostRedisplay();
	}
	else if (key == 'a' || key == 'A'){
		if(!ind)
			return;
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
	int k;
	for (k = 0; k < num_of_blue; k++){
		if(x_coor >= blue_imp[k].begin + 2 && x_coor <= blue_imp[k].end -2 && y_coor == 0)
			draw_text("   GAME OVER\n Press ESC to exit");
	}
	if (x_coor >= blue_imp[k-1].end + 5  && y_coor == 0)
		draw_text("   YOU WIN\n Press ESC to exit");

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
		int i;
		for( i = 0; i < num_of_green ; i++){
			glPushMatrix();
			glTranslatef(green_imp[i].begin - XMOVE, 0, 0);
			draw_impediments();
			glPopMatrix();
		}
		
	glPopMatrix();
	/* poziva se funkcija koja iscrtava cigle, to jest prepreke u vazduhu*/
	glPushMatrix();
		glTranslatef(- x_coor, -7, 0);
		for( i = 0; i < num_of_air ; i++){
			int j;
			int num = (air_imp[i].end - air_imp[i].begin - MARIOSIZE) / AIRIMPSIZE;
			glPushMatrix();
			glTranslatef(air_imp[i].begin - XMOVE, 0, 0);
			draw_impediments2();
			glPopMatrix();
			for (j = 1; j < num ; j++){
				glPushMatrix();
				glTranslatef(air_imp[i].begin - XMOVE + j * AIRIMPSIZE, 0, 0);
				draw_impediments2();
				glPopMatrix();
			}
		}
		
	glPopMatrix();
	/* poziva se funkcija koja iscrtava rupe */
	glPushMatrix();
		glTranslatef(-x_coor, -29.98, 0);
		for( i = 0; i < num_of_blue ; i++){
			int j;
			int num = (blue_imp[i].end - blue_imp[i].begin - MARIOSIZE) / BLUEIMPSIZE;
			glPushMatrix();
			glTranslatef(blue_imp[i].begin - XMOVE, 0, 0);
			draw_impediments3();
			glPopMatrix();
			for (j = 1; j < num ; j++){
				glPushMatrix();
				glTranslatef(blue_imp[i].begin - XMOVE + j * BLUEIMPSIZE , 0, 0);
				draw_impediments3();
				glPopMatrix();
			}
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

static void draw_impediments3(){
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glColor3f(0.529412, 0.807843, 0.921569);
	glutSolidCube(4);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}
static void check_right(){
	
	if (check_right_green_imp())
		return;
	if (check_right_air_imp())
		return;
	

	
	x_coor += SPEED;
}


static int check_right_green_imp(){
	/*proveravamo da li se ispred nas nalazi zelena prepreka i sprecava prolaz kroz nju.
	Takodje gledamo da li smo sisli sa prepreke, ako smo uopste bili na njoj, i u tom slucaju sledi pad*/
	float height = 8;
	int i;
	for (i  = 0; i < num_of_green; i++){
		if (x_coor >= green_imp[i].begin && x_coor < green_imp[i].end - 1 && y_coor <= height)
			return 1;
		if (on_imp[i] && x_coor > green_imp[i].end){
			in_air = 1;
			if(y_coor <= height + 2)
				down = 1;
			on_imp[i] = 0;
			on_imp2[i] = 0;
			on_timer(TIMER_ID);
		}
	}
	
	return 0;
}

static int check_right_air_imp(){
	/* sada proveravamo da li se nalazimo na preprekama u vazduhu i implementiramo  pad */
	/* proveravamo da li udaramo u prepreke i ako je to slucaj implementira se odbijanje od njih */
	
	float imp_height = 15.0;
	float height = 13.2;
	int i;
	for (i = 0; i < num_of_air; i++){
		
		if(x_coor >= air_imp[i].begin &&  x_coor < air_imp[i].end - 1  && y_coor < imp_height && y_coor > height){
			x_coor = air_imp[i].begin - 2;
			return 1;
		}
		if (x_coor >= air_imp[i].end + 0.2 && on_imp_air[i]){
			in_air = 1;
			if(y_coor <= imp_height + 8.2)
				down = 1;
			on_imp_air[i] = 0;
			on_imp_air2[i] = 0;
			on_timer(TIMER_ID);
		}
	}
	
	return 0;
}

static void check_left(){
	if (check_left_green_imp())
		return;
	if(check_left_air_imp())
		return;
	
	x_coor -= SPEED;
}

static int check_left_green_imp(){
	/*proveravamo da li se iza nas nalazi zelena prepreka
	 Isto ka i kod kretanja desno gledamo da li imamo pad sa prepreke */
	int i;
	float height = 8;
	for (i = 0; i < num_of_green; i++){
		if (x_coor  > green_imp[i].begin + 1 && x_coor <= green_imp[i].end && y_coor <= height){
			return 1;
		}
		if (on_imp[i] && x_coor < green_imp[i].begin){
			in_air = 1;
			if(y_coor <= height + 2)
				down = 1;
			on_imp[i] = 0;
			on_imp2[i] = 0;
			on_timer(TIMER_ID);
		}
	}
	
	return 0;
}

static int check_left_air_imp(){
	/* sada proveravamo da li se nalazimo na preprekama u vazduhu i implementiramo  pad, implementiramo odbijanje od prepreke */

	float imp_height = 15.0;
	float height = 13.2;
	int i;
	for ( i = 0; i < num_of_air; i++){
		
		if(x_coor > air_imp[i].begin + 1 && x_coor <= air_imp[i].end && y_coor < imp_height && y_coor > height){
			x_coor = air_imp[i].end + 2;
			return 1;
		}
		if (x_coor <= air_imp[i].begin && on_imp_air[i]){
			in_air = 1;
			if(y_coor <= imp_height + 8.2)
				down = 1;
			on_imp_air[i] = 0;
			on_imp_air2[i] = 0;
			on_timer(TIMER_ID);
		}
	}
	return 0;
}

static void check_jump(){
	/* proveravamo da nasu poziciju, to jest da li ako nastavimo skok u vis prolazimo kroz prepreku i ako je to slucaj sprecavamo */
	/* i omogucavamo skok na prepreku */
	/* takodje implementira se skok sa visece prepreke */
	float imp_height = 15.0;
	float height = 12.9;
	y_coor += speed;
	int i;
	for (i = 0; i < num_of_air; i++){
		
		if (x_coor >= air_imp[i].begin && x_coor < air_imp[i].end && y_coor >= height ){
			if(!extraJump){
				speed =- speed*2;
				return;
			}
			else if ( y_coor >= imp_height + 7 && y_coor <= imp_height + 7.5){
				on_imp_air[i] = 1;
				on_imp_air2[i] = 1;
				in_air = 0;
				extraJump = GIPSECOND;
			}
		
		}
	}

	check_jump_green();
	
}


static void check_jump_green(){

	float height = 8.4;
	/*proveravamo da li se nalazimo na zelenoj prepreci, sprecavamo da prodjemo kroz nju */
	int i;
	for (i = 0; i < num_of_green; i++){
		if (x_coor >= green_imp[i].begin && x_coor < green_imp[i].end && y_coor >= height && y_coor <= height + 0.8){
			on_imp[i] = 1;
			on_imp2[i] = 1;
			in_air = 0;
			extraJump = GIP;
		}
	}

}

static void on_timer(int value){
		/*proveravamo da li dolazi sa tajmera za skok*/
		if(value!=TIMER_ID){
			exit(0);
		}
		check_jump();	
		if (checkImp())
			speed = JUMPSPEED;
		if (checkImp2())
			speed = JUMPSPEED;
		if(y_coor>=MAXREACH + extraJump || down){
			if (speed> 0 )
				speed=-speed*1.5;
			down = 0;
			flag = 0;
		}
		if(y_coor<=0){
			in_air = 0;
			speed = JUMPSPEED;
			y_coor = 0;
			down = 0;
			extraJump = 0;
			flag = 0;
		}
		glutPostRedisplay();
		if(in_air)
           		 glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
}        

/* ispisuje poruku na ekran */

static void draw_text(char* s){
	glColor3f(1,0,0);
 	glRasterPos3f(-10, 5.7, -2.9);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, s);
	ind = 0;
}


/* vraca tacno ako stojimo na nekoj prepreci */
static int checkImp(){
	int i;
	for (i = 0; i < COUNTIMP; i++)
		if (on_imp2[i] == 1){
			on_imp2[i] = 0;
			return 1;
		}
	return 0;
}
static int checkImp2(){
	int i;
	for (i = 0; i < COUNT; i++)
		if (on_imp_air2[i] == 1){
			on_imp_air2[i] = 0;
			return 1;
		}
	return 0;
}
