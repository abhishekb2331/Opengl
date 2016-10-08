//#ifdef __APPLE__
//#include <GLUT/glut.h>
//#else
//#include <GL/glut.h>
//#endif


//sudo apt-get install sox

#include<unistd.h>
#include<sys/utsname.h>
#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<signal.h>
#include "GL/glu.h"
//glm not required
//#include <glm.hpp>
//#include <gtc/matrix_transform.hpp>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <cmath>
#include<string.h>
#include <GL/glut.h>
using namespace std;

#define PI 3.141592653589
#define DEG2RAD(deg) (deg * PI / 180)
typedef struct ll {
	float box_x;
	float box_y;
	//	float box_velx;
	float box_vely;
	float box_len;
	int color;
	struct ll *next;
	int flag;
}spider;

typedef struct laser{
	float laser_x;
	float laser_y;
	float angle;
	struct laser *next;
}laser;

// Function Declarations
void startscene();
void score3();
void score4();
void score5();
void score6();
void drawScene();
void drawScene2();
void incontainer(int value);
void remove(int value);
void over(int value);
void drag(int x,int y);
void drag_rotate(int x,int y);
void drawrect();
void drawborder(float len);
void drawline();
void drawleg(float len);
void drawspider(float len);
void drawredbox();
void drawCanon();
void change(int value);
void drawgreenbox();
void final(int value);
void update(int value);
void drawBox(float len);
void drawBall(float rad);
void My_mouse_routine(int x,int y);
void GetOGLPos(int x, int y);
void drawTriangle();
void initRendering();
void show_score();
void gen_spider(int value);
void handleResize(int w, int h);
void handleKeypress1(unsigned char key, int x, int y);
void handleKeypress2(int key, int x, int y);
void handleMouseclick(int button, int state, int x, int y);

float mouseposx;
float mouseposy;
float mouseposz;

int scores=0;
spider *list=NULL;
laser *bullet=NULL;
// Global Variables
int drag2=0;
int drag1=0;
int timing=0;
int pausing=1;
int prev=-1000;
int overflag=0;
float ball_x = 1.0f;
float ball_y = 0.0f;
float ball_velx = 0.01f;
float ball_vely = 0.02f;
float ball_rad = 0.2f;
float box_len = 4.0f;
float tri_x = 0.0f;
float tri_y = 0.0f;
float theta = 0.0f; 

