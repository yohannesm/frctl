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

/* local function declarations */
void display(void);
void init(void);
int endPlant(int);
void myKeyHandler(unsigned char ch, int x, int y);
void rotateCamera(double deg, int axis);

enum Axis {X_AXIS, Y_AXIS, Z_AXIS};

int main (int argc, char** argv) {
/*
  GLfloat m1[16] = { 1, 1, 1, 1, 
		      1, 1, 1, 1,
		      1, 1, 1, 1,
		      1, 1, 1, 1};
 GLfloat v1[4] = {1, 1, 1, 1};
 GLfloat result[4];
 mat_multiplyv(m1, v1, 4, result);
 print_vec(result, 4);
 */
  GLfloat m1[16] = { 1, 1, 1, 1, 
		      1, 1, 1, 1,
		      1, 1, 1, 1,
		      1, 1, 1, 1};
  GLfloat m2[16] = { 1, 1, 1, 1, 
		      1, 1, 1, 1,
		      1, 1, 1, 1,
		      1, 1, 1, 1};
 GLfloat result[16];
 mat_multiplym(m1, m2, 4, result);
 print_mat(result, 4);
 
#if 0
  glutInit(&argc,argv);
  glutInitWindowSize(W, H);
  glutInitWindowPosition(X_OFF, Y_OFF);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow("plant");
  init();
  glutDisplayFunc(display);
  glutKeyboardFunc(myKeyHandler); 
  glutMainLoop();
  #endif
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
	} else if (a == Z_AXIS) {
		z = 1.0f;
	}
 
	glRotatef(deg, x, y, z);
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
			break;
			
		case '?':
		    // rotate cw about y axis
		    break;
		    
		case ',':
			rotateCamera(5, X_AXIS);
			break;

		case '<':
			rotateCamera(-5, X_AXIS);
			break;

		case '.':
			rotateCamera(5, Y_AXIS);
			break;

		case '>':
			rotateCamera(-5, Y_AXIS);
			break;

		case ';':
			rotateCamera(5, Z_AXIS);
			break;

		case ':':
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
	//myDisplay();

	return;
}


int endPlant(int status) {
  printf("\nQuitting plant.\n\n");
  fflush(stdout);

  exit(status);
}

void display() {
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	/* See drawplant.c for the definition of this routine */
	drawPlant();


    glFlush();  /* Flush all executed OpenGL ops finish */

    /*
     * Since we are using double buffers, we need to call the swap
     * function every time we are done drawing.
     */
    glutSwapBuffers();
}

/* end of plant.c */
