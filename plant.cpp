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
#include <unistd.h>
//#include "mouse.h"
#define MOUSE_ROTATE_YX		0
#define MOUSE_ROTATE_YZ		1
#define MOUSE_ZOOM			2

int mouse_mode = 0;

/* GLOBAL VARAIBLES */
/* (storage is actually allocated here) */
int W=800;		/* window width */
int H=600;		/* window height */
int X_OFF = 10;	/* window x offset */
int Y_OFF = 10;	/* window y offset */
float rotY = 0;
float rotZ = 0;
int treeStep = 1;
bool orth = true;
int m_last_x, m_last_y;

/* The dimensions of the viewing frustum */
GLfloat fleft   = -60.0;
GLfloat fright  =  60.0;
GLfloat fbottom = -60.0;
GLfloat ftop    =  60.0;
GLfloat zNear   = -100.0;
GLfloat zFar    = 400.0;

/* Global zoom factor.  Modified by user input. Initially 1.0 */
GLfloat zoomFactor = 1.0; 

/* local function declarations */
void display(void);
void init(void);
int endPlant(int);
void myKeyHandler(unsigned char ch, int x, int y);
void rotateCamera(double deg, int axis);
void growAnim();
void myMouseButton(int button, int state, int x, int y);
void myMouseMotion(int x, int y);

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

  glutMouseFunc(myMouseButton);
  glutMotionFunc(myMouseMotion);

  glutMainLoop();
  return 0;
  
}
void toOrtho()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(fleft, fright, fbottom, ftop, zNear, zFar);
}

void init() {
  glClearColor(0.0, 0.0, 0.0, 0.0);  
    toOrtho();
}



void toFrust()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glFrustum(-40.0, 40.0, -40.0, 40.0, 0.0, 40.0);
    glFrustum(fleft, fright, fbottom, ftop, zNear, zFar);
    gluPerspective(80, 4.0/3.0, 10, 400); 
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

void myMouseButton(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		m_last_x = x;
		m_last_y = y;

		if (button == GLUT_LEFT_BUTTON) {
			mouse_mode = MOUSE_ROTATE_YX;
	/*	} else if (button == GLUT_MIDDLE_BUTTON) {
			mouse_mode = MOUSE_ROTATE_YZ;
		} else if (button == GLUT_RIGHT_BUTTON) {
			mouse_mode = MOUSE_ZOOM;
		} */
	}
  }
}
void myMouseMotion(int x, int y) {
	double d_x, d_y;	

	d_x = x - m_last_x;
	d_y = y - m_last_y;

	m_last_x = x;
	m_last_y = y;

	if (mouse_mode == MOUSE_ROTATE_YX) {
		/* scaling factors 
		d_x /= 2.0;
		d_y /= 2.0;

		glRotatef(d_x, 0.0, 1.0, 0.0);	
		glRotatef(-d_y, 1.0, 0.0, 0.0);	*/
        rotateTree(d_x, Y_AXIS);
        }

    display();
	glutPostRedisplay();
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
            std::cout<<"Rotating the tree 5 degree counter clockwise about the y-axis"<<std::endl;
		    rotateTree(5, Y_AXIS);
			break;
			
		case '?':
		    // rotate cw about y axis
            std::cout<<"Rotating the tree 5 degree clockwise about the y-axis"<<std::endl;
		    rotateTree(-5, Y_AXIS);
		    break;
		    
	    case '.':
            rotateTree(5, Z_AXIS);
	        break;
	        
        case '>':
            rotateTree(-5, Z_AXIS);
            break;
			
	    case 'p':
	        if (!orth)
	        {
	            std::cout<<"Switching to orthographic view"<<std::endl;
	            toOrtho();
	            orth = true;
            }
            else
            {
                std::cout<<"Switching to frustum view"<<std::endl;
                // parameters in toFrust need to change
                toFrust();
                orth = false;
            }
	        break;	
	        
        case 'a':
            ++treeStep;
            std::cout<<"Incremented the fractal step by one"<<std::endl;
            break;
            
        case 's':
            if (treeStep > 0)
            {
                std::cout<<"Decremented the fractal step by one"<<std::endl;
                --treeStep;
            }
            break;

        case 'q':
            endPlant(0);
            break;

        case 'g':
     std::cout << "Starting the growing animation" << std::endl;
            growAnim();
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

void growAnim(){
     for(int i = 0; i < 5; ++i){
        sleep(1);
        treeStep = i;
        display();
     }
}

void display() {
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    initMatrixStack();

    if (!orth)
        translate(0, 0, -80);
    
    push();
	drawPlant(0, rotY, rotZ, treeStep);
	pop();
	

	
    glFlush();  /* Flush all executed OpenGL ops finish */

    /*
     * Since we are using double buffers, we need to call the swap
     * function every time we are done drawing.
     */
    glutSwapBuffers();
}

/* end of plant.c */