float canon_len=1.0f;
float mouse_x;
float mouse_y;
float canon_x=0;
float canon_y=-(box_len/2)+(canon_len)*0.4330127;
float red_basket_x=-2.0f;
float red_basket_y=-2.0f;
float red_basket_lenx=1.0f;
float red_basket_leny=1.0f;
float green_basket_y=-2.0f;
float green_basket_lenx=1.0f;
float green_basket_x=(box_len/2)-green_basket_lenx;
float green_basket_leny=1.0f;
float line_length=0.8f;
int last_key_pressed=0;
float array[1000];
int check[1000];
int arraylen;
int diff;
int start=0;
int control=0;
int main(int argc, char **argv) {
	// Initialize GLUT
	printf("SELECT DIFFICULTY LEVEL :\n");
	printf("1. EASY\n");
	printf("2. MEDIUM\n");
	printf("3. HARD\n");
	scanf("%d",&diff);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	int w = glutGet(GLUT_SCREEN_WIDTH);
	int h = glutGet(GLUT_SCREEN_HEIGHT);
	int windowWidth = w * 2 / 3;
	int windowHeight = h * 2 / 3;
	//printf("%d %d\n",windowWidth,windowHeight);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition((w - windowWidth) / 2, (h - windowHeight) / 2);

	glutCreateWindow("Assignment-1");  // Setup the window
	initRendering();
	float i;
	int j=0;
	for(i=0.3;i+0.3<=(box_len/2)*1.0;i+=0.5)
	{
		array[j++]=i;
		//	    printf("%f\n",i);
		//	    i+=0.4;
	}
	arraylen=j;
	//printf("arraylength: %d\n",arraylen);
	// Register callbacks
	printf("%d\n",start);
	if(overflag!=1 && start!=0)
	{
		glutDisplayFunc(drawScene);
		glutIdleFunc(drawScene);
	}
	else if(start!=0)
	{
		glutDisplayFunc(drawScene2);
		glutIdleFunc(drawScene2);
	}
	if(start==0 && control!=1)
	{
		glutDisplayFunc(startscene);
		glutIdleFunc(startscene);
	}
	if(control==1)
	{
	
	}
	glutKeyboardFunc(handleKeypress1);
	glutSpecialFunc(handleKeypress2);
	glutMouseFunc(handleMouseclick);
	glutReshapeFunc(handleResize);
	glutMotionFunc(drag);
	//	glutMotionFunc(drag_rotate);
	glutPassiveMotionFunc( My_mouse_routine );
	glutTimerFunc(10, update, 0);
	glutTimerFunc(3000, gen_spider, 0);
	glutTimerFunc(10, change, 0);
	glutTimerFunc(1,remove, 0);
	glutTimerFunc(50,final, 0);
	glutTimerFunc(10,incontainer,0);
	glutTimerFunc(10,over,0);
	glutMainLoop();
	return 0;
}
void GetOGLPos(int x, int y)
{
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

	gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
	mouse_x=posX*100/2;
	mouse_y=(posY*100)/2;
	//gc=posZ*100;
}
void drag(int x,int y)
{
	//	printf("YES\n");
	if(drag2==1)
	{

		spider *tmp=list;
		float canon_lower=-box_len/2,canon_upper=box_len/2,red_lower=-box_len/2,red_upper=box_len/2,green_lower=-box_len/2,green_upper=box_len/2;
		while(tmp!=NULL)
		{
			if(tmp->box_y-0.1<(-box_len/2))
			{
				if(tmp->box_x<=canon_x && tmp->box_x>canon_lower)
				{
					canon_lower=tmp->box_x;
				}
				if(tmp->box_x>=canon_x && tmp->box_x<canon_upper)
				{
					canon_upper=tmp->box_x;
				}
				if(tmp->box_x<=red_basket_x && tmp->box_x>red_lower)
				{
					red_lower=tmp->box_x;
				}
				if(tmp->box_x>=(red_basket_x) && tmp->box_x<red_upper)
				{
					red_upper=tmp->box_x;
				}
				if(tmp->box_x<=green_basket_x && tmp->box_x>green_lower)
				{
					green_lower=tmp->box_x;
				}
				if(tmp->box_x>=(green_basket_x) && tmp->box_x<green_upper)
				{
					green_upper=tmp->box_x;
				}
			}
			tmp=tmp->next;
		}
		float pt_x=x,pt_y=y;
//		pt_x=((pt_x-(853/2))/231)*box_len/2;
//		pt_y=-((pt_y-240)/480)*box_len;
		GetOGLPos(x, y);
		pt_x=mouse_x;
		pt_y=mouse_y;
//		printf("%f %f\n",pt_x,pt_y);
		if(pt_x<=2.0 && pt_x>=-2.0f && pt_y<=2.0f && pt_y>=-2.0f)
		{
			if(last_key_pressed==67)
			{
				//				if(canon_x+(canon_len/2)<=(canon_upper))
				//					canon_x+=0.1;
				if(((pt_x)-(canon_len/2)>=canon_lower)&&(pt_x+(canon_len/2)<=(canon_upper)))
				{
					//canon_x-=0.1;
					canon_x=pt_x;
				}
			}
			if(last_key_pressed==82)
			{
				if(((pt_x)>red_lower)&&((pt_x+red_basket_lenx)<=red_upper))
				{
					//				if(pt_x>-(box_len/2) && pt_x+red_basket_lenx<(box_len/2))
					red_basket_x=pt_x;
				}
				//		printf("%f %f\n",pt_x,pt_y);
			}
			if(last_key_pressed==71)
			{
				if(((pt_x)>green_lower)&&((pt_x+green_basket_lenx)<=green_upper))
				{
					//if(pt_x>-(box_len/2) && pt_x+green_basket_lenx<(box_len/2))
					green_basket_x=pt_x;
				}
			}
		}
	}
	else if(drag1==1)
	{
		float pt_x=x,pt_y=y,theta1;
		pt_x=((pt_x-(853/2))/231)*box_len/2;
		pt_y=-((pt_y-240)/480)*box_len;
		float angle=atan((pt_y-canon_y)/(canon_x-pt_x));
		if(angle>0)
		{
			theta1=90-angle*180/3.14;
			if(theta1<=60)
				theta=90-angle*180/3.14;
			//			printf("1 : %f\n",theta);
		}
		else
		{
			theta1=270-(angle*180/3.14)-360;
			if(theta1>=-60)
				theta=270-(angle*180/3.14)-360;
			//			printf("2 : %f\n",theta);
		}
	}
}
void drag_rotate(int x,int y)
{
	if(drag1==1)
	{
		float pt_x=x,pt_y=y,theta1;
//		pt_x=((pt_x-(853/2))/231)*box_len/2;
//		pt_y=-((pt_y-240)/480)*box_len;
		GetOGLPos(x,y);
		pt_x=mouse_x;
		pt_y=mouse_y;
		float angle=atan((pt_y-canon_y)/(canon_x-pt_x));
		if(angle>0)
		{
			theta1=90-angle*180/3.14;
			if(theta1<=60)
				theta=90-angle*180/3.14;
			//			printf("1 : %f\n",theta);
		}
		else
		{
			theta1=270-(angle*180/3.14)-360;
			if(theta1>=-60)
				theta=270-(angle*180/3.14)-360;
			//			printf("2 : %f\n",theta);
		}
	}
}
void My_mouse_routine(int x, int y)
{
	GetOGLPos(x,y);
	//printf("%f %f\n",mouse_x,mouse_y);
//	mouse_x = x; //place current mouse pos in mouse_x
//	mouse_y = y;
//	mouse_x=((mouse_x-(853/2))/231)*box_len/2;
//	mouse_y=-((mouse_y-240)/480)*box_len;
	//	printf("%f %f %f %f\n",mouse_x,mouse_y,((mouse_x-(853/2))/231)*box_len/2,-((mouse_y-240)/480)*box_len);
}
void gen_spider(int value)
{
	if(pausing!=1 && overflag!=1)
	{
		spider *prev=list,*nex=list;
		int times;
		int c,i,k,u;
		float p;
		for(i=0;i<arraylen;i++)
			check[i]=0;
		times=(rand()%diff)+1;
		for(i=0;i<times;i++)
		{
			nex=list;
			prev=list;
			if(list==NULL)
			{
				list=(spider *)(malloc(sizeof(spider)));
				/*		k=(int)(box_len/2);*/
				k=arraylen;
				u=((((rand())%(k))));
				while(check[u]!=0)
					u=((((rand())%(k))));
				check[u]=1;
				p=array[u];
				//printf("%f\n",p);
				//		while(p<0.2f)
				//			p=((((rand()+100)%(k)))*1.0)/100;
				//		printf("%f\n",p);
				c=rand()%2;
				if(c==0)
					list->box_x=-p;
				else
					list->box_x=p;
				list->box_y=box_len/2;
				//				list->box_vely=0.01f;
				list->box_vely=((100+(rand()%40))*1.0)/10000;
				list->box_len=0.4f;
				c=rand()%3;
				list->color=c;		//0 for black  1 for green 2 for red
				int hy=rand()%50;
				if(hy>=40)
					list->flag=1;
				else
					list->flag=0;
				list->next=NULL;
			}
			else
			{
				while(nex->next!=NULL)
				{
					prev=nex;
					nex=nex->next;
				}
				nex->next=(spider *)(malloc(sizeof(spider)));
				prev=nex->next;
				//k=(box_len/2);
				k=arraylen;
				u=((((rand())%(k))));
				while(check[u]!=0)
					u=((((rand())%(k))));
				check[u]=1;
				p=array[u];
				//printf("%f\n",p);
				//	p=(((rand())%(k)));
				//	printf("%f\n",p);
				c=rand()%2;
				if(c==0)
					prev->box_x=-p;
				else
					prev->box_x=p;
				prev->box_y=box_len/2;
				//prev->box_vely=0.01f;
				prev->box_vely=(((100+(rand()%40))*1.0)/10000);
				prev->box_len=0.4f;
				c=rand()%3;
				prev->color=c;		//0 for black  1 for green 2 for red
				int hy=rand()%50;
				if(hy>=40)
					prev->flag=1;
				else
					prev->flag=0;
				prev->next=NULL;
				nex->next=prev;
			}
		}
	}
	glutTimerFunc(3000, gen_spider, 0);
}
void drawScene2()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_BLEND); // Enable the OpenGL Blending functionality 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set the blend mode to blend our current RGBA with what is already in the buffer
	glLoadIdentity();
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -5.0f);
	glColor3f(0.1f, 0.1f, 0.2f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);
	glVertex2f(-box_len / 2,( -box_len / 2)-1.0f);
	glVertex2f(box_len / 2, (-box_len / 2)-1.0f);
	glVertex2f(box_len / 2, (box_len / 2)+1.0f);
	glVertex2f(-box_len / 2, (box_len / 2)+1.0f);
	glEnd();
	glColor3f(1.0,0.0,0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
	glVertex2f(-0.7,-0.5);
	glVertex2f(0.7,-0.5f);
	glVertex2f(0.7,0.5);
	glVertex2f(-0.7,0.5);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	char sh[1000],s[1000];
	int l,i;
	sprintf(sh,"%d",scores);
	l=strlen(sh);
	glColor3f(1.0, 0.0, 0.0);
	glRasterPos2f(-0.05, -0.1);
	//	string s = "HERE I AM";
	void * font = GLUT_BITMAP_9_BY_15;
	for (i =0; i<l; i++)
	{
		char c =sh[i];
		glutBitmapCharacter(font, c);
	}
	sprintf(s,"%s","FINAL SCORE");
	l=strlen(s);
	//printf("%s\n",s);
	glColor3f(1.0, 0.0, 0.0);
	glRasterPos2f(-0.45, 0.1);
	void * font1 = GLUT_BITMAP_9_BY_15;
	for (i =0; i<l; i++)
	{
		char c =s[i];
		glutBitmapCharacter(font1, c);
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//    drawBox(box_len);
	glPopMatrix();
	glutSwapBuffers();
	int t=6000;
	while(t--)
	{
	}
	exit(0);
}
void startscene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_BLEND); // Enable the OpenGL Blending functionality 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set the blend mode to blend our current RGBA with what is already in the buffer
	glLoadIdentity();
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -5.0f);
	glColor3f(0.1f, 0.1f, 0.2f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);
	glVertex2f(-box_len / 2,( -box_len / 2)-1.0f);
	glVertex2f(box_len / 2, (-box_len / 2)-1.0f);
	glVertex2f(box_len / 2, (box_len / 2)+1.0f);
	glVertex2f(-box_len / 2, (box_len / 2)+1.0f);
	glEnd();
	glPushMatrix();
	glTranslatef(canon_x,3+canon_y-0.15,0.0f);
	glRotatef(theta,0.0f,0.0f,1.0f);
	if(last_key_pressed==67 || last_key_pressed==99)
		glColor3f(0.0f,0.0f,0.0f);
	else
		glColor3f(0.128f,0.128f,0.0f);
	//    drawrealcanon();
	drawCanon();

	glPushMatrix();
	//    glTranslatef(-0.9f,0.0f,0.0f);
	glTranslatef(-(canon_len*1.0)/2.5,0.0f,0.0f);
	glColor3f(0.0f,0.0f,0.0f);
	drawrect();
	glPopMatrix();
	glPushMatrix();
	glTranslatef((canon_len*1.0)/2.5,0.0f,0.0f);
	glColor3f(0.0f,0.0f,0.0f);
	drawrect();
	glPopMatrix();
	//draw canon aim fire line
	/*  glPushMatrix();
	//    glTranslatef(0,(canon_len)*0.4330127,0.0f);
	glRotatef(theta,0.0f,0.0f,1.0f);
	glColor3f(0.0f,0.0f,1.0f);
	drawline();
	glPopMatrix();*/
	//glPopMatrix();
	glPopMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	score3();
	score4();
	score5();
	glPopMatrix();
	glutSwapBuffers();


}
void score3()
{
	glRasterPos2f(-0.5f,0.0f);
	char game[16]="Welcome to Tank";
	int i;

	for(i=0;i<(int)(strlen(game));i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,game[i]);
	//	for(i=0;i<(int)(strlen(string));i++)
	//		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,string[i]);
	//	char str[80];
	//	sprintf(str,"%d",score);
	//	for(i=0;i<(int)(strlen(str));i++)
	//		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,str[i]);
}
void score4()
{
	glRasterPos2f(-0.7f,-0.5f);
	char game[40]="Click s or S to start the game";
	int i;

	for(i=0;i<(int)(strlen(game));i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,game[i]);
	//	for(i=0;i<(int)(strlen(string));i++)
	//		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,string[i]);
	//	char str[80];
	//	sprintf(str,"%d",score);
	//	for(i=0;i<(int)(strlen(str));i++)
	//		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,str[i]);
}
void score5()
{
	glRasterPos2f(-0.7f,-0.7f);
	char game[40]="Click c or C to see controls";
	int i;

	for(i=0;i<(int)(strlen(game));i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,game[i]);
	//	for(i=0;i<(int)(strlen(string));i++)
	//		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,string[i]);
	//	char str[80];
	//	sprintf(str,"%d",score);
	//	for(i=0;i<(int)(strlen(str));i++)
	//		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,str[i]);
}
void score6()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_BLEND); // Enable the OpenGL Blending functionality 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set the blend mode to blend our current RGBA with what is already in the buffer
	glLoadIdentity();
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -5.0f);
	glColor3f(0.1f, 0.1f, 0.2f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);
	glVertex2f(-2,-2);
	glVertex2f(2, -2);
	glVertex2f(2, 2.0f);
	glVertex2f(-2,2.0f);
	glEnd();
	glPushMatrix();
	glTranslatef(canon_x,3+canon_y-0.15,0.0f);
	glRotatef(theta,0.0f,0.0f,1.0f);
	if(last_key_pressed==67 || last_key_pressed==99)
		glColor3f(0.0f,0.0f,0.0f);
	else
		glColor3f(0.128f,0.128f,0.0f);
	//    drawrealcanon();
	drawCanon();

	glPushMatrix();
	//    glTranslatef(-0.9f,0.0f,0.0f);
	glTranslatef(-(canon_len*1.0)/2.5,0.0f,0.0f);
	glColor3f(0.0f,0.0f,0.0f);
	drawrect();
	glPopMatrix();
	glPushMatrix();
	glTranslatef((canon_len*1.0)/2.5,0.0f,0.0f);
	glColor3f(0.0f,0.0f,0.0f);
	drawrect();
	glPopMatrix();
	//draw canon aim fire line
	/*  glPushMatrix();
	//    glTranslatef(0,(canon_len)*0.4330127,0.0f);
	glRotatef(theta,0.0f,0.0f,1.0f);
	glColor3f(0.0f,0.0f,1.0f);
	drawline();
	glPopMatrix();*/
	//glPopMatrix();
	glPopMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
