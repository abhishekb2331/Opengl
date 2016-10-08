//	This is a sample program that illustrates OpenGL and GLUT in 3D. It
//	renders a picture of 36 snowmen. The camera can be moved by dragging
//	the mouse. The camera moves forward by hitting the up-arrow key and
//	back by moving the down-arrow key. Hit ESC, 'q' or 'Q' to exit.
//
//	Warning #1: This program uses the function glutSpecialUpFunc, which
//	may not be available in all GLUT implementations. If your system
//	does not have it, you can comment this line out, but the up arrow
//	processing will not be correct.
//----------------------------------------------------------------------
#include <stdlib.h> 
#include <math.h> 
#include <stdio.h> 
#include <GL/freeglut.h>
float coinangle=0;
// The following works for both linux and MacOS
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#define max(a,b) a>b ? a:b
#include <math.h>
#include "imageloader.h"
#include "vec3f.h"
#define minimum(a,b) a>b ? b:a
float place_x[60],prev_angle;
float place_y[60];
int scores;
float height1,height2,rot_angle=0,acc;
int initial=1,times=0,hj=0;
float roll_angle=0.0;
int roll_flag=0,jump,sideview=0,used[60];
float prev_ht=0,cur_ht=0,h1=0.0,h2,h3;
#define WIN_WIDTH      600 
#define WIN_HEIGHT      600    
#define CYCLE_LENGTH   3.3f 
#define ROD_RADIUS      0.05f 
#define NUM_SPOKES      20 
#define SPOKE_ANGLE      18 
#define RADIUS_WHEEL   1.0f 
#define TUBE_WIDTH      0.08f 
#define RIGHT_ROD      1.6f 
#define RIGHT_ANGLE      48.0f 
#define MIDDLE_ROD      1.7f 
#define MIDDLE_ANGLE   106.0f 
#define BACK_CONNECTOR   0.5f 
#define LEFT_ANGLE      50.0f 
#define WHEEL_OFFSET   0.11f 
#define WHEEL_LEN      1.1f 
#define TOP_LEN         1.5f 
#define CRANK_ROD      0.7f 
#define CRANK_RODS      1.12f 
#define CRANK_ANGLE      8.0f 
#define HANDLE_ROD      1.2f 
#define FRONT_INCLINE   70.0f 
#define HANDLE_LIMIT   70.0f 

#define INC_STEERING   2.0f 
#define INC_SPEED      0.05f 

// escape key (for exit)

//#define max(a,b) (a>b ? (a):(b))
#define ESC 27
#define PI 3.141592653589
#define DEG2RAD(deg) (deg * PI / 180)
float vx,vy,dist,fl=0,prevang=0;

GLfloat pedalAngle, speed, steering;
GLfloat camx,camy,camz; 
GLfloat anglex,angley,anglez; 
int prevx,prevy; 
float gangle=0,gangle2=0.0f,gangle3=0.0f,fangle=0.0f;
GLenum Mouse; 
GLfloat xpos,zpos,direction; 
float pos_x=0.0,pos_y=-5.0;

void ZCylinder(GLfloat radius,GLfloat length); 
void XCylinder(GLfloat radius,GLfloat length); 

void drawFrame(void); 
void gear( GLfloat inner_radius, GLfloat outer_radius, 
		GLfloat width,GLint teeth, GLfloat tooth_depth ); 
void drawChain(void); 
void drawPedals(void); 
void drawTyre(void); 
void drawSeat(void); 
void display(void); 
void idle(void); 
void updateScene(void); 
void landmarks(void);    
GLfloat Abs(GLfloat); 
GLfloat degrees(GLfloat); 
GLfloat radians(GLfloat); 
GLfloat angleSum(GLfloat, GLfloat); 

float max(float a,float b)
{
	if(a>b)
		return a;
	return b;
}
//----------------------------------------------------------------------
// Global variables
//
// The coordinate system is set up so that the (x,y)-coordinate plane
// is the ground, and the z-axis is directed upwards. The y-axis points
// to the north and the x-axis points to the east.
//
// The values (x,y) are the current camera position. The values (lx, ly)
// point in the direction the camera is looking. The variables angle and
// deltaAngle control the camera's angle. The variable deltaMove
// indicates the amount of incremental motion for the camera with each
// redraw cycle. The variables isDragging and xDragStart are used to
// monitor the mouse when it drags (with the left button down).
//----------------------------------------------------------------------

// Camera position
float x = -30.0, y = -30.0; // initially 5 units south of origin
float deltaMove = 0.0; // initially camera doesn't move

// Camera direction
float lx = 0.0, ly = 1.0; // camera points initially along y-axis
float angle = 0.0; // angle of rotation for the camera direction
float deltaAngle = 0.0; // additional angle change when dragging

// Mouse drag control
int isDragging = 0; // true when dragging
int xDragStart = 0; // records the x-coordinate when dragging starts

int leftflag=0;
int rightflag=0;
int upview=0;
int wheelview=0;
int userview=0;
int heliview=0;
float mouse_x,mouse_y;


//----------------------------------------------------------------------
// Reshape callback
//
// Window size has been set/changed to w by h pixels. Set the camera
// perspective to 45 degree vertical field of view, a window aspect
// ratio of w/h, a near clipping plane at depth 1, and a far clipping
// plane at depth 100. The viewport is the entire window.
//
//----------------------------------------------------------------------

void ZCylinder(GLfloat radius,GLfloat length) 
{ 
	GLUquadricObj *cylinder; 
	cylinder=gluNewQuadric(); 
	glPushMatrix(); 
	glTranslatef(0.0f,0.0f,0.0f); 
	gluCylinder(cylinder,radius,radius,length,15,5); 
	glPopMatrix(); 
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
	printf("%f %f\n",mouse_x,mouse_y);
	//gc=posZ*100;
}
void handleMouseclick(int button, int state, int x, int y) {
	GetOGLPos(x,y);
}
void XCylinder(GLfloat radius,GLfloat length) 
{ 
	glPushMatrix(); 
	glRotatef(90.0f,0.0f,1.0f,0.0f); 
	ZCylinder(radius,length); 
	glPopMatrix(); 
} 

// called by idle() 
void updateScene() 
{ 
	GLfloat xDelta, zDelta; 
	GLfloat rotation; 
	GLfloat sin_steering, cos_steering; 

	// if the tricycle is not moving then do nothing 
	if (-INC_SPEED < speed && speed < INC_SPEED) return; 

	if(speed < 0.0f) 
		pedalAngle = speed = 0.0f; 

	// otherwise, calculate the new position of the tricycle 
	// and the amount that each wheel has rotated. 
	// The tricycle has moved "speed*(time elapsed)". 
	// We assume that "(time elapsed)=1". 

	xDelta = speed*cos(radians(direction + steering)); 
	zDelta = speed*sin(radians(direction + steering)); 
	xpos += xDelta; 
	zpos -= zDelta; 
	pedalAngle = degrees(angleSum(radians(pedalAngle), speed/RADIUS_WHEEL)); 

	// we'll be using sin(steering) and cos(steering) more than once 
	// so calculate the values one time for efficiency 
	sin_steering = sin(radians(steering)); 
	cos_steering = cos(radians(steering)); 

	// see the assignment 3 "Hint" 
	rotation = atan2(speed * sin_steering, CYCLE_LENGTH + speed * cos_steering); 
	direction = degrees(angleSum(radians(direction),rotation)); 
} 

// angleSum(a,b) = (a+b) MOD 2*PI 
// a and b are two angles (radians) 
//  both between 0 and 2*PI 
GLfloat angleSum(GLfloat a, GLfloat b) 
{ 
	a += b; 
	if (a < 0) return a+2*PI; 
	else if (a > 2*PI) return a-2*PI; 
	else return a; 
} 

/************************************************ 
 *   Draw the metal frame of the cycle and also 
 *   draw the seat and the back wheel with 
 *   this. 
 *   All these parts are always together in the 
 *   same plane.They never move out ot the 
 *   PLANE!   ;) 
 ************************************************/ 
