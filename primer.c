/**
	programata shto go iscrtuva moeto meche
	Mecheto Ushko
*/

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.c"
#include "shadow.c"
#include "meche2.c"
#include "pick.c"

int gPolyMode = GL_FILL;
int gCamera = 1;
int gDrawLight = 1;

static GLuint gTexNone = 0;
static GLuint gTexBack1;
static GLuint gTexUshko2;
static GLuint gTexCrno;

GLfloat gMovX = 0.0;
GLfloat gMovY = 0.0;
GLfloat gMovZ = 0.0;

GLint gRotX = 0.0;
GLint gRotY = -5.0;
GLint gRotZ = 0.0;

GLfloat gLX = 0.0;
GLfloat gLY = 3.0;
GLfloat gLZ = 0.0;


/**
	ja vchituva teksturata i ja mesti za koristenje
*/

void setTex(GLuint * tex, char * pic){
	
	/* zafati memorija za slikata od teksturata */
	Image *img;	
	img = (Image *) malloc(sizeof(Image));
	
	/*
		vchitaj ja slikata, a ako ne 
		nema dovolno memorija ili ne mozhe da ja vchita
		javi greshka i izlezi od programata
	*/
	
	if(img == NULL){
	  	printf("Error allocating space for image %s", pic);
	    exit(1);
	}
	if( !ImageLoad(pic, img) ) exit(1);
	
	/*
		sozdadi ja teksturata od slikata vo memorijata
		i dodeli i ime, t.e. int broj smesten vo texUshko1
	*/
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	glGenTextures(1, tex);
	glBindTexture(GL_TEXTURE_2D, *tex);   
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR); 
	
	/*
		nerazjasneti problemi so glTexImage2D, no
		zatoa gluBuild2DMipmaps ne pravi problemi
	*/
	/*
	glTexImage2D(
			GL_TEXTURE_2D, 0, 3,
			img->sizeX, img->sizeY,
			0, GL_RGB, GL_UNSIGNED_BYTE,
			img->data
	);
	*/
	
	gluBuild2DMipmaps(
			GL_TEXTURE_2D, GL_RGB, img->sizeX, img->sizeY,
			GL_RGB, GL_UNSIGNED_BYTE, img->data
	); 
	
	
} /* setTex */


/**
	go mesti materijalot za iscrtuvanje
	parametrite se za
	svetkavost, RGB boja i RGB odblesok
*/

void setMat(float shininess, float r, float g, float b, float sr, float sg, float sb){
	
	/* namesti go odblesokot */
	GLfloat specular[4] = { sr, sg, sb, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	
	/* namesti ja svetkavosta */
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	
	/* 
		ambientalnata i difuznata boja na materijalot
		se postavuvaat so istata vrednost od glColor()
	*/
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glColor3f(r, g, b);
	
} /* setMat */

/**
	gi mesti materijalot i teksturata
	za svetlite delovi od mechkata
	vo zavisnost od toa dali e dozvoleno
	da se mestat materijali
	i dali delot e selektiran
*/
void setMatTexLight(int mats, GLuint test){
	
	/* odberi ja teksturata za krznoto na Ushko */
	glBindTexture(GL_TEXTURE_2D, gTexUshko2);
		
	if( mats ){
		if( odbrano == test ){
			setMat(12.0000f, 0.5, 0.5, 1.0, 0.2, 0.2, 0.2);	
		} else {
			setMat(12.0000f, 0.95, 0.95, 1.0, 0.2, 0.2, 0.2);
		} /* if-else odbrano */
	} /* if mats */
	
} /* setMatTexLight */

/**
	gi mesti materijalot i teksturata
	za temnite delovi od mechkata
	vo zavisnost od toa dali e dozvoleno
	da se mestat materijali
	i dali delot e selektiran
*/
void setMatTexDark(int mats, GLuint test){
	
	if( mats ){
		if( odbrano == test ){
			/* koga se odbrani ochite i nosot se svetli */
			glBindTexture(GL_TEXTURE_2D, gTexUshko2);
			setMat(12.0000f, 1.0, 0.5, 0.5, 0.2, 0.2, 0.2);	
		} else {
			/* ochite i nosot se temni */
			glBindTexture(GL_TEXTURE_2D, gTexCrno);
			setMat(12.0000f, 0.95, 0.95, 1.0, 0.2, 0.2, 0.2);
		} /* if-else odbrano */
	} /* if mats */
	
} /* setMatTexDark */

/**
	gi vchituva potrebnite teksturi
	vo memorijata i pravi referenci
	kon niv
*/

void setTextures(void){
	
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);	
	setTex( &gTexBack1,"planina.bmp");
	setTex( &gTexUshko2,"ushko3.bmp");
	setTex( &gTexCrno,"crno.bmp");
	
} /* setTextures */