//	score3();
//	score4();
//	score5();

	
	
	
	
	glRasterPos2f(-0.7f,-0.7f);
	char game[40]="Space to Shoot";
	char game1[40]="C to select Canon";
	char game2[40]="R to select Red Basket";
	char game3[40]="G to select Green Basket";
	char game4[40]="Left Right Arrow keys to move";
	char game5[40]="Up Down Arrow keys to rotate Canon";
	char game6[40]="Click to select";
	int i;
	//printf("%s\n",game);
	for(i=0;i<(int)(strlen(game));i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,game[i]);
	glRasterPos2f(-0.7f,-0.9f);
	for(i=0;i<(int)(strlen(game1));i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,game1[i]);
	glRasterPos2f(-0.7f,-1.1f);
	for(i=0;i<(int)(strlen(game2));i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,game2[i]);
	glRasterPos2f(-0.7f,-1.3f);
	for(i=0;i<(int)(strlen(game3));i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,game3[i]);
	glRasterPos2f(-0.7f,-1.5f);
	for(i=0;i<(int)(strlen(game4));i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,game4[i]);
	glRasterPos2f(-0.7f,-1.7f);
	for(i=0;i<(int)(strlen(game5));i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,game5[i]);
	glRasterPos2f(-0.7f,-1.9f);
	for(i=0;i<(int)(strlen(game6));i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,game6[i]);
	glPopMatrix();
	glutSwapBuffers();
}
// Function to draw objects on the screen
void drawScene() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_BLEND); // Enable the OpenGL Blending functionality 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set the blend mode to blend our current RGBA with what is already in the buffer
	glLoadIdentity();
	glPushMatrix();

	// Draw Box
	glTranslatef(0.0f, 0.0f, -5.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	drawBox(box_len);
	//drawleg(-1);
	drawborder(1);
	//draw spiders
	spider * tmp=list;
	while(tmp!=NULL)
	{
		glPushMatrix();
		glTranslatef(tmp->box_x,tmp->box_y,0.0f);
		if(tmp->flag!=1)
		{
		if(tmp->color==0)
			glColor3f(0.0f,0.0f,0.0f);
		else if(tmp->color==1)
			glColor3f(0.0f,1.0f,0.0f);
		else
			glColor3f(1.0f,0.0f,0.0f);
		//	drawspider(tmp->box_len);
		drawBall((tmp->box_len)/4);
		glPushMatrix();
		if(tmp->color==0)
			glColor3f(0.0f,0.0f,0.0f);
		else if(tmp->color==1)
			glColor3f(0.0f,1.0f,0.0f);
		else
			glColor3f(1.0f,0.0f,0.0f);
		glRotatef(45.0f,0.0f,0.0f,1.0f);
		drawleg(0.45f);
		glPushMatrix();
		glTranslatef(0.0f,0.225 ,0.0f);
		glRotatef(-45.0f,0.0f,0.0f,1.0f);
		drawleg(0.25f);
		glPopMatrix();
		glPopMatrix();
		glPushMatrix();
		if(tmp->color==0)
			glColor3f(0.0f,0.0f,0.0f);
		else if(tmp->color==1)
			glColor3f(0.0f,1.0f,0.0f);
		else
			glColor3f(1.0f,0.0f,0.0f);
		glRotatef(-45.0f,0.0f,0.0f,1.0f);
		drawleg(0.45f);
		glPushMatrix();
		glTranslatef(0.0f,0.225,0.0f);
		glRotatef(45.0f,0.0f,0.0f,1.0f);
		drawleg(0.25f);
		glPopMatrix();
		glPopMatrix();

		glPushMatrix();
		if(tmp->color==0)
			glColor3f(0.0f,0.0f,0.0f);
		else if(tmp->color==1)
			glColor3f(0.0f,1.0f,0.0f);
		else
			glColor3f(1.0f,0.0f,0.0f);
		glRotatef(135.0f,0.0f,0.0f,1.0f);
		drawleg(0.45f);
		glPushMatrix();
		glTranslatef(0.0f,0.225 ,0.0f);
		glRotatef(45.0f,0.0f,0.0f,1.0f);
		drawleg(0.225f);
		glPopMatrix();
		glPopMatrix();
		glPushMatrix();
		if(tmp->color==0)
			glColor3f(0.0f,0.0f,0.0f);
		else if(tmp->color==1)
			glColor3f(0.0f,1.0f,0.0f);
		else
			glColor3f(1.0f,0.0f,0.0f);
		glRotatef(-135.0f,0.0f,0.0f,1.0f);
		drawleg(0.45f);
		glPushMatrix();
		glTranslatef(0.0f,0.225,0.0f);
		glRotatef(-45.0f,0.0f,0.0f,1.0f);
		drawleg(0.225f);
		glPopMatrix();
		glPopMatrix();


		glPushMatrix();
		if(tmp->color==0)
			glColor3f(0.0f,0.0f,0.0f);
		else if(tmp->color==1)
			glColor3f(0.0f,1.0f,0.0f);
		else
			glColor3f(1.0f,0.0f,0.0f);
		glRotatef(90.0f,0.0f,0.0f,1.0f);
		drawleg(0.35f);
		glPushMatrix();
		glTranslatef(0.0f,0.175 ,0.0f);
		glRotatef(45.0f,0.0f,0.0f,1.0f);
		drawleg(0.175f);
		glRotatef(-90.f,0.0f,0.0f,1.0f);
		drawleg(0.175f);
		glPopMatrix();
		glPopMatrix();
		glPushMatrix();
		if(tmp->color==0)
			glColor3f(0.0f,0.0f,0.0f);
		else if(tmp->color==1)
			glColor3f(0.0f,1.0f,0.0f);
		else
			glColor3f(1.0f,0.0f,0.0f);
		glRotatef(-90.0f,0.0f,0.0f,1.0f);
		drawleg(0.35f);
		glPushMatrix();
		glTranslatef(0.0f,0.175,0.0f);
		glRotatef(45.0f,0.0f,0.0f,1.0f);
		drawleg(0.175f);
		glRotatef(-90.0f,0.0f,0.0f,1.0f);
		drawleg(0.175f);
		glPopMatrix();
		glPopMatrix();
		}
		else
		{
			glColor4f(0.147f,0.112f,0.219f,0.8f);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glBegin(GL_QUADS);
			glVertex2f(0,-(tmp->box_len/2));
			glVertex2f((tmp->box_len)/2,0);
			glVertex2f(0,(tmp->box_len/2));
			glVertex2f(-(tmp->box_len/2),0);
			glEnd();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		glPopMatrix();
		tmp=tmp->next;
	}
	//Draw Lasers
	laser *tmp2=bullet;
	float a,b,c,initial_x,initial_y,final_x,final_y,dist,remain;
	while(tmp2!=NULL)
	{
		glPushMatrix();
		a=tmp2->laser_x-(canon_len*0.4330127)*sin(DEG2RAD(tmp2->angle));
		b=cos(DEG2RAD(tmp2->angle));
		c=tmp2->laser_y+(canon_len*0.4330127)*b;
		remain=canon_len*0.4330127;
		if(a>(box_len/2))
		{
			initial_x=box_len/2;
			//			printf("initial: %f %f %f\n",tmp2->laser_x,tmp2->laser_y,tmp2->angle);
			if(sin(DEG2RAD(tmp2->angle))!=0)
			{
				dist=(initial_x-tmp2->laser_x)/sin(DEG2RAD(tmp2->angle));
				//				printf("%f\n",dist);
				if(dist<0)
					dist=-dist;
				initial_y=tmp2->laser_y+dist*b;
				remain=(canon_len*0.4330127)-dist;
				glPushMatrix();
				glTranslatef(tmp2->laser_x,tmp2->laser_y,0);
				glRotatef(tmp2->angle,0.0f,0.0f,1.0f);
				glColor3f(1.0f,0.0f,0.0f);
				drawleg(2*dist);
				glPopMatrix();
				tmp2->laser_x=initial_x;
				tmp2->laser_y=initial_y;
				tmp2->angle=-tmp2->angle;
			}
			//			printf("final: %f %f %f\n",tmp2->laser_x,tmp2->laser_y,tmp2->angle);
		}
		else if(a<(-box_len/2))
		{
			initial_x=-box_len/2;
			if(sin(DEG2RAD(tmp2->angle))!=0)
			{
				dist=(initial_x-tmp2->laser_x)/sin(DEG2RAD(tmp2->angle));
				if(dist<0)
					dist=-dist;
				initial_y=tmp2->laser_y+dist*b;
				remain=(canon_len*0.4330127)-dist;
				glPushMatrix();
				glTranslatef(tmp2->laser_x,tmp2->laser_y,0);
				glRotatef(tmp2->angle,0.0f,0.0f,1.0f);
				glColor3f(1.0f,0.0f,0.0f);
				drawleg(2*dist);
				glPopMatrix();
				tmp2->laser_x=initial_x;
				tmp2->laser_y=initial_y;
				tmp2->angle=-tmp2->angle;
			}
		}
		if(c>(box_len/2) || c<(-box_len/2))
		{
			if(c>box_len/2)
				initial_y=box_len/2;
			else
				initial_y=-box_len/2;
			if(b!=0)
			{
				dist=(initial_y-tmp2->laser_y)/b;
				if(dist<0)
					dist=-dist;
				initial_x=tmp2->laser_x-dist*sin(DEG2RAD(tmp2->angle));
				remain=(canon_len*0.4330127)-dist;
				glPushMatrix();
				glTranslatef(tmp2->laser_x,tmp2->laser_y,0);
				glRotatef(tmp2->angle,0.0f,0.0f,1.0f);
				glColor3f(1.0f,0.0f,0.0f);
				drawleg(dist);
				glPopMatrix();
				tmp2->laser_x=initial_x;
				tmp2->laser_y=initial_y;
				tmp2->angle=180-tmp2->angle;
			}
		}
		glTranslatef(tmp2->laser_x,tmp2->laser_y,0);
		glRotatef(tmp2->angle,0.0f,0.0f,1.0f);
		glColor3f(1.0f,0.0f,0.0f);
		drawleg(2*remain);
		glPopMatrix();
		tmp2=tmp2->next;
	}
	//Draw Red Basket
	glPushMatrix();
	glTranslatef((red_basket_x+((red_basket_lenx)/2)),red_basket_y+(red_basket_leny/2),0.0f);
	if(last_key_pressed==82 || last_key_pressed==114)
		glColor3f(1.0f,0.0f,0.0f);
	else
	{
		//	glEnable(GL_BLEND);
		//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1.0f,0.0f,0.0f,0.8f);
		//		glColor3f(0.128f,0.128f,0.0f);
		//	glDisable(GL_BLEND);
	}
	drawredbox();
	glPopMatrix();
	//Draw Green Basket
	glPushMatrix();
	glTranslatef((green_basket_x+((green_basket_lenx)/2)),green_basket_y+(green_basket_leny/2),0.0f);
	if(last_key_pressed==71 || last_key_pressed==103)
		glColor3f(0.0f,1.0f,0.0f);
	else
		glColor4f(0.128f,0.128f,0.0f,0.8f);
	drawgreenbox();
	glPopMatrix();
	// Draw Ball
	/*    glPushMatrix();
	      glTranslatef(ball_x, ball_y, 0.0f);
	      glColor3f(0.0f, 1.0f, 0.0f);
	      drawBall(ball_rad);
	      glPopMatrix();*/

	// Draw Triangle
	//    glPushMatrix();
	//   glTranslatef(tri_x, tri_y, 0.0f);
	//   glRotatef(theta, 0.0f, 0.0f, 1.0f);
	//   glScalef(0.5f, 0.5f, 0.5f);
	//   drawTriangle();
	// glPopMatrix();

	// Draw Canon
	glPushMatrix();
	glTranslatef(canon_x,canon_y-0.15,0.0f);
	glRotatef(theta,0.0f,0.0f,1.0f);
	if(last_key_pressed==67 || last_key_pressed==99)
		glColor3f(0.0f,0.0f,0.0f);
	else
		glColor3f(0.128f,0.128f,0.0f);
	//    drawrealcanon();
	drawCanon();

	glPushMatrix();
	//    glTranslatef(-0.9f,0.0f,0.0f);
	glTranslatef(-(canon_len*1.0)/2.5,0.0f,0.0f);
	glColor3f(0.0f,0.0f,0.0f);
	drawrect();
	glPopMatrix();
	glPushMatrix();
	glTranslatef((canon_len*1.0)/2.5,0.0f,0.0f);
	glColor3f(0.0f,0.0f,0.0f);
	drawrect();
	glPopMatrix();
	//draw canon aim fire line
	/*  glPushMatrix();
	//    glTranslatef(0,(canon_len)*0.4330127,0.0f);
	glRotatef(theta,0.0f,0.0f,1.0f);
	glColor3f(0.0f,0.0f,1.0f);
	drawline();
	glPopMatrix();*/
	//glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	show_score();
	glPopMatrix();
	glPopMatrix();
	glutSwapBuffers();
}
void show_score()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
	glVertex2f(2.3-0.2,1.25-0.25);
	glVertex2f(2.3+0.7,1.25-0.25f);
	glVertex2f(2.3+0.7,1.25+0.5);
	glVertex2f(2.3-0.2, 1.25+0.5);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	char sh[1000],s[1000];
	int l,i;
	sprintf(sh,"%d",scores);
	l=strlen(sh);
	//	printf("%d\n",scores);
	glColor3f(1.0, 0.0, 0.0);
	glRasterPos2f(2.5, 1.25);
	//	string s = "HERE I AM";
	void * font = GLUT_BITMAP_9_BY_15;
	for (i =0; i<l; i++)
	{
		char c =sh[i];
		glutBitmapCharacter(font, c);
	}
	sprintf(s,"%s","SCORE");
	l=strlen(s);
	//printf("%s\n",s);
	glColor3f(1.0, 0.0, 0.0);
	glRasterPos2f(2.3, 1.4);
	void * font1 = GLUT_BITMAP_9_BY_15;
	for (i =0; i<l; i++)
	{
		char c =s[i];
		glutBitmapCharacter(font1, c);
	}
}
void drawrect()
{
	//	printf("$\n");
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);
	glVertex2f(-(canon_len)/18,-(canon_len/12)-0.10f);
	glVertex2f((canon_len)/18,-(canon_len/12)-0.10f);
	glVertex2f((canon_len/18),(canon_len/12)+0.10f);
	glVertex2f(-(canon_len/18), ((canon_len)/12)+0.10f);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