void drawFrame() 
{ 
	glColor3f(1.0f,0.0f,0.0f); 

	/******************************** 
	 *   First draw the all the items 
	 *   at the center of the frame. 
	 *   Draw the bigger gear,the small 
	 *   cylinder acting as the socket 
	 *   for the pedals.Also DON'T 
	 *   forget to draw the two 
	 *   connecting cemtral rods 
	 *********************************/ 
	glPushMatrix(); 
	/****************************** 
	 *   Allow me to draw the BIGGER 
	 *   gear and the socket cylinder 
	 *******************************/ 
	glPushMatrix(); 
	/*************************** 
	 *   Let the gear have the 
	 *   green color 
	 ***************************/ 
	glColor3f(0.0f,1.0f,0.0f); 

	/************************** 
	 *   The gear should be 
	 *   outside the frame !!! 
	 *   This is the bigger 
	 *   GEAR 
	 ***************************/ 
	glPushMatrix(); 
	glTranslatef(0.0f,0.0f,0.06f); 
	glRotatef(-2*pedalAngle,0.0f,0.0f,1.0f); 
	gear(0.08f,0.3f,0.03f,30,0.03f); 
	glPopMatrix(); 
	/*************************** 
	 *   Restore the color of the 
	 *   frame 
	 ****************************/ 
	glColor3f(1.0f,0.0f,0.0f); 
	glTranslatef(0.0f,0.0f,-0.2f); 
	ZCylinder(0.08f,0.32f); 
	glPopMatrix(); 
	/***************************** 
	 *   Lets first draw the 
	 *   rightmost rod of the frame 
	 *******************************/ 
	glRotatef(RIGHT_ANGLE,0.0f,0.0f,1.0f); 
	XCylinder(ROD_RADIUS,RIGHT_ROD); 

	/******************************* 
	 *   Now draw the centre rod of 
	 *   the frame which also supports 
	 *   the seat 
	 *********************************/ 
	glRotatef(MIDDLE_ANGLE-RIGHT_ANGLE,0.0f,0.0f,1.0f); 
	XCylinder(ROD_RADIUS,MIDDLE_ROD); 
	/******************************** 
	 *   We have drawn the support.So 
	 *   let's draw the seat with a 
	 *   new color 
	 *********************************/ 
	glColor3f(1.0f,1.0f,0.0f); 
	glTranslatef(MIDDLE_ROD,0.0f,0.0f); 
	glRotatef(-MIDDLE_ANGLE,0.0f,0.0f,1.0f); 
	glScalef(0.3f,ROD_RADIUS,0.25f); 
	drawSeat(); 
	/********************** 
	 *   Restore the color ! 
	 ************************/ 
	glColor3f(1.0f,0.0f,0.0f); 
	glPopMatrix(); 
	/********************************* 
	 *   Draw the horizontal part of 
	 *   the frame. 
	 *********************************/ 

	/********************************* 
	 *   Draw the main single rod 
	 *   connecting the center of the 
	 *   frame to the back wheel of the 
	 *   cycle 
	 **********************************/ 
	glPushMatrix(); 
	glRotatef(-180.0f,0.0f,1.0f,0.0f); 
	XCylinder(ROD_RADIUS,BACK_CONNECTOR); 

	/*********************************** 
	 *   Draw the two rods on the either 
	 *   side of the wheel 
	 *   These rods are part of the 
	 *   horizontal part of the cycle 
	 ************************************/ 
	glPushMatrix(); 
	glTranslatef(0.5f,0.0f,WHEEL_OFFSET); 
	XCylinder(ROD_RADIUS,RADIUS_WHEEL+TUBE_WIDTH); 
	glPopMatrix(); 
	glPushMatrix(); 
	glTranslatef(0.5f,0.0f,-WHEEL_OFFSET); 
	XCylinder(ROD_RADIUS,RADIUS_WHEEL+TUBE_WIDTH); 
	glPopMatrix(); 
	glPopMatrix(); 

	/************************************ 
	 *   Draw the leftmost rods of the 
	 *   frame of the cycle 
	 *************************************/ 
	glPushMatrix(); 
	glTranslatef(-(BACK_CONNECTOR+RADIUS_WHEEL+TUBE_WIDTH),0.0f,0.0f); 
	/******************************** 
	 *   Transalted to the back wheel 
	 *   position.Why not draw the back 
	 *   wheel and also the gear ? :)) 
	 **********************************/ 
	glPushMatrix(); 
	glRotatef(-2*pedalAngle,0.0f,0.0f,1.0f); 
	drawTyre(); 
	glColor3f(0.0f,1.0f,0.0f); 
	gear(0.03f,0.15f,0.03f,20,0.03f); 
	glColor3f(1.0f,0.0f,0.0f); 
	glPopMatrix(); 
	glRotatef(LEFT_ANGLE,0.0f,0.0f,1.0f); 

	/************************************ 
	 *   Draw the two rods on the either 
	 *   side of the wheel connecting the 
	 *   backwheel and topmost horizontal 
	 *   part of the wheel 
	 *************************************/ 
	glPushMatrix(); 
	glTranslatef(0.0f,0.0f,-WHEEL_OFFSET); 
	XCylinder(ROD_RADIUS,WHEEL_LEN); 
	glPopMatrix(); 
	glPushMatrix(); 
	glTranslatef(0.0f,0.0f,WHEEL_OFFSET); 
	XCylinder(ROD_RADIUS,WHEEL_LEN); 
	glPopMatrix(); 

	/***************************** 
	 *   Draw the single rod of the 
	 *   same setup 
	 ******************************/ 
	glTranslatef(WHEEL_LEN,0.0f,0.0f); 
	XCylinder(ROD_RADIUS,CRANK_ROD); 

	/***************************** 
	 *   Now Draw The topmost 
	 *   Horizontal rod 
	 *****************************/ 
	glTranslatef(CRANK_ROD,0.0f,0.0f); 
	glRotatef(-LEFT_ANGLE,0.0f,0.0f,1.0f); 
	XCylinder(ROD_RADIUS,TOP_LEN); 

	/******************************* 
	 *   Now instead of again traversing 
	 *   all the way back and again 
	 *   forward.WHY NOT DRAW THE 
	 *   HANDLE FROM HERE ITSELF? 
	 ********************************/ 
	/***************************** 
	 *   Now draw the handle and 
	 *   small support rod which 
	 *   is incorporated in the 
	 *   frame itself. 
	 *   Set y-axis at te required 
	 *   incline. 
	 ******************************/ 
	glTranslatef(TOP_LEN,0.0f,0.0f); 
	glRotatef(-FRONT_INCLINE,0.0f,0.0f,1.0f); 

	/****************************** 
	 *   Draw the small rod that acts 
	 *   as the socket joining the 
	 *   frame and the handle 
	 ******************************/ 
	glPushMatrix(); 
	glTranslatef(-0.1f,0.0f,0.0f); 
	XCylinder(ROD_RADIUS,0.45f); 
	glPopMatrix(); 

	/****************************** 
	 *   I Hope the handle can rotate 
	 *   about its mean position 
	 *******************************/ 
	glPushMatrix(); 
	glRotatef(-steering,1.0f,0.0f,0.0f); 
	/****************************** 
	 *   Roll back to the height of 
	 *   the handle to draw it 
	 *******************************/ 
	glTranslatef(-0.3f,0.0f,0.0f); 

	/******************************** 
	 *   We cannot use the incline    
	 *   the incline to draw the 
	 *   horizontal part of the rod 
	 ********************************/ 
	glPushMatrix(); 
	glRotatef(FRONT_INCLINE,0.0f,0.0f,1.0f); 

	glPushMatrix(); 
	glTranslatef(0.0f,0.0f,-HANDLE_ROD/2); 
	ZCylinder(ROD_RADIUS,HANDLE_ROD); 
	glPopMatrix(); 

	glPushMatrix(); 
	glColor3f(1.0f,1.0f,0.0f); 
	glTranslatef(0.0f,0.0f,-HANDLE_ROD/2); 
	ZCylinder(0.07f,HANDLE_ROD/4); 
	glTranslatef(0.0f,0.0f,HANDLE_ROD*3/4); 
	ZCylinder(0.07f,HANDLE_ROD/4); 
	glColor3f(1.0f,0.0f,0.0f); 
	glPopMatrix(); 
	glPopMatrix(); 

	/********************************* 
	 *   Using this incline now draw 
	 *   the handle.Maybe use this again 
	 *   to draw the wheel. ;) 
	 **********************************/ 
	glPushMatrix(); 
	/**************************** 
	 *   Draw the main big rod 
	 ****************************/ 
	XCylinder(ROD_RADIUS,CRANK_ROD); 

	/****************************** 
	 *   Why not draw the two rods and 
	 *   the WHEEL?   :) 
	 *   Yes!So,first go to the 
	 *   end of the main rod. 
	 *******************************/ 
	glTranslatef(CRANK_ROD,0.0f,0.0f); 
	//				glRotatef(CRANK_ANGLE,0.0f,0.0f,1.0f); 		//no use

	/******************************* 
	 *   Draw the two rods connecting 
	 *   the handle and the front 
	 *   wheel. 
	 *   The two rods are at a incline 
	 *   to the connector. 
	 ********************************/ 
	glPushMatrix(); 
	glTranslatef(0.0f,0.0f,WHEEL_OFFSET); 
	XCylinder(ROD_RADIUS,CRANK_RODS); 
	glPopMatrix(); 
	glPushMatrix(); 
	glTranslatef(0.0f,0.0f,-WHEEL_OFFSET); 
	XCylinder(ROD_RADIUS,CRANK_RODS); 
	glPopMatrix(); 
	/******************************** 
	 *   Why not draw the wheel. 
	 *   The FRONT wheel to be precise 
	 *********************************/ 
	glTranslatef(CRANK_RODS,0.0f,0.0f); 
	glRotatef(-2*pedalAngle,0.0f,0.0f,1.0f); 
	drawTyre(); 
	glPopMatrix(); 
	glPopMatrix();   /*   End of the rotation of the handle effect   */ 
	glPopMatrix(); 
} 


