/*
 * plant.cpp
 * ---------
 * Contains function callbacks and bookkeeping used by glut.  Starter code
 * for Project 2.
 *
 * Group Members: <FILL IN>
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

/* GLOBAL VARAIBLES */
/* (storage is actually allocated here) */
int W=800;		/* window width */
int H=600;		/* window height */
int X_OFF = 10;	/* window x offset */
int Y_OFF = 10;	/* window y offset */
float rotY = 0;
float rotZ = 0;

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
	double x, y, z;

	x = 0;
	y = 0;
	z = 0;

	if (a == X_AXIS) {
		x = 1.0f;
	} else if (a == Y_AXIS) {
	    y = 1.0f;
		//rotatey(deg);
	} else if (a == Z_AXIS) {
        z = 1.0f;
		//rotatez(deg);
	}
 
	glRotatef(deg, x, y, z);
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
		    rotateTree(5, Y_AXIS);
			break;
			
		case '?':
		    // rotate cw about y axis
		    rotateTree(-5, Y_AXIS);
		    break;
		    
		case ',':
		    std::cout<<"ROTATING X"<<std::endl;
			rotateCamera(5, X_AXIS);
			break;

		case '<':
		    std::cout<<"ROTATING -X"<<std::endl;
			rotateCamera(-5, X_AXIS);
			break;

		case '.':
		    std::cout<<"ROTATING Y"<<std::endl;
			rotateCamera(5, Y_AXIS);
			break;

		case '>':
    		std::cout<<"ROTATING -Y"<<std::endl;
			rotateCamera(-5, Y_AXIS);
			break;

		case ';':
			std::cout<<"ROTATING Z"<<std::endl;
			rotateCamera(5, Z_AXIS);
			break;

		case ':':
			std::cout<<"ROTATING -Z"<<std::endl;
			rotateCamera(-5, Z_AXIS);
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
    
	drawPlant(rotY, rotZ);
	//draw_cone_tri_calc(5, 5, 5);
	
    glFlush();  /* Flush all executed OpenGL ops finish */

    /*
     * Since we are using double buffers, we need to call the swap
     * function every time we are done drawing.
     */
    glutSwapBuffers();
}

void draw_cone_tri_calc(double height, double radius, int base_tri) {
	/* ADD YOUR CODE HERE */
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

/* end of plant.c */
