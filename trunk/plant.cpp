/*
 * plant.cpp
 * ---------
 * Contains function callbacks and bookkeeping used by glut.  Starter code
 * for Project 2.
 *
 * Group Members: Yohannes Himawan, David Lee
 */

#ifdef _WIN32
#include <windows.h>
#endif
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "common.h"
#include "drawplant.h"
#include "mouse.h"

/* GLOBAL VARAIBLES */
/* (storage is actually allocated here) */
int W=800;		/* window width */
int H=600;		/* window height */
int X_OFF = 10;	/* window x offset */
int Y_OFF = 10;	/* window y offset */
float rotY = 0;
float rotZ = 0;

/* The dimensions of the viewing frustum */
GLfloat fleft   = -1.0;
GLfloat fright  =  1.0;
GLfloat fbottom = -1.0;
GLfloat ftop    =  1.0;
GLfloat zNear   = -2.0;
GLfloat zFar    = -7.0;

GLfloat curMatrix[16] = {1.0, 0.0, 0.0, 0.0,
		 	 0.0 , 1.0, 0.0, 0.0, 
			0.0, 0.0, 1.0, 0.0,
			0.0, 0.0, 0.0, 0.1};

/* Global zoom factor.  Modified by user input. Initially 1.0 */
GLfloat zoomFactor = 1.0; 
GLfloat cam[] = {1.0, 0.0, 0.0, 0.0,
	       0.0, 1.0, 0.0, 0.0,
	       0.0, 0.0, 1.0, 0.0,
	       0.0, 0.0, 0.0, 1.0};

/* local function declarations */
void display(void);
void init(void);
int endPlant(int);
void myKeyHandler(unsigned char ch, int x, int y);
void rotateCamera(double deg, int axis);
void draw_cone_tri_calc(double height, double radius, int base_tri);

enum Axis {X_AXIS, Y_AXIS, Z_AXIS};

int main (int argc, char** argv) 
{
  glutInit(&argc,argv);
  glutInitWindowSize(W, H);
  glutInitWindowPosition(X_OFF, Y_OFF);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow("plant");
  init();
  glutDisplayFunc(display);
  glutKeyboardFunc(myKeyHandler); 

  //glutMouseFunc(myMouseButton);
  // glutMotionFunc(myMouseMotion);
  glutMainLoop();
  return 0;
  
}

void init() {
  glClearColor(0.0, 0.0, 0.0, 0.0);  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-40.0, 40.0, -40.0, 40.0, -10.0, 10.0);
}



/*
 * The rotation is specified in degrees about a certain axis of
 * the original model.
 *
 * AXIS should be either X_AXIS, Y_AXIS, or Z_AXIS.
 *
 * Positive degrees rotate in the counterclockwise direction.
 */
void rotateCamera(double deg, Axis a) {
       //glMatrixMode(GL_PROJECTION);

	GLfloat temp[16];
        

	if (a == X_AXIS) {
	GLfloat rotm[16] = {1.0, 0.0, 0.0, 0.0, 
		0.0, cos(deg), -sin(deg), 0.0, 
		0.0, sin(deg), cos(deg), 0.0,
		0.0, 0.0, 0.0, 1.0};
        mat_multiplym(cam, rotm, 4, temp);
	mat_copy(cam, temp, 4);

        load3DMatrixp(cam[0], cam[1], cam[2], cam[3],
			cam[4], cam[5], cam[6], cam[7],
			cam[8], cam[9], cam[10], cam[11],
			cam[12], cam[13], cam[14], cam[15]);
	//glLoadMatrixf(cam);
	} else if (a == Y_AXIS) {
	/*
	GLfloat rotm[16] = {
		cos(deg) , 0.0 ,  sin(deg), 0.0, 
		0.0,    1.0, 0.0, 0.0, 
		- sin (deg), 0.0, cos (deg), 0.0,
		0.0, 0.0, 0.0, 1.0};
        mat_multiplym(cam, rotm, 4, temp);
	mat_copy(cam, temp, 4);

        load3DMatrixp(cam[0], cam[1], cam[2], cam[3],
			cam[4], cam[5], cam[6], cam[7],
			cam[8], cam[9], cam[10], cam[11],
			cam[12], cam[13], cam[14], cam[15]);*/
			rotatey(deg);
	} else if (a == Z_AXIS) {
	GLfloat rotm[16] = {
		cos(deg) , - sin(deg), 0.0, 0.0,
		sin(deg),    cos(deg), 0.0, 0.0, 
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0};
        mat_multiplym(cam, rotm, 4, temp);
	mat_copy(cam, temp, 4);

        load3DMatrixp(cam[0], cam[1], cam[2], cam[3],
			cam[4], cam[5], cam[6], cam[7],
			cam[8], cam[9], cam[10], cam[11],
			cam[12], cam[13], cam[14], cam[15]);

	}
 #if 0
        mat_multiplym(cam, rotm[16], 4, temp);
	mat_copy(mat, temp, 4);

	load3DMatrix(cam);
	#endif
}

