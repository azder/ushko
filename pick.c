#define BUFFER_GOLEMINA					1024

#define IME_NEMA								0
#define IME_ZHIVOTNO						1

#define IME_TORZO								1
#define IME_VRAT								2
#define IME_GLAVA								3
#define IME_OKO_LEVO						4
#define IME_OKO_DESNO						5
#define IME_NOS									6
#define IME_NOGA_DOLU_DESNO			7
#define IME_NOGA_DOLU_LEVO			8
#define IME_NOGA_GORE_DESNO			9
#define IME_NOGA_GORE_LEVO			10
#define IME_STAPALO_DOLU_DESNO	11
#define IME_STAPALO_DOLU_LEVO		12
#define IME_STAPALO_GORE_DESNO	13
#define IME_STAPALO_GORE_LEVO		14

GLuint odbrano = IME_NEMA;


/**
	ja mesti proekciskata matrica za odbiranje
	t.e. pravi da se iscrtuva samo mal del
	okolu glushecot
*/

void setPick(int x, int y){

	/*
		definiraj niza vp[] koja shto 
		kje gi sodrzhi {x,y,shirina,visina}
		od viewport-ot
	*/
	GLint vp[4];
	
	/*
		gi vchitaj gi vrednostite za 
		parametrite na viewport-ot
	*/
	glGetIntegerv(GL_VIEWPORT, vp); 
	
	/*
		odberi ja matricata za proekcija
		i resetiraj ja
	*/
	glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  /*
		sozdadi ja matricata za selekcija taka shto
		kje bide centrirana na x i y
		(zradi glushecot mora visina na viewport minus y)
		i so shirina i visina od 4 edinici
  */
	gluPickMatrix( (GLdouble) x, (GLdouble) (vp[3] - y), 4.0, 4.0, vp );

	/*
		namesti go pogledot na istite vrednosti
		kako i vo funkcijata onReshape
	*/
	gluPerspective(
		45, /* Field Of Vision = 45 stepeni*/
		(float)vp[2]/vp[3], /* soodnos shirina/visina */
		0.01, /* bliskata ramnina na otsekuvanje */
		1000 /* dalechnata ramnina na otsekuvanje */ 
	); //gluPerspective
	
	/*
		vrati se vo rezhim na menuvanje
		na modelview matricata
	*/
	glMatrixMode(GL_MODELVIEW);
	
} //setPick


/**
	gi obrabotuva t.n. pogodoci (hits)
	t.e. iscrtanite raboti vo
	maliot prostor okolu glushecot
*/

void hits(int hits, GLuint *buffer){
	
	/* obrabotka na 'krajni sluchai' */
	odbrano=IME_NEMA;
	if(hits<0) return;
	if(hits==0) return;
		
	/* pokazhuvach kon baferot */
	GLuint *ptr = (GLuint *) buffer;
	
	/*
		minimalnoto z se inicijalizira
		so najgolemiot nenegativen broj
		'-1' binarno e '11111....1111'
	*/
	GLfloat z = (GLfloat)((GLuint) -1);
	
	/* odbrano shto momentalno se obrabotuva */
	GLuint o = IME_NEMA;
	
  printf( "HITS: HITS %u\n",hits);
  int i, j;
  
	for(i = 0; i < hits; i++){
		
	  /* broj na iminja vo pogodokot */
	  GLuint n = *ptr;	ptr++;
	  /* minimalen z na 'iscrtanoto' */
	  GLfloat minz = (float)*ptr/0x7fffffff; ptr++;
	  /* maximalen z na 'iscrtanoto' */
	  GLfloat maxz = (float)*ptr/0x7fffffff; ptr++;
	  /* imeto na iscrtanoto */
	  
	  printf( "HITS: i=%u, n=%u\n", i, n );
	  if(n==0){ continue; }
	  
	  GLuint ime = *ptr; ptr++;
	  
	  /*
	  	go obrabotuva samo ako e del od zhivotnoto
	  */
	  if(ime == IME_ZHIVOTNO){
			
			/*
				sledniot element vo nizata e
				imeto na delot od zhivotnoto
				shto e selektiran
			*/
			o = *ptr; ptr++;
			printf( "HITS:\tZHIVOTNO %u\n",o);
			/*
				mora pokazhuvachot da 'izvrti' do n
				t.e. da pokazhuva kon pochetokot na
				sledniot 'iscrtan' element
			*/
			for( j = 2; j < n; j++, ptr++ ) ;
			
			/*
				proveruva dali momentalno razgleduvanoto
				e poblisku od momentalno najbliskoto,
				ako e go pravi momentalno najblisko
			*/
			if(minz<z){
				odbrano=o;
				z=minz;
			} /*if minz<z */		
			
			printf( "HITS:\tODBRANO %u , z = %g \n", odbrano, z );
			
		}else{
			
			printf( "HITS:\time=%u, minz=%g, maxz=%g\n", ime,minz,maxz );
			/*
			 	pokazhuvachot povtorno mora da 'izvrti' do n
			 	bez razlika dali go ima ili nema toa shto ni treba
			*/
			for( j = 1; j < n; j++, ptr++ ) ;
				
	  } /* if-else */
	
	} /* for i */
	
} /* hits */