void gear( GLfloat inner_radius, GLfloat outer_radius, GLfloat width, 
		GLint teeth, GLfloat tooth_depth ) 
{ 
	GLint i; 
	GLfloat r0, r1, r2; 
	GLfloat angle, da; 
	GLfloat u, v, len; 
	const double pi = 3.14159264; 

	r0 = inner_radius; 
	r1 = outer_radius - tooth_depth/2.0; 
	r2 = outer_radius + tooth_depth/2.0; 

	da = 2.0*pi / teeth / 4.0; 

	glShadeModel( GL_FLAT ); 

	glNormal3f( 0.0, 0.0, 1.0 ); 

	/* draw front face */ 
	glBegin( GL_QUAD_STRIP ); 
	for (i=0;i<=teeth;i++) { 
		angle = i * 2.0*pi / teeth; 
		glVertex3f( r0*cos(angle), r0*sin(angle), width*0.5 ); 
		glVertex3f( r1*cos(angle), r1*sin(angle), width*0.5 ); 
		glVertex3f( r0*cos(angle), r0*sin(angle), width*0.5 ); 
		glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), width*0.5 ); 
	} 
	glEnd(); 

	/* draw front sides of teeth */ 
	glBegin( GL_QUADS ); 
	da = 2.0*pi / teeth / 4.0; 
	for (i=0;i<teeth;i++) { 
		angle = i * 2.0*pi / teeth; 

		glVertex3f( r1*cos(angle),      r1*sin(angle),     width*0.5 ); 
		glVertex3f( r2*cos(angle+da),   r2*sin(angle+da),     width*0.5 ); 
		glVertex3f( r2*cos(angle+2*da), r2*sin(angle+2*da), width*0.5 ); 
		glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), width*0.5 ); 
	} 
	glEnd(); 


	glNormal3f( 0.0, 0.0, -1.0 ); 

	/* draw back face */ 
	glBegin( GL_QUAD_STRIP ); 
	for (i=0;i<=teeth;i++) { 
		angle = i * 2.0*pi / teeth; 
		glVertex3f( r1*cos(angle), r1*sin(angle), -width*0.5 ); 
		glVertex3f( r0*cos(angle), r0*sin(angle), -width*0.5 ); 
		glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), -width*0.5 ); 
		glVertex3f( r0*cos(angle), r0*sin(angle), -width*0.5 ); 
	} 
	glEnd(); 

	/* draw back sides of teeth */ 
	glBegin( GL_QUADS ); 
	da = 2.0*pi / teeth / 4.0; 
	for (i=0;i<teeth;i++) { 
		angle = i * 2.0*pi / teeth; 

		glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), -width*0.5 ); 
		glVertex3f( r2*cos(angle+2*da), r2*sin(angle+2*da), -width*0.5 ); 
		glVertex3f( r2*cos(angle+da),   r2*sin(angle+da),     -width*0.5 ); 
		glVertex3f( r1*cos(angle),      r1*sin(angle),     -width*0.5 ); 
	} 
	glEnd(); 


	/* draw outward faces of teeth */ 
	glBegin( GL_QUAD_STRIP ); 
	for (i=0;i<teeth;i++) { 
		angle = i * 2.0*pi / teeth; 

		glVertex3f( r1*cos(angle),      r1*sin(angle),      width*0.5 ); 
		glVertex3f( r1*cos(angle),      r1*sin(angle),     -width*0.5 ); 
		u = r2*cos(angle+da) - r1*cos(angle); 
		v = r2*sin(angle+da) - r1*sin(angle); 
		len = sqrt( u*u + v*v ); 
		u /= len; 
		v /= len; 
		glNormal3f( v, -u, 0.0 ); 
		glVertex3f( r2*cos(angle+da),   r2*sin(angle+da),      width*0.5 ); 
		glVertex3f( r2*cos(angle+da),   r2*sin(angle+da),     -width*0.5 ); 
		glNormal3f( cos(angle), sin(angle), 0.0 ); 
		glVertex3f( r2*cos(angle+2*da), r2*sin(angle+2*da),  width*0.5 ); 
		glVertex3f( r2*cos(angle+2*da), r2*sin(angle+2*da), -width*0.5 ); 
		u = r1*cos(angle+3*da) - r2*cos(angle+2*da); 
		v = r1*sin(angle+3*da) - r2*sin(angle+2*da); 
		glNormal3f( v, -u, 0.0 ); 
		glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da),  width*0.5 ); 
		glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), -width*0.5 ); 
		glNormal3f( cos(angle), sin(angle), 0.0 ); 
	} 

	glVertex3f( r1*cos(0.0), r1*sin(0.0), width*0.5 ); 
	glVertex3f( r1*cos(0.0), r1*sin(0.0), -width*0.5 ); 

	glEnd(); 


	glShadeModel( GL_SMOOTH ); 

	/* draw inside radius cylinder */ 
	glBegin( GL_QUAD_STRIP ); 
	for (i=0;i<=teeth;i++) { 
		angle = i * 2.0*pi / teeth; 
		glNormal3f( -cos(angle), -sin(angle), 0.0 ); 
		glVertex3f( r0*cos(angle), r0*sin(angle), -width*0.5 ); 
		glVertex3f( r0*cos(angle), r0*sin(angle), width*0.5 ); 
	} 
	glEnd(); 

} 


void drawChain() 
{ 
	GLfloat depth; 
	static int mode=0; 

	glColor3f(0.0f,1.0f,0.0f); 
	glEnable(GL_LINE_STIPPLE); 
	mode=(mode+1)%2; 

	if(mode==0 && speed>0) 
		glLineStipple(1,0x1c47); 
	else if(mode==1 && speed>0) 
		glLineStipple(1,0x00FF); 

	glBegin(GL_LINES); 
	for(depth=0.06f;depth<=0.12f;depth+=0.01f) 
	{ 
		glVertex3f(-1.6f,0.15f,ROD_RADIUS); 
		glVertex3f(0.0f,0.3f,depth); 

		glVertex3f(-1.6f,-0.15f,ROD_RADIUS); 
		glVertex3f(0.0f,-0.3f,depth); 
	} 
	glEnd(); 
	glDisable(GL_LINE_STIPPLE); 
} 