void rotateTree(double deg, Axis a)
{
    if (a == X_AXIS)
    {

    }
    else if (a == Y_AXIS)
    {
        rotY += deg;
    }
    else
    {
        rotZ += deg;
    }
}

/* Handle user input */
void myKeyHandler(unsigned char ch, int x, int y) {
	switch(ch) {

#if 0
'/' - rotates the plant about its y-axis counterclockwise (i.e. when you are looking at the plant from the top down, the plant rotates counterclockwise).
'?' - rotates the plant about its y-axis clockwise.
'q' - quits the program
#endif
		case '/':
		    // rotate ccw about y axis
		    //rotateTree(5, Y_AXIS);
		    rotateCamera(5, Y_AXIS);
			break;
			
		case '?':
		    // rotate cw about y axis
		    rotateCamera(-5, Y_AXIS);
		    //rotateTree(-5, Y_AXIS);
		    break;
		    
		case ',':
		    std::cout<<"ROTATING X"<<std::endl;
			rotateCamera(.1, X_AXIS);
			break;

		case '<':
		    std::cout<<"ROTATING -X"<<std::endl;
			rotateCamera(-.1, X_AXIS);
			break;


		case '.':
			std::cout<<"ROTATING Z"<<std::endl;
			rotateCamera(.1, Z_AXIS);
			break;

		case '>':
			std::cout<<"ROTATING -Z"<<std::endl;
			rotateCamera(-.1, Z_AXIS);
			break;

        case 'q':
            endPlant(0);
            break;

		default:
			/* Unrecognized key press, just return */
			return;

			break;
	}

	/*
	 * If control reaches here, the key press was recognized.  Refresh
	 * the screen, since most key presses change the display in some way.
	 */
	display();

	return;
}


int endPlant(int status) {
  printf("\nQuitting plant.\n\n");
  fflush(stdout);

  exit(status);
}

void display() {
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
	initMatrixStack();
	push();
	drawPlant(0, rotY, rotZ);
	pop();
	
    glFlush();  /* Flush all executed OpenGL ops finish */

    /*
     * Since we are using double buffers, we need to call the swap
     * function every time we are done drawing.
     */
    glutSwapBuffers();
}

#if 0
void draw_cone_tri_calc(double height, double radius, int base_tri) {
    // >= 3 use isoceles triangles with the legs as radii
	/* All arguments here are pointers */

    // get the angle for each triangle
    double theta = 2 * 3.14159 / base_tri;
    int q = 0;
    //printf("Theta %d", theta);
    for (q = 0; q < base_tri; ++q)
    {
        // draw the base triangle, the ones which approximate the circular base of a cone
        // this is somewhat inefficient as it reuses verticies
    	glBegin(GL_TRIANGLES);
        // pick color based on loop var
    	glColor3f( (q%3 == 0 ? 1 : 0), (q%3 == 1 ? 1 : 0), (q%3 == 2 ? 1 : 0));
	    glVertex3f(0, 0, 0);
	    glVertex3f(radius * cos(q * theta), radius * sin(q * theta), 0);
	    glVertex3f(radius * cos( (q + 1)  * theta), radius * sin( (q + 1) * theta), 0);
        glEnd();

        // draw the triangle that make up the cone
    	glBegin(GL_TRIANGLES);
    	glColor3f( (q%3 == 2 ? 1 : 0), (q%3 == 1 ? 1 : 0), (q%3 == 0 ? 1 : 0));
	    glVertex3f(0, 0, height);
	    glVertex3f(radius * cos(q * theta), radius * sin(q * theta), 0);
	    glVertex3f(radius * cos( (q + 1)  * theta), radius * sin( (q + 1) * theta), 0);
        glEnd();
    }
}
#endif

/* end of plant.c */