/**
	go dvizhi spot svetloto
	mu ja menuva polozhbata
*/

GLfloat* setLightPos(void){
	
	float x = gLX;
	float y = gLY;
	float z = gLZ;
	
	/* postavi ja polozhbata na spot svetloto */
	GLfloat svetlo0_polozba[] = { x, y, z, 1.0 };
	GLfloat svetlo0_nasoka[] = { -x, -y, -z };
	glLightfv(GL_LIGHT0, GL_POSITION, svetlo0_polozba);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, svetlo0_nasoka);
	
	/* ispechati polozhba na svetloto */
	/* printf( "light X=%f, Y=%f, Z=%f \n", gLX, gLY, gLZ ); */
	
} /* setLightPos */

/**
	gi mesti svetlata:
	ambientalno i spot
*/

void setLight(void){
	
	/* ovozmozhi gi svetlata */
	glEnable(GL_LIGHTING);
	
	/* postavi go ambientalnoto svetlo */
	GLfloat global_ambient[]= {0.25,0.25,0.25,1};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
		
	/* postavi go spot svetloto */
	GLfloat svetlo0_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	/*
	GLfloat svetlo0_polozba[] = { 0.0, 1.0, -1.0, 1.0 };
	GLfloat svetlo0_nasoka[] = { 0.0, -1.0, -1.0 };
	*/
	GLfloat svetlo0_spot_cut = 60.0;
	GLfloat svetlo0_spot_exp = 2.0;
	/*
	glLightfv(GL_LIGHT0, GL_POSITION, svetlo0_polozba);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, svetlo0_nasoka);
	*/
	glLightfv(GL_LIGHT0, GL_SPOT_CUTOFF, &svetlo0_spot_cut);
	glLightfv(GL_LIGHT0, GL_SPOT_EXPONENT, &svetlo0_spot_exp);
	glLightfv(GL_LIGHT0, GL_AMBIENT, svetlo0_ambient);
	
	/* 'vkluchi' go svetloto */
	glEnable(GL_LIGHT0);
	
} /* setLight */

/**
	gi mesti pochetnite vrednosti za
	svetlo, materijali, teksturi, boi i sl.
*/

void setInit(void){
	
	glClearColor(0.0, 0.0, 0.0, 0.0);
	/*glutFullScreen();*/
	
	/* ovozmozhi gi krienjeto i materijalite */
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	
	/* namesti go svetloto */
	setLight();
	
	/* namesti gi teksturite */
	setTextures();
		
} /* setInit */


/**
	funkcijata za iscrtuvanje
	na mrezha 10x10 napravena
	za N x N
*/

void drawMX(int n) {
	
	glPushMatrix();
	glTranslatef(-n/2,0,-n/2);
	glBegin(GL_QUADS);
	  glNormal3f(0,1,0);
	  float i=0;
	  float j=0;
	  for( i=0; i<n; i++){ 
	    for( j=0; j<n; j++){
	    	glTexCoord2f(0, 0);
	    	glVertex3f(i,  0,j);
	      glTexCoord2f(1, 0);
	      glVertex3f(i+1,0,j);
	      glTexCoord2f(1, 1);
	      glVertex3f(i+1,0,j+1);
	      glTexCoord2f(0, 1);
	      glVertex3f(i,  0,j+1);
	    }
	  }
	glEnd();
	glPopMatrix();

} /* drawMX */

/**
	funkcija za iscrtuvanje
	na kvadrat so N x N strani
	i tekstura
*/

void drawSQ(int n) {
	
	float n2 = n/2;
	glPushMatrix();
	glBegin(GL_QUADS);
		glNormal3f(0,1,0);
		glTexCoord2f(0, 0);
		glVertex3f(-n2,  0,-n2);
		glTexCoord2f(1, 0);
		glVertex3f(n2,0,-n2);
		glTexCoord2f(1, 1);
		glVertex3f(n2,0,n2);
		glTexCoord2f(0, 1);
		glVertex3f(-n2,  0,n2);
	glEnd();
	glPopMatrix();

} /* drawSQ */

/**
	go iscrtuva spot svetloto
*/