void drawSeat() 
{ 
	/********************************* 
	 *   Draw the top of the seat 
	 **********************************/ 
	glBegin(GL_POLYGON); 
	glVertex3f(-0.1f, 1.0f, -0.5f); 
	glVertex3f(   1.0f, 1.0f, -0.3f); 
	glVertex3f( 1.0f, 1.0f,  0.3f); 
	glVertex3f(-0.1f, 1.0f,  0.5f); 
	glVertex3f(-0.5f, 1.0f,  1.0f); 
	glVertex3f(-1.0f, 1.0f,  1.0f); 
	glVertex3f(-1.0f, 1.0f, -1.0f); 
	glVertex3f(-0.5f, 1.0f, -1.0f); 
	glEnd(); 

	/********************************** 
	 *   Draw the bottom base part of the 
	 *   seat 
	 ************************************/ 
	glBegin(GL_POLYGON); 
	glVertex3f(-0.1f, -1.0f, -0.5f); 
	glVertex3f(   1.0f, -1.0f, -0.3f); 
	glVertex3f( 1.0f, -1.0f,  0.3f); 
	glVertex3f(-0.1f, -1.0f,  0.5f); 
	glVertex3f(-0.5f, -1.0f,  1.0f); 
	glVertex3f(-1.0f, -1.0f,  1.0f); 
	glVertex3f(-1.0f, -1.0f, -1.0f); 
	glVertex3f(-0.5f, -1.0f, -1.0f); 
	glEnd(); 

	/********************** 
	 *   Draw the sides! 
	 ***********************/ 
	glBegin(GL_QUADS); 
	glVertex3f(1.0f,1.0f,-0.3f); 
	glVertex3f(1.0f,1.0f,0.3f); 
	glVertex3f(1.0f,-1.0f,0.3f); 
	glVertex3f(1.0f,-1.0f,-0.3f); 

	glVertex3f(1.0f,1.0f,0.3f); 
	glVertex3f(-0.1f,1.0f,0.5f); 
	glVertex3f(-0.1f,-1.0f,0.5f); 
	glVertex3f(1.0f,-1.0f,0.3f); 

	glVertex3f(1.0f,1.0f,-0.3f); 
	glVertex3f(-0.1f,1.0f,-0.5f); 
	glVertex3f(-0.1f,-1.0f,-0.5f); 
	glVertex3f(1.0f,-1.0f,-0.3f); 

	glVertex3f(-0.1f,1.0f,0.5f); 
	glVertex3f(-0.5f,1.0f,1.0f); 
	glVertex3f(-0.5f,-1.0f,1.0f); 
	glVertex3f(-0.1f,-1.0f,0.5f); 

	glVertex3f(-0.1f,1.0f,-0.5f); 
	glVertex3f(-0.5f,1.0f,-1.0f); 
	glVertex3f(-0.5f,-1.0f,-1.0f); 
	glVertex3f(-0.1f,-1.0f,-0.5f); 

	glVertex3f(-0.5f,1.0f,1.0f); 
	glVertex3f(-1.0f,1.0f,1.0f); 
	glVertex3f(-1.0f,-1.0f,1.0f); 
	glVertex3f(-0.5f,-1.0f,1.0f); 

	glVertex3f(-0.5f,1.0f,-1.0f); 
	glVertex3f(-1.0f,1.0f,-1.0f); 
	glVertex3f(-1.0f,-1.0f,-1.0f); 
	glVertex3f(-0.5f,-1.0f,-1.0f); 

	glVertex3f(-1.0f,1.0f,1.0f); 
	glVertex3f(-1.0f,1.0f,-1.0f); 
	glVertex3f(-1.0f,-1.0f,-1.0f); 
	glVertex3f(-1.0f,-1.0f,1.0f); 

	glEnd(); 


} 

void drawPedals() 
{ 
	glColor3f(0.0f,0.0f,1.0f); 
	/*************************** 
	 *   Lets draw the two pedals 
	 *   offset from the center 
	 *   of the frame. 
	 ****************************/ 

	/***************************** 
	 *   First draw the one visible 
	 *   to the viewer 
	 ******************************/ 
	glPushMatrix(); 
	glTranslatef(0.0f,0.0f,0.105f); 
	glRotatef(-pedalAngle,0.0f,0.0f,1.0f); 
	glTranslatef(0.25f,0.0f,0.0f); 
	/************************* 
	 *   Draw the pedal rod 
	 *************************/ 
	glPushMatrix(); 
	glScalef(0.5f,0.1f,0.1f); 
	glutSolidCube(1.0f); 
	glPopMatrix(); 

	/************************ 
	 *   Draw the resting pad 
	 ************************/ 
	glPushMatrix(); 
	glTranslatef(0.25f,0.0f,0.15f); 
	glRotatef(pedalAngle,0.0f,0.0f,1.0f); 
	glScalef(0.2f,0.02f,0.3f); 
	glutSolidCube(1.0f); 
	glPopMatrix(); 

	glPopMatrix(); 

	/******************************* 
	 *   Draw the one on the other 
	 *   side  of the frame 
	 *******************************/ 
	glPushMatrix(); 
	glTranslatef(0.0f,0.0f,-0.105f); 
	glRotatef(180.0f-pedalAngle,0.0f,0.0f,1.0f); 
	glTranslatef(0.25f,0.0f,0.0f); 

	/*************************** 
	 *   Now again draw the pedal 
	 *   rod 
	 ****************************/ 
	glPushMatrix(); 
	glScalef(0.5f,0.1f,0.1f); 
	glutSolidCube(1.0f); 
	glPopMatrix(); 

	/**************************** 
	 *   Draw the resting pad of 
	 *   the pedal 
	 *****************************/ 
	glPushMatrix(); 
	glTranslatef(0.25f,0.0f,-0.15f); 
	glRotatef(pedalAngle-180.0f,0.0f,0.0f,1.0f); 
	glScalef(0.2f,0.02f,0.3f); 
	glutSolidCube(1.0f); 
	glPopMatrix(); 

	glPopMatrix(); 

	glColor3f(1.0f,0.0f,0.0f); 
} 

void drawTyre(void) 
{ 
	int i; 
	//   Draw The Rim 
	glColor3f(1.0f,1.0f,1.0f); 
	glutSolidTorus(0.06f,0.92f,4,30); 
	//   Draw The Central Cylinder 
	//   Length of cylinder  0.12f 
	glColor3f(1.0f,1.0f,0.5f); 
	glPushMatrix(); 
	glTranslatef(0.0f,0.0f,-0.06f); 
	ZCylinder(0.02f,0.12f); 
	glPopMatrix(); 
	glutSolidTorus(0.02f,0.02f,3,20); 

	//   Draw The Spokes 
	glColor3f(1.0f,1.0f,1.0f); 
	for(i=0;i<NUM_SPOKES;++i) 
	{ 
		glPushMatrix(); 
		glRotatef(i*SPOKE_ANGLE,0.0f,0.0f,1.0f); 
		glBegin(GL_LINES); 
		glVertex3f(0.0f,0.02f,0.0f); 
		glVertex3f(0.0f,0.86f,0.0f); 
		glEnd(); 
		glPopMatrix(); 
	} 

	//   Draw The Tyre 
	glColor3f(0.0f,0.0f,0.0f); 
	glutSolidTorus(TUBE_WIDTH,RADIUS_WHEEL,10,30); 
	glColor3f(1.0f,0.0f,0.0f); 
} 

void landmarks(void) 
{ 
	GLfloat i; 
	glColor3f(0.0f,1.0f,0.0f); 

	/************************************ 
	 *   Draw the ground for the cycle 
	 *   Looks incomplete with it!Don't 
	 *   forget to define the normal 
	 *   vectors for the vertices. 
	 *   gotta fix this bug! 
	 ************************************/ 
	glBegin(GL_LINES); 
	for(i=-100.0f ; i<100.0f ; i += 1.0f) 
	{ 
		glVertex3f(-100.0f,-RADIUS_WHEEL,i); 
		glVertex3f( 100.0f,-RADIUS_WHEEL,i); 
		glVertex3f(i,-RADIUS_WHEEL,-100.0f); 
		glVertex3f(i,-RADIUS_WHEEL,100.0f); 
	} 
	glEnd(); 
} 

void display(void) 
{ 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glEnable(GL_NORMALIZE); 

	glPushMatrix();    
	/******************************* 
	 *    Prepare the rotations 
	 *    and start doing the 
	 *    remaining scene 
	 *******************************/ 
	glRotatef(angley,1.0f,0.0f,0.0f); 
	glRotatef(anglex,0.0f,1.0f,0.0f); 
	glRotatef(anglez,0.0f,0.0f,1.0f); 

	/*********************** 
	 *    Start rendering    
	 *    the scene; 
	 *    the bicycle ;) 
	 **********************/     

	//landmarks(); 

	/**************************** 
	 *   Move the cycle. 
	 ****************************/ 
	glPushMatrix(); 
	glRotatef(90.0f,0.0f,1.0f,0.0f);
	glTranslatef(xpos,0.0f,zpos); 
	glRotatef(direction,0.0f,1.0f,0.0f); 

	drawFrame();    
	drawChain(); 
	drawPedals(); 
	glPopMatrix(); 

	glPopMatrix(); 

	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity(); 
	gluLookAt(camx,camy,camz,  camx,0.0,0.0,  0.0,1.0,0.0); 

	glutSwapBuffers(); 
} 

GLfloat Abs(GLfloat a) 
{ 
	if(a < 0.0f) 
		return -a; 
	else 
		return a; 
} 

/************************ 
 *   Returns the value of 
 *   the given angle in 
 *   degrees 
 ************************/ 
GLfloat degrees(GLfloat a) 
{ 
	return a*180.0f/PI; 
} 

/************************ 
 *   Returns the value of 
 *   the given angle in 
 *   radians 
 ************************/ 
GLfloat radians(GLfloat a) 
{ 
	return a*PI/180.0f; 
} 

/************************* 
 *   The idle function of 
 *   the program which makes 
 *   the contniuous loop 
 ***************************/ 
void idle(void) 
{ 
	updateScene(); 
	glutPostRedisplay(); 
} 

class Terrain {
	private:
		int w; //Width
		int l; //Length
		float** hs; //Heights
		Vec3f** normals;
		bool computedNormals; //Whether normals is up-to-date
	public:
		Terrain(int w2, int l2) {
			w = w2;
			l = l2;

			hs = new float*[l];
			for(int i = 0; i < l; i++) {
				hs[i] = new float[w];
			}

			normals = new Vec3f*[l];
			for(int i = 0; i < l; i++) {
				normals[i] = new Vec3f[w];
			}

			computedNormals = false;
		}