void drawleg(float len)
{
	glLineWidth(2.5);
	//	 glColor3f(0.0, 0.0, 1.0); 
	glBegin(GL_LINES); 
	glVertex3f(0.0f, 0.0f,0.0f); 
	glVertex3f(0.0f,len/2 ,0.0f);
	glEnd();
}
void drawborder(float len)
{
	glLineWidth(2.5);
	glColor3f(0.0, 2.0, 2.0); 
	glBegin(GL_LINES); 
	glVertex3f(-2.0f,-1.3f,0.0f); 
	glVertex3f(2.0f,-1.3f ,0.0f);
	glEnd();
}
//drawing spider
void drawspider(float len)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
	glVertex2f(-len / 2, -len / 2);
	glVertex2f(len / 2, -len / 2);
	glVertex2f(len / 2, len / 2);
	glVertex2f(-len / 2, len / 2);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
void change(int value)
{
	if(pausing!=1)
	{
		//		printf("$$$$$$$$$$$$$$$$$$$$$$\n");
		spider *tmp=list;
		while(tmp!=NULL)
		{
			if(tmp->box_y-0.1>=(-box_len/2))
			{
				//printf("%f\n",tmp->box_vely);
				tmp->box_y-=tmp->box_vely;
				tmp->box_vely+=0.0000001;
			}
			tmp=tmp->next;
		}
		//		printf("$$$$$$$$$$$$$$$$$$$$$$\n");
	}
	glutTimerFunc(10, change, 0);
}
void over(int value)
{
	//	printf("%d\n",scores);
	if(pausing!=1 && overflag!=1)
	{
		spider *tmp=list;
		while(tmp!=NULL)
		{
			if((tmp->box_x>=(canon_x-(canon_len/2)))&&(tmp->box_x<=(canon_x+(canon_len/2)))&&(tmp->box_y<=(canon_y+(canon_len/4)))&&(tmp->box_y-0.1>=(-box_len/2)))
			{
				//printf("%f %f %f %f %f\n",tmp->box_x,(canon_x-canon_len/4),canon_x+(canon_len/4),tmp->box_y,canon_y+(canon_len/4));
				///		printf("OVER\n");
				if(tmp->flag==1)
				{
					while(list!=NULL)
					{
						tmp=list->next;
						free(list);
						list=tmp;
					}
					pid_t pid;
					pid=fork();
					if(pid==0)
					{
						system("aplay Bomb.wav");
						//system("play explosion_01.wav");
						//system("aplay GAME_OVER.wav");
						exit(EXIT_SUCCESS);
						//				return 0;
						exit(0);
					}
					break;
				}
				else
				{
				overflag=1;
				pid_t pid;
				pid=fork();
				if(pid==0)
				{
					//system("play explosion_01.wav");
					system("aplay GAME_OVER.wav");
					exit(EXIT_SUCCESS);
					//				return 0;
					exit(0);
				}
				glutDisplayFunc(drawScene2);
				glutIdleFunc(drawScene2);
				break;
				}
			}
			if(tmp!=NULL)
				tmp=tmp->next;
		}
	}
	glutTimerFunc(10,over,0);
}
void incontainer(int value)
{
	if(pausing!=1 && overflag!=1)
	{
		spider *tmp=list,*prev=NULL;
		while(tmp!=NULL)
		{
			if((tmp->box_x>=red_basket_x+((tmp->box_len)/4))&&(tmp->box_x<=red_basket_x+red_basket_lenx-((tmp->box_len)/4))&&(tmp->box_y-0.1<=red_basket_y+red_basket_lenx)&&(tmp->box_y+0.1>=red_basket_y+red_basket_lenx))
			{
				if(tmp->color==2)
					scores++;
				else
					scores--;
				//printf("%f %f %f %f %f %f %f\n",tmp->box_x,tmp->box_y,-(box_len/2),red_basket_x,red_basket_x+red_basket_lenx,red_basket_y,red_basket_y+red_basket_lenx);
				if(prev==NULL)
				{
					prev=tmp;
					list=list->next;
					free(prev);
					tmp=list;
					prev=NULL;
				}
				else
				{
					prev->next=tmp->next;
					free(tmp);
					tmp=prev->next;
				}
			}
			else if((tmp->box_x>=green_basket_x+((tmp->box_len)/4))&&(tmp->box_x<=green_basket_x+green_basket_lenx-((tmp->box_len)/4))&&(tmp->box_y-0.1<=green_basket_y+green_basket_lenx)&&(tmp->box_y+0.1>=green_basket_y+green_basket_lenx))
			{
				if(tmp->color==1)
					scores++;
				else
					scores--;
				//printf("%f %f %f %f %f %f %f\n",tmp->box_x,tmp->box_y,-(box_len/2),green_basket_x,green_basket_x+green_basket_lenx,green_basket_y,green_basket_y+green_basket_lenx);
				if(prev==NULL)
				{
					prev=tmp;
					list=list->next;
					free(prev);
					tmp=list;
					prev=NULL;
				}
				else
				{
					prev->next=tmp->next;
					free(tmp);
					tmp=prev->next;
				}
			}
			prev=tmp;
			if(tmp!=NULL)
				tmp=tmp->next;
		}
	}
	glutTimerFunc(10, incontainer, 0);
}
// Function to handle all calculations in the scene
// updated evry 10 milliseconds
void update(int value) {
	if(pausing!=1 && overflag!=1)
	{
		// Handle ball collisions with box
		if(ball_x + ball_rad > box_len/2 || ball_x - ball_rad < -box_len/2)
			ball_velx *= -1;
		if(ball_y + ball_rad > box_len/2 || ball_y - ball_rad < -box_len/2)
			ball_vely *= -1;

		//    laser *tmp=bullet;
		//    while(tmp!=NULL)
		//  {
		//	    tmp->laser_x=tmp->laser_x-(canon_len*0.4330127)*cos(tmp->angle);
		//	    tmp->laser_y=tmp->laser_y-(canon_len*0.4330127)*cos(tmp->angle);
		//	    tmp=tmp->next;
		//  }
		// Update position of ball
		ball_x += ball_velx;
		ball_y += ball_vely;
		timing+=10;
	}
	glutTimerFunc(10, update, 0);
}
void remove(int value)
{
	if(pausing!=1 && overflag!=1)
	{
		laser *tmp2=bullet,*prev=NULL ,*check;
		float dist,dist2,p,bullet_x,bullet_y,laser_x,laser_y;
		spider *tmp3=list,*prev1=NULL;
		while(tmp3!=NULL)
		{
			tmp2=bullet;
			prev=NULL;
			bullet_x=tmp3->box_x;
			bullet_y=tmp3->box_y;
			while(tmp2!=NULL && tmp3!=NULL)
			{
				laser_x=tmp2->laser_x;
				laser_y=tmp2->laser_y;
				dist=((bullet_x)-(laser_x))*((bullet_x)-(laser_x))+((bullet_y)-(laser_y))*((bullet_y)-(laser_y));
				if((dist<=(tmp3->box_len/4)*(tmp3->box_len/4)+0.015)&&(tmp3->box_y-0.1>=(-box_len/2)))
				{
					pid_t pid;
					pid=fork();
					if(pid==0)
					{
						system("aplay Bomb.wav");
						exit(EXIT_SUCCESS);
						//				return 0;
						exit(0);
					}
					if(tmp3->flag==1)
						scores+=20;
					else
					{
					if(tmp3->color==0)
						scores+=5;
					if(tmp3->color==1)
						scores++;
					if(tmp3->color==2)
						scores++;
					}
					//			    printf("^\n");
					//printf("%f %f  %f %f  %f %f\n",bullet_x,bullet_y,laser_x,laser_y,dist,(tmp3->box_len/4)*(tmp3->box_len/4));
					if(prev==NULL)
					{
						//				    printf("%d\n",)
						//printf("1\n");
						prev=tmp2;
						bullet=bullet->next;
						free(prev);
						tmp2=bullet;
						prev=NULL;
					}
					else
					{
						//						printf("2\n");
						prev->next=tmp2->next;
						free(tmp2);
						tmp2=prev->next;
					}
					if(prev1==NULL)
					{
						//						printf("3\n");
						prev1=tmp3;
						//						if(list->next!=NULL)
						//							printf("tmp3: %f %fnext: %f %f\n",prev1->box_x,prev1->box_y,(list->next)->box_x,(list->next)->box_y);
						list=list->next;
						free(prev1);
						tmp3=list;
						prev1=NULL;
					}
					else
					{
						//						printf("4\n");
						(prev1->next)=(tmp3->next);
						//						if(tmp3->next!=NULL)
						//							printf("Prev1: %f %f tmp3: %f %fnext: %f %f\n",prev1->box_x,prev1->box_y,tmp3->box_x,tmp3->box_y,(tmp3->next)->box_x,(tmp3->next)->box_y);
						//						if(tmp3!=NULL)
						//							printf("initial :%f %f\n",tmp3->box_x,tmp3->box_y);
						free(tmp3);
						tmp3=prev1->next;
						//						if(tmp3!=NULL)
						//							printf("Final :%f %f\n",tmp3->box_x,tmp3->box_y);
					}
				}
				prev=tmp2;
				if(tmp2!=NULL)
					tmp2=tmp2->next;
			}
			prev1=tmp3;
			if(tmp3!=NULL)
				tmp3=tmp3->next;
		}
	}
	glutTimerFunc(1,remove, 0);
}
void final(int value)
{
	if(pausing!=1 && overflag!=1)
	{
		laser *tmp=bullet;
		float p;
		while(tmp!=NULL)
		{
			//	    if(tmp)
			// tmp->laser_x=tmp->laser_x-(canon_len*0.4330127)*sin(DEG2RAD(tmp->angle));
			// p=cos(DEG2RAD(tmp->angle));
			// tmp->laser_y=tmp->laser_y+(canon_len*0.4330127)*p;
			tmp->laser_x=tmp->laser_x-(0.1330127)*sin(DEG2RAD(tmp->angle));
			p=cos(DEG2RAD(tmp->angle));
			tmp->laser_y=tmp->laser_y+(0.1330127)*p;
			tmp=tmp->next;
		}
	}
	glutTimerFunc(50,final, 0);
}
void drawBox(float len) {

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
	glVertex2f(-len / 2,( -len / 2)-1.0f);
	glVertex2f(len / 2, (-len / 2)-1.0f);
	glVertex2f(len / 2, (len / 2)+1.0f);
	glVertex2f(-len / 2, (len / 2)+1.0f);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void drawredbox()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
	glVertex2f((-((red_basket_lenx)/2) +0.3),-(((red_basket_leny)/2)));
	glVertex2f(((red_basket_lenx)/2 -0.3),-((red_basket_leny)/2));
	glVertex2f(((red_basket_lenx)/2),((red_basket_leny)/2));
	glVertex2f(-((red_basket_lenx)/2),((red_basket_leny)/2));
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
void drawgreenbox()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
	glVertex2f(-(((green_basket_lenx)/2)-0.2),-((green_basket_leny)/2));
	glVertex2f((((green_basket_lenx)/2)-0.2),-((green_basket_leny)/2));
	glVertex2f(((green_basket_lenx)/2),((green_basket_leny)/2));
	glVertex2f(-((green_basket_lenx)/2),((green_basket_leny)/2));
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
void drawBall(float rad) {

	glBegin(GL_TRIANGLE_FAN);
	for(int i=0 ; i<360 ; i++) {
		glVertex2f(rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
	}
	glEnd();
}

void drawTriangle() {

	glBegin(GL_TRIANGLES);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glEnd();
}

//draw Canon
void drawCanon(){

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
	glVertex2f(-(((canon_len)/4)),-((canon_len)/4));
	glVertex2f((((canon_len)/4)),-((canon_len)/4));
	glVertex2f(((canon_len)/4),((canon_len)/4));
	glVertex2f(-((canon_len)/4),((canon_len)/4));
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
	glVertex2f(-(((canon_len)/16)),0);
	glVertex2f((((canon_len)/16)),0);
	glVertex2f(((canon_len)/16),0.30+((canon_len)/4));
	glVertex2f(-((canon_len)/16),0.30+((canon_len)/4));
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLineWidth(2.5);
	glColor3f(0.0f,0.0f,0.0f);
	glBegin(GL_LINES); 
	glVertex3f(0.0f, 0.0f,0.0f); 
	glVertex3f(((-canon_len*1.0)/2.5),0.0f,0.0f);
	glEnd();
	glLineWidth(2.5);
	glColor3f(0.0f,0.0f,0.0f);
	glBegin(GL_LINES); 
	glVertex3f(0.0f, 0.0f,0.0f); 
	glVertex3f(((canon_len*1.0)/2.5),0.0f,0.0f);
	glEnd();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//	glBegin(GL_QUADS);
	//	glVertex2f(-(((canon_len))),0);
	//	glVertex2f((((canon_len))),0);
	//	glVertex2f(((canon_len)),2.30+((canon_len)));
	//	glVertex2f(-((canon_len)),2.30+((canon_len)));
	//	glEnd();
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//	glPushMatrix();
	//	glTranslatef(((canon_len*1.0)/2.5),0.0f,0.0f);
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//	glBegin(GL_QUADS);
	//	glVertex2f(-(((canon_len))),0);
	//	glVertex2f((((canon_len))),0);
	//	glVertex2f(((canon_len)),0.30+((canon_len)));
	//	glVertex2f(-((canon_len)),0.30+((canon_len)));
	//	glEnd();
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//	glPopMatrix();

	//	glEnd();
	//	glBegin(GL_TRIANGLES);
	//	glVertex3f((-canon_len/2),-(canon_len)*0.4330127,0.0f);
	//	glVertex3f((canon_len/2),-(canon_len)*0.4330127,0.0f);
	//	glVertex3f((0.0f),(canon_len)*0.4330127,0.0f);
	//  	glEnd();
}

void drawline()
{
	glLineWidth(2.5);
	glColor3f(0.0, 0.0, 1.0); 
	glBegin(GL_LINES); 
	glVertex3f(0.0f, 0.0f,0.0f); 
	glVertex3f(0.0f,line_length ,0.0f);
	glEnd();
}
// Initializing some openGL 3D rendering options
void initRendering() {

	glEnable(GL_DEPTH_TEST);        // Enable objects to be drawn ahead/behind one another
	glEnable(GL_COLOR_MATERIAL);    // Enable coloring
	//	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);   // Setting a background color
	glClearColor(0.196078f, 0.60f, 0.80f, 0.1f);   // Setting a background color
}

// Function called when the window is resized
void handleResize(int w, int h) {

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)w / (float)h, 0.1f, 200.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void handleKeypress1(unsigned char key, int x, int y) {

	if (key == 27 || key==81 ||key==113) {
		killpg(getpgid(0),9);
		exit(0);     // escape key is pressed
	}
	if(key!=32)
		last_key_pressed=key;
	if(key==80 ||key==112)
	{
		//printf("POaused\n");
		pausing=pausing^1;
		system("aplay pause.wav &");
	}
	if((key==67||key==99) &&(start==0))
	{
			control=control^1;
			if(control==1)
			{
				//printf("YELLS\n");
				glutDisplayFunc(score6);
				glutIdleFunc(score6);
			}
			else
			{
				glutDisplayFunc(startscene);
				glutIdleFunc(startscene);
		//		glutDisplayFunc(score6);
		//		glutIdleFunc(score6);
			}
	}
	if(key==83 || key==115)
	{
		//printf("ENTERED\n");
		start=1;
		pausing=0;
		glutDisplayFunc(drawScene);
		glutIdleFunc(drawScene);
	}
	if(key==32 && overflag!=1)
	{
		if(timing-prev>=1000)
		{
			prev=0;
			timing=0;
			if(bullet==NULL)
			{
				//		    printf("lol1\n");
				bullet=(laser *)malloc(sizeof(laser));
				bullet->angle=theta;
				bullet->laser_x=canon_x;
				bullet->laser_y=canon_y-0.15;
				pid_t pid;
				pid=fork();
				if(pid==0)
				{
					//		system("play explosion_01.wav");
					system("aplay LASER.WAV");
					exit(EXIT_SUCCESS);
					//				return 0;
					exit(0);
				}
				//    bullet->laser_x=(canon_x)-(canon_len)*0.4330127*sin(theta);
				//  bullet->laser_y=(canon_y+(canon_len*0.4330127))+(canon_len)*0.4330127*cos(theta);
				//printf("last:  %f %f %f %f\n",canon_x,canon_y,canon_len,cos(theta));
				//printf("%f %f %f\n",bullet->laser_x,bullet->laser_y,theta);
				bullet->next=NULL;
			}
			else
			{
				//		    printf("lol2\n");
				laser * tmp=bullet;
				while(tmp->next!=NULL)
				{
					tmp=tmp->next;
				}
				tmp->next=(laser *)malloc(sizeof(laser));
				laser * tmp1;
				tmp1=tmp->next;
				tmp1->angle=theta;
				//		    printf("last:  %f %f %f\n",canon_x,canon_y,canon_len,cos(theta));
				tmp1->laser_x=canon_x;
				tmp1->laser_y=canon_y-0.15;
				pid_t pid;
				pid=fork();
				if(pid==0)
				{
					//system("play explosion_01.wav");
					system("aplay LASER.WAV");
					exit(EXIT_SUCCESS);
					//				return 0;
					exit(0);
				}
				//tmp1->laser_x=(canon_x)-(canon_len)*0.4330127*sin(theta);
				//tmp1->laser_y=(canon_y+(canon_len*0.4330127))+(canon_len)*0.4330127*cos(theta);
				//		    tmp1->laser_x=canon_x-(canon_len)*0.4330127*cos(theta);
				//		    tmp1->laser_y=canon_y-(canon_len)*0.4330127*cos(theta);
				//		    printf("%f %f %f\n",tmp1->laser_x,tmp1->laser_y,theta);
				tmp1->next=NULL;
				tmp->next=tmp1;
				//			printf("%f\n",p);
			}
		}
	}
	//    printf("%d\n",last_key_pressed);
	//    last_key
}

void handleKeypress2(int key, int x, int y) {
	//    if (key == GLUT_KEY_LEFT)
	//        tri_x -= 0.1;
	//    if (key == GLUT_KEY_RIGHT)
	//        tri_x += 0.1;
	//    if (key == GLUT_KEY_UP)
	//        tri_y += 0.1;
	//    if (key == GLUT_KEY_DOWN)
	//        tri_y -= 0.1;
	spider *tmp=list;
	float canon_lower=-box_len/2,canon_upper=box_len/2,red_lower=-box_len/2,red_upper=box_len/2,green_lower=-box_len/2,green_upper=box_len/2;
	while(tmp!=NULL)
	{
		if(tmp->box_y-0.1<(-box_len/2))
		{
			if(tmp->box_x<=canon_x && tmp->box_x>canon_lower)
			{
				canon_lower=tmp->box_x;
			}
			if(tmp->box_x>=canon_x && tmp->box_x<canon_upper)
			{
				canon_upper=tmp->box_x;
			}
			if(tmp->box_x<=red_basket_x && tmp->box_x>red_lower)
			{
				red_lower=tmp->box_x;
			}
			if(tmp->box_x>=(red_basket_x) && tmp->box_x<red_upper)
			{
				red_upper=tmp->box_x;
			}
			if(tmp->box_x<=green_basket_x && tmp->box_x>green_lower)
			{
				green_lower=tmp->box_x;
			}
			if(tmp->box_x>=(green_basket_x) && tmp->box_x<green_upper)
			{
				green_upper=tmp->box_x;
			}
		}
		tmp=tmp->next;
	}
	//printf("%f %f %f %f %f %f\n",canon_lower,canon_upper,red_lower,red_upper,green_lower,green_upper);
	if(last_key_pressed==82 || last_key_pressed==114)
	{
		if(key==GLUT_KEY_LEFT) //R is pressed
		{
			if((red_basket_x-0.1)>red_lower)
				red_basket_x-=0.1;
		}
		if(key==GLUT_KEY_RIGHT)
		{
			if((red_basket_x+red_basket_lenx)<=red_upper)
			{
				red_basket_x+=0.1;
			}
		}
		if(key==GLUT_KEY_UP)
		{

		}
		if(key==GLUT_KEY_DOWN)
		{
		}
	}
//	printf("%d\n",last_key_pressed);
	if(last_key_pressed==71 || last_key_pressed==103)
	{
		if(key==GLUT_KEY_LEFT) //G is pressed
		{
			if((green_basket_x-0.1)>green_lower)
				green_basket_x-=0.1;
		}
		if(key==GLUT_KEY_RIGHT)
		{
			if((green_basket_x+green_basket_lenx)<(green_upper))
			{
				green_basket_x+=0.1;
			}
		}
		if(key==GLUT_KEY_UP)
		{

		}
		if(key==GLUT_KEY_DOWN)
		{
		}
	}
	if(last_key_pressed==67 || last_key_pressed==99)
	{
		if(start==1)
		{
		if(key==GLUT_KEY_LEFT)
		{
			if((canon_x)-(canon_len/2)>=canon_lower)
				canon_x-=0.1;
		}
		if(key==GLUT_KEY_RIGHT)
		{
			if(canon_x+(canon_len/2)<=(canon_upper))
				canon_x+=0.1;
		}
		if(key==GLUT_KEY_UP)
		{
			if(theta<=45.0f)
				theta += 15;
		}
		if(key==GLUT_KEY_DOWN)
		{
			if(theta>=(-45.0f))
				theta -= 15;
		}
		}
		else
		{
			control=control^1;
		}
	}
}

void handleMouseclick(int button, int state, int x, int y) {

	if (state == GLUT_DOWN)
	{
		if (button == GLUT_LEFT_BUTTON)
		{
			drag1=1;
			drag2=0;
			//			if(theta<=45.0f)
			//				theta += 15;
		}
		else if (button == GLUT_RIGHT_BUTTON)
		{
			drag2=1;
			drag1=0;
			GetOGLPos(x, y);
			if((mouse_x>=(canon_x-(canon_len/2)))&&(mouse_x<=(canon_x+(canon_len/2)))&&(mouse_y<=(canon_y+(canon_len/4))))
			{
				last_key_pressed=67;
				//				printf("Yes %d\n",last_key_pressed);
			}
			if(mouse_x>=red_basket_x && mouse_x<=red_basket_x+red_basket_lenx && mouse_y>=red_basket_y && mouse_y<=red_basket_y+red_basket_leny)
			{
				last_key_pressed=82;
			}
			if(mouse_x>=green_basket_x && mouse_x<=green_basket_x+green_basket_lenx && mouse_y>=green_basket_y && mouse_y<=green_basket_y+green_basket_leny)
			{
				last_key_pressed=71;
			}
			//			if()
			//		printf("Entered\n");
			//	if(theta>=(-45.0f))
			//		theta -= 15;
		}
		//	printf("%f\n",theta);
	}
//float mouseposx;
}
