/*
 * drawplant.cpp
 * -------------
 * Contains the drawing routine and related helper functions for the
 * L-system fractal plant.  Starter code for Project 2.
 *
 * Group Members: Yohannes Himawan, David Lee
 */

#ifdef _WIN32
#include <windows.h>
#endif
#include <math.h>
#include <assert.h>

#include <iostream>
#include "Matrix.h"
#include "common.h"
#include "drawplant.h"
#include "readppm.h"

#define PI 3.14159265

/* Takes a 2D matrix in row-major order, and loads the 3D matrix which
   does the same trasformation into the OpenGL MODELVIEW matrix, in
   column-major order. */
void load2DMatrix(
		GLfloat m00, GLfloat m01, GLfloat m02,
		GLfloat m10, GLfloat m11, GLfloat m12,
		GLfloat m20, GLfloat m21, GLfloat m22) {

	GLfloat M3D [16];  /* three dimensional matrix doing same transform */

	M3D[0] = m00;  M3D[1] = m10; M3D[2] = 0.0; M3D[3] = m20;
	M3D[4] = m01;  M3D[5] = m11; M3D[6] = 0.0; M3D[7] = m21;
	M3D[8] = 0.0;  M3D[9] = 0.0; M3D[10] = 1.0; M3D[11] = 0.0;
	M3D[12] = m02; M3D[13] = m12; M3D[14] = 0.0; M3D[15] = m22;

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(M3D);
}

/* Takes a 3D matrix in row-major order, and loads the 3D matrix which
   does the same trasformation into the OpenGL MODELVIEW matrix, in
   column-major order. */
void load3DMatrix(
		GLfloat m00, GLfloat m01, GLfloat m02, GLfloat m03,
		GLfloat m10, GLfloat m11, GLfloat m12, GLfloat m13,
		GLfloat m20, GLfloat m21, GLfloat m22, GLfloat m23,
		GLfloat m30, GLfloat m31, GLfloat m32, GLfloat m33) {

	GLfloat M3D [16];  

	M3D[0] = m00;  M3D[1] = m10; M3D[2] = m20; M3D[3] = m30;
	M3D[4] = m01;  M3D[5] = m11; M3D[6] = m21; M3D[7] = m31;
	M3D[8] = m02;  M3D[9] = m12; M3D[10] = m22; M3D[11] = m32;
	M3D[12] = m03; M3D[13] = m13; M3D[14] = m23; M3D[15] = m33;

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(M3D);

}

void translate(GLfloat x, GLfloat y, int z = 1){
	load3DMatrix(
		1.0 , 0.0, 0.0, x,
		0.0, 1.0, 0.0, y, 
		0.0, 0.0, 1, z,
		0.0, 0.0, 0.0, 1);
}

void scale(int x, int y, int z = 1){
	load3DMatrix(
		x , 0.0, 0.0, 0.0,
		0.0, y, 0.0, 0.0, 
		0.0, 0.0, z, 0.0,
		0.0, 0.0, 0.0, 1.0);
}

void rotatez(GLfloat angle){
        GLfloat deg = angle*PI/180.0;
	load3DMatrix(
		cos(deg) , - sin(deg), 0.0, 0.0,
		sin(deg),    cos(deg), 0.0, 0.0, 
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1);
}


void rotatey(GLfloat angle){
        GLfloat deg = angle*PI/180.0;
	load3DMatrix(
		cos(deg) , 0.0 ,  sin(deg), 0.0, 
		0.0,    1.0, 0.0, 0.0, 
		- sin (deg), 0.0, cos (deg), 0.0,
		0.0, 0.0, 0.0, 1.0);
}



void drawLeaf(void) {
	/* ADD YOUR CODE to make the 2D leaf a 3D extrusion */
	glColor3f(0.1,0.9,0.1); 
	glBegin(GL_POLYGON);
	
	glVertex3f(0.0,0.0,0.0);
	glVertex3f(1.0,0.7,0.0);
	glVertex3f(1.3,1.8,0.0);
	glVertex3f(1.0,2.8,0.0);
	glVertex3f(0.0,4.0,0.0);
	glVertex3f(-1.0,2.8,0.0);
	glVertex3f(-1.3,1.8,0.0);
	glVertex3f(-1.0,0.7,0.0);
	glEnd();
	
	glBegin(GL_POLYGON);
	glVertex3f(0.0,0.0,0.1);
	glVertex3f(1.0,0.7,0.1);
	glVertex3f(1.3,1.8,0.1);
	glVertex3f(1.0,2.8,0.1);
	glVertex3f(0.0,4.0,0.1);
	glVertex3f(-1.0,2.8,0.1);
	glVertex3f(-1.3,1.8,0.1);
	glVertex3f(-1.0,0.7,0.1);
	glEnd();
}

void drawBranch(void) {
	/* ADD YOUR CODE to make the 2D branch a 3D extrusion */
	glColor3f(0.54,0.27,0.07); 
	glBegin(GL_POLYGON);
	glVertex2f(1.0,0.0);
	glVertex2f(1.0,6.0);
	glVertex2f(-1.0,6.0);
	glVertex2f(-1.0,0.0);
	glEnd();
}

/*
 * Draws the plant.
 *
 * ADD YOUR CODE and modify the function to take an L-system depth and
 * any other necessary arguments.
 */
void drawPlant(void) {

	/* Load a hard-coded rotation matrix of -30 degrees about positive z */
	/* This matrix is only here as an example, and can be removed 
	load2DMatrix(
	       sqrt(3.0)/2.0, -1.0/2.0,      0.0,
		   1.0/2.0,       sqrt(3.0)/2.0, 0.0,
		   0.0,           0.0,           1.0);

	
	 * The location of the leaf and branch will not look right until
	 * transformation matrices are implmented.
	 */
	scale(3, 3);
	rotatez(90);
	drawLeaf();

	drawBranch();
}

void zero_mat(GLfloat* vec, int dim){
     for(int i=0; i<dim; ++i){
     	vec[i] = 0;
     }
}


void mat_multiplyv(GLfloat* matrix, GLfloat* vector, int dim, GLfloat* result ){
   zero_mat(result, dim);
   for(int row=0; row<dim; ++row){
   	for(int col=0; col<dim; ++col){
	 result[row] += matrix[(row*dim+col)] * vector[col];
	}
   }
}

void mat_multiplym(GLfloat* m1, GLfloat* m2, int dim, GLfloat* result ){
   zero_mat(result, dim);
   for(int i=0; i < dim; i++){
     for(int row=0; row<dim; ++row){
   	for(int col=0; col<dim; ++col){
	 result[i*dim+row] += m1[(row*dim+col)] * m2[col*dim + row];
	}
   }
   }
}

void print_mat(GLfloat* mat, int dim){
     std::cout << "[ ";
     for(int i=0; i<dim; ++i){
      for(int j=0; j<dim; ++j){
     	std::cout << mat[(i*dim+j)] << " ";
	}
     std::cout << std::endl;
    }
    std:: cout << "] ";
     std::cout << std::endl;
}

void print_vec(GLfloat* vec, int dim){
     std::cout << "[ ";
     for(int i=0; i<dim; ++i){
     	std::cout << vec[i] << " ";
     }
    std:: cout << "] ";
     std::cout << std::endl;
}

/* end of drawplant.c */