void drawLight(void){
	if(!gDrawLight) return;
	glPushMatrix();
		/*glLoadIdentity();*/
		setMat(96.0000f, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 );
		glTranslatef( gLX, gLY, gLZ	);
		glutSolidSphere(0.1,10,10);
	glPopMatrix();	
} /* drawLight */



/**
	go crta Mecheto Ushko
	parametarot mats e true/false
	za toa dali da se koristat materijali
*/

void drawUshko(int mats){
	
	/*
		go mesti zhivotnoto za selekcija
		potoa so push i pop samo se dodavaat
		iminjata na delovite od zhivotnoto
		posle IME_ZHIVOTNO
	*/
	if(mats) glPushName(IME_ZHIVOTNO);
	
	/* namesti go materijalot na krznoto */
	if(mats) setMat(12.0000f, 0.95, 0.95, 1.0, 0.2, 0.2, 0.2);
	
	/*
		iscrtaj go Ushko, del po del
		funkciite se vo druga datoteka
		export-irani od 3d programa	
	*/
	glPushMatrix();
	
		if(mats) glPushName(IME_GLAVA);
		setMatTexLight(mats,IME_GLAVA);
		glava02();
		if(mats) glPopName();

		if(mats) glPushName(IME_OKO_DESNO);
		setMatTexDark(mats,IME_OKO_DESNO);
		okoDesno02();
		if(mats) glPopName();
		
		if(mats) glPushName(IME_OKO_LEVO);
		setMatTexDark(mats,IME_OKO_LEVO);
		okoLevo02();
		if(mats) glPopName();
		
		if(mats) glPushName(IME_NOS);
		setMatTexDark(mats,IME_NOS);
		if(mats) nos02();
		glPopName();
		
		if(mats) glPushName(IME_VRAT);
		setMatTexLight(mats,IME_VRAT);
		vrat02();
		if(mats) glPopName();
		
		if(mats) glPushName(IME_TORZO);
		setMatTexLight(mats,IME_TORZO);
		torzo02();
		if(mats) glPopName();
		
		if(mats) glPushName(IME_NOGA_DOLU_DESNO);
		setMatTexLight(mats,IME_NOGA_DOLU_DESNO);
		nogaDoluDesno02();
		if(mats) glPopName();
		
		if(mats) glPushName(IME_STAPALO_DOLU_DESNO);
		setMatTexLight(mats,IME_STAPALO_DOLU_DESNO);
		stapaloDoluDesno02();
		if(mats) glPopName();
		
		if(mats) glPushName(IME_NOGA_DOLU_LEVO);
		setMatTexLight(mats,IME_NOGA_DOLU_LEVO);
		nogaDoluLevo02();
		if(mats) glPopName();
		
		if(mats) glPushName(IME_STAPALO_DOLU_LEVO);
		setMatTexLight(mats,IME_STAPALO_DOLU_LEVO);
		stapaloDoluLevo02();
		if(mats) glPopName();
		
		if(mats) glPushName(IME_NOGA_GORE_DESNO);
		setMatTexLight(mats,IME_NOGA_GORE_DESNO);
		nogaGoreDesno02();
		if(mats) glPopName();
		
		if(mats) glPushName(IME_STAPALO_GORE_DESNO);
		setMatTexLight(mats,IME_STAPALO_GORE_DESNO);
		stapaloGoreDesno02();
		if(mats) glPopName();
		
		if(mats) glPushName(IME_NOGA_GORE_LEVO);
		setMatTexLight(mats,IME_NOGA_GORE_LEVO);
		nogaGoreLevo02();
		if(mats) glPopName();
		
		if(mats) glPushName(IME_STAPALO_GORE_LEVO);
		setMatTexLight(mats,IME_STAPALO_GORE_LEVO);
		stapaloGoreLevo02();
		if(mats) glPopName();
		
	glPopMatrix();
	
	if(mats) glPopName(); /* IME_ZHIVOTNO */
	
	/* napravi da nema odbrana tekstura */
	glBindTexture(GL_TEXTURE_2D, gTexNone);
	
} /* drawUshko */

/**
	ja rotira kamerata vo odnos na
	globalnite promenlivi gMov*, gRot*
*/

void setCamera(void){

	/*
		izrotiraj ja kamerata
	*/
	
	glTranslatef(gMovX, 0, 0);
	glTranslatef(0, gMovY, 0);
	glTranslatef(0, 0, gMovZ);
	glRotatef(gRotX, 1, 0, 0);
	glRotatef(gRotY, 0, 1, 0);
	glRotatef(gRotZ, 0, 0, 1);
	
} /* setCamera */