		~Terrain() {
			for(int i = 0; i < l; i++) {
				delete[] hs[i];
			}
			delete[] hs;

			for(int i = 0; i < l; i++) {
				delete[] normals[i];
			}
			delete[] normals;
		}

		int width() {
			return w;
		}

		int length() {
			return l;
		}

		//Sets the height at (x, z) to y
		void setHeight(int x, int z, float y) {
			hs[z][x] = y;
			computedNormals = false;
		}

		//Returns the height at (x, z)
		float getHeight(int x, int z) {
			return hs[z][x];
		}

		//Computes the normals, if they haven't been computed yet
		void computeNormals() {
			if (computedNormals) {
				return;
			}

			//Compute the rough version of the normals
			Vec3f** normals2 = new Vec3f*[l];
			for(int i = 0; i < l; i++) {
				normals2[i] = new Vec3f[w];
			}

			for(int z = 0; z < l; z++) {
				for(int x = 0; x < w; x++) {
					Vec3f sum(0.0f, 0.0f, 0.0f);

					Vec3f out;
					if (z > 0) {
						out = Vec3f(0.0f, hs[z - 1][x] - hs[z][x], -1.0f);
					}
					Vec3f in;
					if (z < l - 1) {
						in = Vec3f(0.0f, hs[z + 1][x] - hs[z][x], 1.0f);
					}
					Vec3f left;
					if (x > 0) {
						left = Vec3f(-1.0f, hs[z][x - 1] - hs[z][x], 0.0f);
					}
					Vec3f right;
					if (x < w - 1) {
						right = Vec3f(1.0f, hs[z][x + 1] - hs[z][x], 0.0f);
					}

					if (x > 0 && z > 0) {
						sum += out.cross(left).normalize();
					}
					if (x > 0 && z < l - 1) {
						sum += left.cross(in).normalize();
					}
					if (x < w - 1 && z < l - 1) {
						sum += in.cross(right).normalize();
					}
					if (x < w - 1 && z > 0) {
						sum += right.cross(out).normalize();
					}

					normals2[z][x] = sum;
				}
			}

			//Smooth out the normals
			const float FALLOUT_RATIO = 0.5f;
			for(int z = 0; z < l; z++) {
				for(int x = 0; x < w; x++) {
					Vec3f sum = normals2[z][x];

					if (x > 0) {
						sum += normals2[z][x - 1] * FALLOUT_RATIO;
					}
					if (x < w - 1) {
						sum += normals2[z][x + 1] * FALLOUT_RATIO;
					}
					if (z > 0) {
						sum += normals2[z - 1][x] * FALLOUT_RATIO;
					}
					if (z < l - 1) {
						sum += normals2[z + 1][x] * FALLOUT_RATIO;
					}

					if (sum.magnitude() == 0) {
						sum = Vec3f(0.0f, 1.0f, 0.0f);
					}
					normals[z][x] = sum;
				}
			}

			for(int i = 0; i < l; i++) {
				delete[] normals2[i];
			}
			delete[] normals2;

			computedNormals = true;
		}

		//Returns the normal at (x, z)
		Vec3f getNormal(int x, int z) {
			if (!computedNormals) {
				computeNormals();
			}
			return normals[z][x];
		}
};

//Loads a terrain from a heightmap.  The heights of the terrain range from
//-height / 2 to height / 2.
Terrain* loadTerrain(const char* filename, float height) {
	Image* image = loadBMP(filename);
	Terrain* t = new Terrain(image->width, image->height);
	for(int y = 0; y < image->height; y++) {
		for(int x = 0; x < image->width; x++) {
			unsigned char color =
				(unsigned char)image->pixels[3 * (y * image->width + x)];
			float h = height * ((color / 255.0f) - 0.5f);
			t->setHeight(x, y, h);
		}
	}

	delete image;
	t->computeNormals();
	return t;
}

float _angle = 60.0f;
Terrain* _terrain;

void cleanup() {
	delete _terrain;
}

void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
}

void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
}


void changeSize(int w, int h) 
{
	float ratio =  ((float) w) / ((float) h); // window aspect ratio
	glMatrixMode(GL_PROJECTION); // projection matrix is active
	glLoadIdentity(); // reset the projection
	gluPerspective(45.0, ratio, 0.1, 500.0); // perspective transformation
	glMatrixMode(GL_MODELVIEW); // return to modelview mode
	glViewport(0, 0, w, h); // set viewport (drawing area) to entire window
}

//----------------------------------------------------------------------
// Draw one snowmen (at the origin)
//
// A snowman consists of a large body sphere and a smaller head sphere.
// The head sphere has two black eyes and an orange conical nose. To
// better create the impression they are sitting on the ground, we draw
// a fake shadow, consisting of a dark circle under each.
//
// We make extensive use of nested transformations. Everything is drawn
// relative to the origin. The snowman's eyes and nose are positioned
// relative to a head sphere centered at the origin. Then the head is
// translated into its final position. The body is drawn and translated
// into its final position.
//----------------------------------------------------------------------
void drawSnowman()
{
	glRotatef(coinangle,0.0f,1.0f,0.0f);
	glColor3f(0.85,0.85,0.2);
	glutSolidCylinder(1.5f,0.8f,20,20);

	// Draw body (a 20x20 spherical mesh of radius 0.75 at height 0.75)
	/*glColor3f(1.0, 1.0, 1.0); // set drawing color to white
	  glPushMatrix();
	  glTranslatef(0.0, 0.0, 0.75);
	  glutSolidSphere(0.75, 20, 20);
	  glPopMatrix();

	// Draw the head (a sphere of radius 0.25 at height 1.75)
	glPushMatrix();
	glTranslatef(0.0, 0.0, 1.75); // position head
	glutSolidSphere(0.25, 20, 20); // head sphere

	// Draw Eyes (two small black spheres)
	glColor3f(0.0, 0.0, 0.0); // eyes are black
	glPushMatrix();
	glTranslatef(0.0, -0.18, 0.10); // lift eyes to final position
	glPushMatrix();
	glTranslatef(-0.05, 0.0, 0.0);
	glutSolidSphere(0.05, 10, 10); // right eye
	glPopMatrix();
	glPushMatrix();
	glTranslatef(+0.05, 0.0, 0.0);
	glutSolidSphere(0.05, 10, 10); // left eye
	glPopMatrix();
	glPopMatrix();

	// Draw Nose (the nose is an orange cone)
	glColor3f(1.0, 0.5, 0.5); // nose is orange
	glPushMatrix();
	glRotatef(90.0, 1.0, 0.0, 0.0); // rotate to point along -y
	glutSolidCone(0.08, 0.5, 10, 2); // draw cone
	glPopMatrix();
	glPopMatrix();

	// Draw a faux shadow beneath snow man (dark green circle)
	glColor3f(0.0, 0.5, 0.0);
	glPushMatrix();
	glTranslatef(0.2, 0.2, 0.001);	// translate to just above ground
	glScalef(1.0, 1.0, 0.0); // scale sphere into a flat pancake
	glutSolidSphere(0.75, 20, 20); // shadow same size as body
	glPopMatrix();
	 */
}