/**
	ja iscrtuva scenata
	parametarot mats e true/false
	za toa dali da se koristat materijali
*/

void drawSC(void){

	/* izrotiraj ja celata scena */
	if(!gCamera) setCamera();
	
	/* iscrtaj go svetloto */
	drawLight();
	
	/* iscrtaj ja podlogata */
	glPushMatrix();
		glTranslatef(0.0, 0.2, 0.0);
		setMat(0.0000f, 0.5, 0.55, 0.5, 0.0, 0.0, 0.0);
		drawMX( 15 );
	glPopMatrix();
	
	/* iscrtaj ja pozadinata */
	glPushMatrix();
		glTranslatef(0.0, 6.0, -2.0);
		glScalef(1.0,-1.0,1.0);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, gTexBack1);
		setMat(0.0000f, 0.5, 0.55, 0.5, 0.0, 0.0, 0.0);
		drawSQ( 12 );
	glPopMatrix();
	
	/*
	izrotiraj ja samo mechkata,
	a ne i podlogata & pozadinata
	*/
	if(gCamera) setCamera();
	
	/* iscrtaj go Mecheto Ushko */
	drawUshko(1);

	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
		
		/*
			senkata ja vrzuvame za ramninata
			na podlogota kade shto
			treba da bide iscrtana
		*/
		GLfloat points[3][3] = {
			{ -10.0, 0.21, -10.0 },
			{ -10.0, 0.21, +10.0 },
			{ +10.0, 0.21, +10.0 }
		}; /* points */
		
		/* matricata na transformacija za senkata */
		GLfloat shadowMat[4][4];
		/* polozhbata na svetloto mora da bide niza */
		GLfloat svetlo0_polozba[] = { gLX, gLY, gLZ, 1.0 };
		/* sozdadi ja matricata za senkata */
		MakeShadowMatrix(points, svetlo0_polozba, shadowMat);
		/*
			primeni ja transformacijata vrz 
			momentalnata modelview matrica
			za da se dobie senkata
		*/
		glMultMatrixf((GLfloat *)shadowMat);
		/* bojata na senkata */
		glColor4f(0.1,0.1,0.1,0.5);

		/* iscrtaj go ushko kako senka */
		drawUshko(0);
		
	glEnable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopAttrib();	

} /* drawSC */


/**
	ja prikazhuva scenata
*/

void onDisplay(void){
	
	/*
		namesti gi promenetite parametri
		i ischisti go pogledot 
	*/
	glPolygonMode(GL_FRONT_AND_BACK, gPolyMode);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	/* namesti go pogledot */
		
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
			0,1,8, /* od kade gledam */
			0,0,0, /* na kade gledam */
			0,1,0 /* kade e 'gore' */
	);
	
	setLightPos();
	
	/* inicijaliziraj gi iminjata */
	glInitNames();
	glPushName(IME_NEMA);
	
	/* iscrtaj ja scenata */
	drawSC();
	
	glPopName();
	
	/* smeni baferi, t.e. iscrtaj */
	glutSwapBuffers();
	
} /* onDisplay */

/**
	go 'obrabotuva' :) vlezot od glushecot
*/

void onMouse(int button, int state, int x, int y){
	
	/*
		go inicijalizira baferot so nuli
	*/
	GLuint buffer[BUFFER_GOLEMINA] = {0};
	
	/*
		selektiranjeto raboti samo so lev klik
		i toa pri pritiskanje ne i pri otpushtanje
	*/
	if(button == GLUT_LEFT_BUTTON && state==GLUT_DOWN){
		
		glSelectBuffer(BUFFER_GOLEMINA, buffer);
		glRenderMode(GL_SELECT);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
			setPick(x,y);
			drawSC();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		hits(glRenderMode(GL_RENDER), buffer);
		
	} /* if */

	glutPostRedisplay();
	
} /* onMouse */

/**
	go 'obrabotuva' :) vlezot od tastaturata
*/