//----------------------------------------------------------------------
// Update with each idle event
//
// This incrementally moves the camera and requests that the scene be
// redrawn.
//----------------------------------------------------------------------
void update(void) 
{
	coinangle+=10.0f;
	scores=0;
	for(int i=0;i<60;i++)
	{
		if(used[i]==0)
		{
			float t1,t2,t3,t4;
			t1=place_x[i];
			t2=x;
			t3=place_y[i];
			t4=y;
			if(((t2-t1)*(t2-t1)+(t3-t4)*(t3-t4))<=2.6f)
			{
				used[i]=1;
				scores++;
			}
		}
		else
			scores++;
	}
	if(fl==0)
	{
		if(rightflag==1)
		{
			//		angle-=2.0f;
			steering-=2.0f;
			//		lx=-sin(angle);
			//		ly=cos(angle);
			//		lx=cos(90+angle);
			//		ly=sin(90+angle);
		}
		if(leftflag==1)
		{
			//		angle+=2.0f;
			steering+=2.0f;
			//		lx=-sin(angle);
			//		ly=cos(angle);
		}
		float t;
		t=90+angle;
		lx=cos(DEG2RAD(t));
		ly=sin(DEG2RAD(t));
		if(deltaMove==1)
			acc+=0.005;
		else if(deltaMove==-1)
			acc-=0.005;
		else
		{
			if(acc>0)
			{
				acc-=0.005;
			}
		}
		if(speed==0 && deltaMove==1)
			speed=0.5;
		else if(deltaMove==1)
			speed+=0.002;
		//	speed+=deltaMove*0.01;
		if(speed<0)
			speed=0;
		else if((deltaMove==0 || deltaMove==-1) && speed>0)
		{
			//		printf("ADS %f %f\n",deltaMove,speed);
			speed-=0.1;
			pos_y+=speed*cos(radians(steering+direction));
			pos_x-=speed*sin(radians(steering+direction));
			x=pos_x;
			y=pos_y;
		}
		//	printf("%f %f %f %f\n",lx,ly,angle,DEG2RAD(t));
		if (deltaMove) { // update camera position
			//speed+=deltaMove*lx*4;
			//x += deltaMove * lx * 4;
			//y += deltaMove * ly * 4;
			initial=0;
			pos_y+=speed*cos(radians(steering+direction));
			pos_x-=speed*sin(radians(steering+direction));
			x=pos_x;
			y=pos_y;
			lx=-sin(radians(steering+direction));
			ly=cos(radians(steering+direction));		
			//printf("pos ::%f %f\n",pos_x,pos_y);		

		}
		else
		{
			if(jump==1)
			{
				lx=-sin(radians(steering+direction));
				ly=cos(radians(steering+direction));		
				x+=0.3*lx;
				y+=0.3*ly;
				pos_x=x;
				pos_y=y;
			}
			else
			{
				pos_y+=speed*cos(radians(steering+direction));
				pos_x-=speed*sin(radians(steering+direction));
				x=pos_x;
				y=pos_y;
			}
			//		x=pos_x;
			//		y=pos_y;
		}
		//	steering=0;
		lx=-sin(radians(direction));
		ly=cos(radians(direction));
		//	lx=-sin(radians(steering+direction));
		//	ly=cos(radians(steering+direction));
		//	lx=-sin(angle);
		//	ly=cos(angle);
		GLfloat sin_steering, cos_steering; 
		GLfloat rotation;
		pedalAngle += speed; 
		pedalAngle = degrees(angleSum(radians(pedalAngle), speed/RADIUS_WHEEL));
		sin_steering = sin(radians(steering)); 
		cos_steering = cos(radians(steering)); 
		rotation = atan2(speed * sin_steering, CYCLE_LENGTH + speed * cos_steering); 
		direction = degrees(angleSum(radians(direction),rotation));
		if(speed < 0.0f) 
			speed = 0.0f; 
		if(pedalAngle < 0.0f) 
			pedalAngle = 0.0f; 
		if(pedalAngle >= 360.0f) 
			pedalAngle -= 360.0f;
		if(roll_flag==1)
		{
			if(roll_angle<45)
			{
				roll_angle+=1;
			}
		}
		else if(roll_flag==2)
		{
			if(roll_angle>-45)
			{
				roll_angle-=1;
			}
		}
	}
	else
	{
		lx=-sin(radians(steering+direction));
		ly=cos(radians(steering+direction));		
		x+=0.4*lx*acc;
		y+=0.4*ly*acc;
		pos_x=x;
		pos_y=y;
		/*		vy=vy-0.02;
				dist+=((vy)*5)/7;
				float h=vx;
				if(h<0)
				h=-h;
		//		if(h<0)
		//			h=-h;
		y+=ly*0.425;
		x+=lx*0.425;
		pos_x=x;
		pos_y=y;*/
	}
	glutPostRedisplay(); // redisplay everything
}