void onKeyboard(unsigned char key, int x, int y){
	
	static int fullscreen = 0;
	
	switch (key) {
		
		/* pritisnato e ESC, izlezi */
		case 27: exit(0); break;
			
		/* smeni go iscrtuvanjeto na poligonite */
		case '1': gPolyMode=GL_LINE; glutPostRedisplay(); break;
		case '2': gPolyMode=GL_FILL; glutPostRedisplay(); break;
			
		/*
			smeni ja kamerata:
			dali samo mechkata da se dvizhe
			ili cela scena
		*/
		case '3': gCamera = !gCamera; break;
			
		/* dali da se iscrtuva svetloto */
		case '4': gDrawLight = !gDrawLight; glutPostRedisplay(); break;
		
		/* full screen ili ne */
		case '0':
			fullscreen = ! fullscreen;
			if(fullscreen){
				glutFullScreen();
			}else{
				glutPositionWindow(100, 100);
				glutReshapeWindow(750, 550);
			} //if-else
		break; /* case '0' */
		 
		/* smeni gi vrednostite za rotiranje na kamerata */
		case 'j': gRotY = (gRotY - 5) % 360; glutPostRedisplay(); break;
		case 'l': gRotY = (gRotY + 5) % 360; glutPostRedisplay(); break;
		case 'i': gRotX = (gRotX - 5) % 360; glutPostRedisplay(); break;
		case 'k': gRotX = (gRotX + 5) % 360; glutPostRedisplay(); break;
		case 'u': gRotZ = (gRotZ - 5) % 360; glutPostRedisplay(); break;
		case 'o': gRotZ = (gRotZ + 5) % 360; glutPostRedisplay(); break;
		
		/* smeni gi vrednostite za pridvizhuvanje na kamerata */
		case 'a': gMovX -= 1; glutPostRedisplay(); break;
		case 'd': gMovX += 1; glutPostRedisplay(); break;
		case 'w': gMovY += 1; glutPostRedisplay(); break;
		case 's': gMovY -= 1; glutPostRedisplay(); break;
		case 'q': gMovZ -= 1; glutPostRedisplay(); break;
		case 'e': gMovZ += 1; glutPostRedisplay(); break;
			
		case 'c': gLX -= 1; glutPostRedisplay(); break;
		case 'b': gLX += 1; glutPostRedisplay(); break;
		case 'f': gLZ -= 1; glutPostRedisplay(); break;
		case 'v': gLZ += 1; glutPostRedisplay(); break;
		
		/* pritisnat e ENTER, vrati gi vrednostite na pochetnite */
		case 13:
			gMovX = gMovY = gMovZ = gRotX = gRotY = gRotZ = 0;
			gLX = gLZ = 0;
			/* gLY = 3.0; */
			setLightPos();
			glutPostRedisplay();
		break; /* case ENTER */
		
		default:
			printf( "key=%d ('%c'), x=%d, y=%d \n", key, x, y );
		
	} /* kraj na switch */
	
} /* onKeyboard */

/**
	Funkcija shto se povikuva koga kje se smeni
	goleminata na prozorecot
	treba da go nameste pogledot pravilno
*/

void onReshape(int w, int h){
	
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	/*
	ne glFrustum(-10, 10, -10, 10, 10, 50);
	polesno e so gluPerspective, iako taa 
	vnatreshno go povikuva glFrustrum
	*/
	
	gluPerspective(
		45,
		(float)w/h,
		0.01,
		1000
	);
	
	glMatrixMode(GL_MODELVIEW);
	
	/*
		da se iscrta scenata vo prozorecot
		so novite dimenzii
	*/
	glutPostRedisplay();
	
} /* onReshape */




/**
	glavnata funkcija
	povrzi se so openGL, inicijaliziraj go
	i kazhi mu koi funkcii da gi povikuva
*/

int main(int argc, char** argv){
	
	/*
		inicijaliziraj go openGL,
		namesti dvoen bafer i rezhim na boja,
		sozdadi prozorec so odredena golemina
		i polozhba
	*/
	
	glutInit(&argc, argv);
  glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
  glutInitWindowSize(750, 550);
  glutInitWindowPosition(100, 100);
  
  /*
  	za da se iskluchi prozorecot so klikanje na X 
  	ako pravi problemi zameni ja so slednata funkcija
  */
	__glutCreateWindowWithExit("Moeto Meche - Mecheto Ushko",exit);
  /*glutCreateWindow("Moeto Meche - Mecheto Ushko");*/
  
  /* inicijaliziraj gi potrebnite raboti */
  setInit();
  
  /*
  	funkcii shto se povikuvaat od openGL
  	callback funkcii
  */
  glutDisplayFunc(onDisplay);
  glutReshapeFunc(onReshape);
  glutKeyboardFunc(onKeyboard);
  glutMouseFunc(onMouse);
  
  /* ovaa treba za animacija */
  /*glutIdleFunc(onIdle);*/
  
  /* zapochni so rabota vekje ednash :) */
  glutMainLoop();
  
  return 0;
  
} /* main */

          