//----------------------------------------------------------------------
// Draw the entire scene
//
// We first update the camera location based on its distance from the
// origin and its direction.
//----------------------------------------------------------------------
float heightAt(Terrain* terrain, float x, float z) {
	//Make (x, z) lie within the bounds of the terrain
	if (x < 0) {
		x = 0;
	}
	else if (x > terrain->width() - 1) {
		x = terrain->width() - 1;
	}
	if (z < 0) {
		z = 0;
	}
	else if (z > terrain->length() - 1) {
		z = terrain->length() - 1;
	}

	//Compute the grid cell in which (x, z) lies and how close we are to the
	//left and outward edges
	int leftX = (int)x;
	if (leftX == terrain->width() - 1) {
		leftX--;
	}
	float fracX = x - leftX;

	int outZ = (int)z;
	if (outZ == terrain->width() - 1) {
		outZ--;
	}
	float fracZ = z - outZ;

	//Compute the four heights for the grid cell
	float h11 = terrain->getHeight(leftX, outZ);
	float h12 = terrain->getHeight(leftX, outZ + 1);
	//	float h13 = terrain->getHeight(leftX, outZ - 1);
	float h21 = terrain->getHeight(leftX + 1.12, outZ);
	float h22 = terrain->getHeight(leftX + 1.12, outZ + 1);
	//	float h23 = terrain->getHeight(leftX + 1.12, outZ - 1);
	//	float h33=terrain->getHeight(leftX-1.58,outZ);
	//	float h36=terrain->getHeight(leftX-1.58,outZ-1);
	//	float h34=terrain->getHeight(leftX-1.58,outZ+1);
	//	gangle=atan(((h21-h11)*1.0)/1.12);
	//	gangle2=atan(((h11-h33)*1.0)/1.58);
	//	gangle3=atan(((h21-h33)*1.0)/2.7);
	//	fangle=(((gangle+gangle2+gangle3)/3)*180)/3.14;
	//	return (h11+h12+h13+h21+h22+h23+h33+h36+h34)/9;
	//	return (h11+h21+h33)/3;
	//Take a weighted average of the four heights
	return (1 - fracX) * ((1 - fracZ) * h11 + fracZ * h12) +
		fracX * ((1 - fracZ) * h21 + fracZ * h22);
}
void renderScene(void) 
{
	int i, j;

	// Clear color and depth buffers
	glClearColor(0.0, 0.7, 1.0, 1.0); // sky color is light blue
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	// Reset transformations
	glLoadIdentity();
	//	float scale = 800.0f / max(_terrain->width() - 1, _terrain->length() - 1);
	//glScalef(0.3, 0.3, 0.3);
	float posx,posy,posz;
	int px,py;
	//	px=x;
	//	py=y;
	/*	if(initial==1)
		{
		x=30;
		y=30;
		}*/
	/*
	   posx=(x*1.0)/scale;
	   posx+=(_terrain->width()*1.0)/2;
	   posy=(y*1.0)/scale;
	   posy+=(_terrain->length()*1.0)/2;
	   px=int(posx);
	   py=int(posy);
	   posz=(_terrain->getHeight(px,py))*4;
	   (heightAt(_terrain,px,py));
	   Vec3f tempnormal = (_terrain->getNormal(px,py)).normalize();
	   Vec3f t1(0.0f,1.0f,0.0f);
	   Vec3f t2(0.0f,0.0f,0.0f);
	   int flag=0;
	   float alpha=0,beta=0;
	   if(tempnormal[1]!=0)
	   {
	   if(tempnormal[2]!=0)
	   {
	   flag=0;
	   alpha=atan((tempnormal[0]*1.0)/tempnormal[2]);
	   beta=atan((sqrt(1-((tempnormal[1])*(tempnormal[1])))*1.0)/tempnormal[1]);
	   beta=1.57-beta;
	//printf("%f %f %f %f %f  -->",alpha,beta,tempnormal[0],tempnormal[2],tempnormal[1]);
	//	beta=DEG2RAD(beta);// abt x +ve;
	//	alpha=DEG2RAD(alpha);				//abt z +ve
	}
	else
	{
	flag=1;
	alpha=atan((tempnormal[1]*1.0)/tempnormal[0]);
	alpha=1.57-alpha;
	//printf("%f 0 -->",alpha);
	alpha=DEG2RAD(alpha); //abt y +ve
	}
	}
	t2+=t1.cross(tempnormal).normalize();
	// Set the camera centered at (x,y,1) and looking along directional
	// vector (lx, ly, 0), with the z-axis pointing up
	//printf("%f %f %f normal :: %f %f %f  %f %f %f\n",x,y,posz,tempnormal[0],tempnormal[2],tempnormal[1],t2[0],t2[2],t2[1]);*/
	posz=heightAt(_terrain,x,y);
	/*	height1=heightAt(_terrain,x-1.58*sin(direction),y-1.58*cos(direction));
		height2=heightAt(_terrain,x+1.58*sin(direction),y+1.58*cos(direction));
		rot_angle=(atan((height2-height1)/(3.16*cos(direction))))*180/3.14;*/
	float hprev=0,height3;
	height1=heightAt(_terrain,x-1.58*lx,y-1.58*ly);
	height2=heightAt(_terrain,x+1.58*lx,y+1.58*ly);
	height3=heightAt(_terrain,x-3.16*lx,y-3.16*ly);
	rot_angle=(atan((height2-height1)/(3.16)))*180/3.14;

	h1=height2;
	h2=h1;
	if(jump==1)
	{
		prev_angle=rot_angle;
		h1=prev_ht-0.2;
		posz=h1;
		//		height1=h1;
		height2=h1;
	}
	if(prev_ht-h2>0.1)
	{
		jump=1;
	}
	else
		jump=0;
	prev_ht=h1;
	if(jump==1)
		rot_angle=prev_angle;
	//	if(height2-prev_ht)
	//	{
	//		jump=1;
	//	}
	//	else
	//	{
	//		if(jump==0)
	//		{
	//			prev_ht=height2;
	//		}
	//	}

	//	if(fl==0)
	//		prevang=rot_angle;
	/*	if(jump)
		{
		if(abs(posz-prev_ht)<0.01)
		{
		jump=0;
		}
		else
		{
		posz=prev_ht-0.2f;
		prev_ht-=0.2f;
		}
		}*/
	//	printf("%f %f %f %f %f %f %f\n",height1,height2,fl,x,y,vx,vy);
	//	if(deltaMove!=-1 && 0.07+height2<=height1 && height1>height3 && fl==0 /*&& ((times%2)==0)|| hj==0*/)
	/*	{
		hj=1;
		fl=1;
		printf("1\n");
		dist=posz;
	//vy=speed*sin(prevang);
	//		vx=speed*cos(prevang);
	vy=0.4*sin(prevang);
	vx=0.4*cos(prevang);
	//		if(vx<0)
	//			vx=-vx;
	}
	else if(dist<posz && fl==1)
	{
	fl=0;
	vy=0;
	vx=0;
	times=1;
	}
	if(fl==1)
	{
	float temp_angle;
	temp_angle=(((atan(vy/vx))*180)/3.14);
	rot_angle=minimum(45,max(15,temp_angle));
	//		printf("%f %f %f %f %f %f %f %f\n",height1,height2,fl,x,y,vx,vy,rot_angle);
	posz=dist;
	}
	//	else
	//		printf("0\n");
	times++;*/
	//	printf("%f %f %f %f\n",rot_angle,height1,height2,3.6*ly);
	if(upview==0 && wheelview==0 && userview==0 && heliview==0 && sideview==0)
	{
		gluLookAt(
				x-7*lx,      y-7*ly,      max(posz,heightAt(_terrain,x-7*lx,y-7*ly))+2.0,
				x + lx, y + ly, posz+2.0,
				0.0,    0.0,    1.0);
	}
	else if(upview==1)
	{
		gluLookAt(
				x-10*lx,y-10*ly,posz+20,
				x+lx,y+ly,posz+1,
				0.0,0.0,1.0);
	}
	else if(wheelview==1)
	{
		gluLookAt(
				x+3*lx,y+3*ly,max(posz,heightAt(_terrain,x+3*lx,y+3*ly))+0.5,
				x+4*lx,y+4*ly,max(posz,heightAt(_terrain,x+4*lx,y+4*ly))+0.7,
				0.0,0.0,1.0);
	}
	else if(userview==1)
	{
		gluLookAt(
				x-lx,      y-ly,      posz+4,
				x + lx, y + ly, posz+3.3,
				0.0,    0.0,    1.0);
	}
	else if(heliview==1)
	{
		gluLookAt(
				x-4*lx,      y-4*ly,      posz+18,
				x, y, posz+0.5,
				0.0,    0.0,    1.0);
	}
	else if(sideview==1)
	{
		gluLookAt(
				x+12,      y+2.5,      posz+4,
				x+lx, y+ly, posz+4,
				0.0,    0.0,    1.0);
	}
	// Draw ground - 200x200 square colored green
	//glColor3f(0.0, 0.7, 0.0);
	//	glBegin(GL_QUADS);
	//		glVertex3f(-100.0, -100.0, 0.0);
	//		glVertex3f(-100.0,  100.0, 0.0);
	//		glVertex3f( 100.0,  100.0, 0.0);
	//		glVertex3f( 100.0, -100.0, 0.0);
	//	glEnd();
	glPushMatrix();
	//glTranslatef(0.0f, 0.0f, -10.0f);
	//	glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
	//	glRotatef(0.0f, 0.0f, 1.0f, 0.0f);

	GLfloat ambientColor[] = {0.4f, 0.4f, 0.4f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	GLfloat lightColor0[] = {0.6f, 0.6f, 0.6f, 1.0f};
	GLfloat lightPos0[] = {-0.5f, 0.8f, 0.1f, 0.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
	//	if(initial==1)
	//		glTranslatef(-30,-30,heightAt(_terrain,-30,-30)+1.5);

	//	glScalef(scale,scale,4);
	//		glTranslatef(-(float)(_terrain->width()) / 2,
	//				-(float)(((_terrain->length()) / 2)),-4.5f);
	//printf("%d %d %f\n",_terrain->width(),_terrain->length(),scale);
	glColor3f(0.3f, 0.9f, 0.0f);
	for(int z = 0; z < _terrain->length() - 1; z++) {
		//Makes OpenGL draw a triangle at every three consecutive vertices
		glBegin(GL_TRIANGLE_STRIP);
		for(int x = 0; x < _terrain->width(); x++) {
			float ht= _terrain->getHeight(x, z);
			if(ht<-8.0f)
				glColor3f(0.0f,0.0f,1.0f);
			else
				glColor3f(0.3f, 0.9f, 0.0f);
			Vec3f normal = _terrain->getNormal(x, z);
			glNormal3f(normal[0], normal[2], normal[1]);
			glVertex3f(x,z, _terrain->getHeight(x, z));
			normal = _terrain->getNormal(x, z + 1);
			glNormal3f(normal[0], normal[2], normal[1]);
			glVertex3f(x,z+1, _terrain->getHeight(x, z + 1));
		}
		glEnd();
	}
	glPopMatrix();
	//	if(wheelview==0)
	//	{
	//	glPushMatrix();
	//	glBegin(GL_QUADS);
	//	glVertex3f(-5,-5,0);
	//	glVertex3f(5,-5,0);
	//	glVertex3f(5,5,0);
	//	glVertex3f(-5,5,0);
	//	glEnd();
	//printf("%d %d\n",px,py);

	glPushMatrix();    
	/******************************* 

	 *    Prepare the rotations 

	 *    and start doing the 

	 *    remaining scene 

	 *******************************/ 
	glRotatef(angley,1.0f,0.0f,0.0f); 
	glRotatef(anglex,0.0f,1.0f,0.0f); 
	glRotatef(anglez,0.0f,0.0f,1.0f); 

	/*********************** 
	 *    Start rendering    
	 *    the scene; 
	 *    the bicycle ;) 
	 **********************/     

	//landmarks(); 

	/**************************** 
	 *   Move the cycle. 
	 ****************************/
	//	printf("%f %f %f %f %f\n",fangle,gangle,gangle2,gangle3,posz);
	glPushMatrix();
	glTranslatef(x,y,posz+1.2f); 
	glRotatef(angle,0,0,1);
	glRotatef(90.0f,1.0f,0.0f,0.0f);
	glRotatef(90.0f,0.0f,1.0f,0.0f);
	//	glRotatef(fangle,0.0f,0.0f,1.0f);
	//glTranslatef(-y,x,posz); 
	glRotatef(direction,0.0f,1.0f,0.0f); 
	glRotatef(rot_angle,0,0,1);
	glRotatef(roll_angle,1,0,0);

	drawFrame();    
	drawChain(); 
	drawPedals(); 
	glPopMatrix(); 
	glPopMatrix(); 


	glPushMatrix();
	glColor3f(1.0, 0.7, 0.0);
	//cout>>angle>>"\n";
	glTranslatef(pos_x,pos_y,posz);
	glRotatef(angle,0,0,1);
	//printf("%f %f\n",alpha,beta);
	/*if(flag==0)
	  {
	  glRotatef(alpha,0,0,1);
	  glRotatef(beta,1,0,0);
	  }
	  else
	  {
	  glRotatef(alpha,0,1,0);
	  }*/
	//printf("%d %d %f\n",px,py,posz);
	//	glTranslatef(px,py,_terrain->getHeight(px,py));
	//	cout>>angle>>"\n";
	//	printf("%f\n",angle);
	/*	glColor3f(1.0, 0.7, 0.0);
		glBegin(GL_QUADS);
		glVertex3f(-1,-1,0.5);
		glVertex3f(1,-1,0.5);
		glVertex3f(1,1,0.5);
		glVertex3f(-1,-1,0.5);
		glEnd();

		glColor3f(1.0, 0.0, 0.0);
		glBegin(GL_QUADS);
		glVertex3f(-1,-1,0.5);
		glVertex3f(-1,1,0.5);
		glVertex3f(-1,1,2.5);
		glVertex3f(-1,-1,2.5);
		glEnd();

		glColor3f(1.0, 0.0, 0.5);
		glBegin(GL_QUADS);
		glVertex3f(1,-1,0.5);
		glVertex3f(1,1,0.5);
		glVertex3f(1,1,2.5);
		glVertex3f(1,-1,2.5);
		glEnd();

		glColor3f(0.4, 0.2, 0.0);
		glBegin(GL_QUADS);
		glVertex3f(-1,-1,0.5);
		glVertex3f(+1,-1,0.5);
		glVertex3f(+1,-1,2.5);
		glVertex3f(-1,-1,2.5);
		glEnd();

		glColor3f(1.0, 0.0, 0.0);
		glBegin(GL_QUADS);
		glVertex3f(-1,1,0.5);
		glVertex3f(1,1,0.5);
		glVertex3f(1,1,2.5);
		glVertex3f(-1,1,2.5);
		glEnd();

		glColor3f(0.0, 0.0, 1.0);
		glBegin(GL_QUADS);
		glVertex3f(-1,-1,2.5);
		glVertex3f(1,-1,2.5);
		glVertex3f(1,1,2.5);
		glVertex3f(-1,1,2.5);
		glEnd();*/
	glPopMatrix();
	//	glPopMatrix();
	//	}
	// Draw 36 snow men
	/*	for(i = -3; i < 3; i++)
		for(j = -3; j < 3; j++) {
		glPushMatrix();
		glRotatef(90,1.0,0.0f,0.f);
		glTranslatef(i*60,(_terrain->getHeight(((i*60)/scale)+(_terrain->width()*1.0)/2,((j*60)/scale)+(_terrain->length()*1.0)/2)*4)+1.6f,-j*60);
	//glTranslatef(i*7.5, j*7.5, 0);
	drawSnowman();
	glPopMatrix();
	}
	 */
	glPushMatrix();
	glColor3f(1.0f,0.0f,0.0f);
	//glTranslatef(box_x -7*lx+3, box_y - 7*ly, 3.0 +z_co)
	//glRasterPos3f(box_x -1*lx, box_y - 1*ly, 3.0 +z_co+1.0);

	      glRasterPos3f(pos_x+4*lx,pos_y+4*ly, posz+6.0f);
	glScalef(5.0f,5.0f,5.0f);
	int k;
	GLvoid *font_style=GLUT_BITMAP_HELVETICA_18;
	char btime[100];
	sprintf(btime,"Score:%d",scores);
	for(k=0;btime[k]!='\0';k++)
		glutBitmapCharacter(font_style,btime[k]);
	glPopMatrix();

	for(i =0; i < 60; i++)
	{
		//              for(j = -6; j < 6; j++) {
		if(used[i]==0)
		{
			glPushMatrix();
			//      glTranslatef((float)(_terrain->width() - 1) / 2,0.0f,
			//                               (float)(_terrain->length() - 1) / 2);
			float g=heightAt(_terrain,place_x[i],place_y[i])+1.5;
			glTranslatef(place_x[i],place_y[i],g+1.5);
			glRotatef(90,1.0,0.0f,0.f);
			drawSnowman();
			glPopMatrix();
		}
	}

	glutSwapBuffers(); // Make it all visible
	} 

	//----------------------------------------------------------------------
	// User-input callbacks
	//
	// processNormalKeys: ESC, q, and Q cause program to exit
	// pressSpecialKey: Up arrow = forward motion, down arrow = backwards
	// releaseSpecialKey: Set incremental motion to zero
	//----------------------------------------------------------------------
	void processNormalKeys(unsigned char key, int xx, int yy)
	{
		if (key == ESC || key == 'q' || key == 'Q') exit(0);
		if(key=='1')
		{
			upview=upview^1;
			if(upview==1)
				wheelview=0;
		}
		if(key=='2')
		{
			wheelview=wheelview^1;
			if(wheelview==1)
				upview=0;
		}
		if(key=='3')
		{
			userview=userview^1;
		}
		if(key=='4')
		{
			heliview=heliview^1;
			//printf("%d\n",heliview);
		}
		if(key=='5')
		{
			sideview=sideview^1;
		}
		if(key=='a')
		{
			roll_flag=2;
		}
		if(key=='d')
		{
			roll_flag=1;
		}
	} 

	void pressSpecialKey(int key, int xx, int yy)
	{
		switch (key) {
			case GLUT_KEY_UP : deltaMove = 1.0; break;
			case GLUT_KEY_DOWN : deltaMove = -1.0; break;
			case GLUT_KEY_RIGHT : rightflag=1; break;
			case GLUT_KEY_LEFT : leftflag=1; break;
		}
	} 

	void releaseSpecialKey(int key, int x, int y) 
	{
		switch (key) {
			case GLUT_KEY_UP : deltaMove = 0.0; break;
			case GLUT_KEY_DOWN : deltaMove = 0.0; break;
			case GLUT_KEY_RIGHT : rightflag = 0.0; break;
			case GLUT_KEY_LEFT : leftflag = 0.0; break;
		}
	} 

	//----------------------------------------------------------------------
	// Process mouse drag events
	// 
	// This is called when dragging motion occurs. The variable
	// angle stores the camera angle at the instance when dragging
	// started, and deltaAngle is a additional angle based on the
	// mouse movement since dragging started.
	//----------------------------------------------------------------------
	void mouseMove(int x, int y) 
	{ 	
		//	printf("ENETERD\n");
		if (isDragging) { // only when dragging
			// update the change in angle
			deltaAngle = (x - xDragStart) * 0.005;

			// camera's direction is set to angle + deltaAngle
			lx = -sin(angle + deltaAngle);
			ly = cos(angle + deltaAngle);
		}
	}

	void mouseButton(int button, int state, int x, int y) 
	{
		//	printf("ENETERD\n");
		GetOGLPos(x,y);
		if (button == GLUT_LEFT_BUTTON) {
			if (state == GLUT_DOWN) { // left mouse button pressed
				//	isDragging = 1; // start dragging
				//	xDragStart = x; // save x where button first pressed
			}
			else  { /* (state = GLUT_UP) */
				//	angle += deltaAngle; // update camera turning angle
				//	isDragging = 0; // no longer dragging
			}
		}
	}

	//----------------------------------------------------------------------
	// Main program  - standard GLUT initializations and callbacks
	//----------------------------------------------------------------------
	void releaseNormalKeys(unsigned char key,int x,int y)
	{
		if(key=='a')
		{
			roll_flag=0;
			roll_angle=0;
		}
		if(key=='d')
		{
			roll_flag=0;
			roll_angle=0;
		}
	}
	int main(int argc, char **argv) 
	{
		printf("\n\
				-----------------------------------------------------------------------\n\
				OpenGL Sample Program:\n\
				- Drag mouse left-right to rotate camera\n\
				- Hold up-arrow/down-arrow to move camera forward/backward\n\
				- q or ESC to quit\n\
				-----------------------------------------------------------------------\n");

		// general initializations
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
		glutInitWindowPosition(100, 100);
		glutInitWindowSize(800, 400);
		glutCreateWindow("OpenGL/GLUT Sampe Program");
		initRendering();

		_terrain = loadTerrain("heightmap.bmp", 20);
		x=-30;
		y=-30;
		printf("%d\n",_terrain->width());
		for(int i=0;i<60;i++)
		{   
			place_x[i]=rand()%180;
			place_y[i]=rand()%180;
			//printf("%f %f %f\n",place_x[i],place_y[i],heightAt(_terrain,place_x[i],place_y[i]));
			//              printf("%f %f\n",place_x[i],place_y[i]);
		}   


		// register callbacks
		glutIdleFunc(idle);

		glutReshapeFunc(changeSize); // window reshape callback
		glutDisplayFunc(renderScene); // (re)display callback
		glutIdleFunc(update); // incremental update 
		glutIgnoreKeyRepeat(1); // ignore key repeat when holding key down
		glutMouseFunc(mouseButton); // process mouse button push/release
		//		glutMouseFunc(handleMouseclick);
		glutMotionFunc(mouseMove); // process mouse dragging motion
		glutKeyboardFunc(processNormalKeys); // process standard key clicks
		glutKeyboardUpFunc(releaseNormalKeys); // process standard key clicks
		glutSpecialFunc(pressSpecialKey); // process special key pressed
		// Warning: Nonstandard function! Delete if desired.
		glutSpecialUpFunc(releaseSpecialKey); // process special key release

		// OpenGL init
		glEnable(GL_DEPTH_TEST);

		// enter GLUT event processing cycle
		glutMainLoop();

		return 0; // this is just to keep the compiler happy
	}
