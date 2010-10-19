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
#include <time.h>

#include <iostream>
#include "Matrix.h"
#include "common.h"
#include "drawplant.h"
#include "readppm.h"

#include <stack>

#define PI 3.14159265

std::stack<GLfloat*> matrixStack;
static GLfloat curMatrix[16];
static int scale_level = -1;

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

void load3DMatrix(GLfloat* temp)
{
    load3DMatrix(temp[0], temp[1], temp[2], temp[3],
                    temp[4], temp[5], temp[6], temp[7],
                    temp[8], temp[9], temp[10], temp[11],
                    temp[12], temp[13], temp[14], temp[15]);
}
void initMatrixStack ()
{               
    GLfloat* temp = new GLfloat[16];
    mat_copy(temp, ident);
    matrixStack.push(temp);
    load3DMatrix(temp[0], temp[1], temp[2], temp[3],
                    temp[4], temp[5], temp[6], temp[7],
                    temp[8], temp[9], temp[10], temp[11],
                    temp[12], temp[13], temp[14], temp[15]);
                    
    //curMatrix = new GLfloat[16];
    mat_copy(curMatrix, ident);
    //std::cout<<"inits"<<std::endl;
    //print_mat(curMatrix, 4);
    //delete[] temp;
}

void push()
{
    GLfloat* pushMatrix = new GLfloat[16];
    mat_copy(pushMatrix, curMatrix);
    matrixStack.push(pushMatrix);
    //delete[] pushMatrix;
}

void pop()
{
    GLfloat* temp = matrixStack.top();
    mat_copy(curMatrix, temp);
    matrixStack.pop(); 
    load3DMatrix(curMatrix);
    delete[] temp;
}

void loadIdentity()
{
    mat_copy(curMatrix, ident);
    load3DMatrix(curMatrix);
}

void translate(GLfloat x, GLfloat y, GLfloat z){

	GLfloat t_mat[16] = {
		1.0 , 0.0, 0.0, x,
		0.0, 1.0, 0.0, y, 
		0.0, 0.0, 1.0, z,
		0.0, 0.0, 0.0, 1.0};
		
    GLfloat result[16];
    mat_multiplym(t_mat, curMatrix, 4, result);
    mat_copy(curMatrix, result);
    load3DMatrix(curMatrix);
}

void scale(GLfloat x, GLfloat y, GLfloat z){
	GLfloat s_mat[16] = {
		x , 0.0, 0.0, 0.0,
		0.0, y, 0.0, 0.0, 
		0.0, 0.0, z, 0.0,
		0.0, 0.0, 0.0, 1.0};
		
    GLfloat result[16];
    mat_multiplym(s_mat, curMatrix, 4, result);
    mat_copy(curMatrix, result);
    load3DMatrix(curMatrix);
}

void rotatez(GLfloat angle){
        GLfloat deg = angle*PI/180.0;
	GLfloat r_mat[16] = {
		cos(deg) , - sin(deg), 0.0, 0.0,
		sin(deg),    cos(deg), 0.0, 0.0, 
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0};
		
    GLfloat result[16];
    mat_multiplym(r_mat, curMatrix, 4, result);
    mat_copy(curMatrix, result);
    load3DMatrix(curMatrix);
}


void rotatex(GLfloat angle){
        GLfloat deg = angle*PI/180.0;
	GLfloat r_mat[16] = {
		1.0, 0.0, 0.0, 0.0, 
		0.0, cos(deg), -sin(deg), 0.0, 
		0.0, sin(deg), cos(deg), 0.0,
		0.0, 0.0, 0.0, 1.0};
    GLfloat result[16];
    mat_multiplym(r_mat, curMatrix, 4, result);
    mat_copy(curMatrix, result);
    load3DMatrix(curMatrix);
}

void rotatey(GLfloat angle){
        GLfloat deg = angle*PI/180.0;
	GLfloat r_mat[16] = {
		cos(deg) , 0.0 ,  sin(deg), 0.0, 
		0.0,    1.0, 0.0, 0.0, 
		- sin (deg), 0.0, cos (deg), 0.0,
		0.0, 0.0, 0.0, 1.0};
    GLfloat result[16];
    mat_multiplym(r_mat, curMatrix, 4, result);
    mat_copy(curMatrix, result);
    load3DMatrix(curMatrix);
}

void rotateAtOriginx(GLfloat angle)
{
    GLfloat temp[16];
    mat_copy(temp, curMatrix);
    loadIdentity();
    
    rotatex(angle);
    
    GLfloat holder[16];
    mat_copy(holder, curMatrix);
    
    mat_multiplym(temp, holder, 4, curMatrix);
    load3DMatrix(curMatrix);
}

void rotateAtOriginy(GLfloat angle)
{
    GLfloat temp[16];
    mat_copy(temp, curMatrix);
    loadIdentity();
    
    rotatey(angle);
    
    GLfloat holder[16];
    mat_copy(holder, curMatrix);
    
    mat_multiplym(temp, holder, 4, curMatrix);
    load3DMatrix(curMatrix);
}

void rotateAtOriginz(GLfloat angle)
{
    GLfloat temp[16];
    mat_copy(temp, curMatrix);
    loadIdentity();
    
    rotatez(angle);
    
    GLfloat holder[16];
    mat_copy(holder, curMatrix);
    
    mat_multiplym(temp, holder, 4, curMatrix);
    load3DMatrix(curMatrix);
}

void testRotate()
{
    rotatey(5);
    rotatez(5);
}

GLfloat getRandAngle(GLfloat angle)
{
    static int m = 1;
    srand( (unsigned)time( NULL ) * m * PI);
    ++m;
    GLfloat r = ((GLfloat)rand()) / ((GLfloat)RAND_MAX);
    r *= 2;
    r -= 1;
    r *= angle;
    return r;
}
void drawLeaf(int i) {
	/* ADD YOUR CODE to make the 2D leaf a 3D extrusion */
	
	if (i == 0)
    {
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
	}
	else
	{
	    GLfloat vec[4] = {2, 6, 0, 0};
	    drawBranch(i - 1, vec); 
	    
	    push();
	    rotatez(20);
	    drawLeaf(i - 1);
	    pop();
	    
	    push();
	    translate(-vec[0], -vec[1], -vec[2]);
	    rotatez(-20);
	    translate(vec[0], vec[1], vec[2]);
	    drawLeaf(i - 1);
	    pop();
	}
}

void drawLeaf3D (int i)
{
    std::cout<<"drawLeaf3D("<<i<<")"<<std::endl;
    GLfloat vertexList[] = 
    {
        // bottom
        0.0, 0.0, 0.0, 
        1.0, 0.7,0.0, 
        1.3, 1.8, 0.0, 
        1.0, 2.8, 0.0, 
        0.0, 4.0, 0.0, 
        -1.0, 2.8,0.0, 
        -1.3, 1.8, 0.0, 
        -1.0, 0.7, 0.0, 
        // top
        -1.0, 0.7, 1.0, 
        -1.3, 1.8, 1.0, 
        -1.0, 2.8,1.0,
        0.0, 4.0,1.0,
        1.0, 2.8, 1.0, 
        1.3, 1.8,1.0,
        1.0, 0.7,1.0, 
        0.0, 0.0,1.0,
        
    };
    
    int indexList[] = 
    {
        0, 1, 14, 15,
        1, 2, 13, 14,
        2, 3, 12, 13,
        3, 4, 11, 12,
        4, 5, 10, 11,
        5, 6, 9, 10,
        6, 7, 8, 9,
        7, 0, 15, 8
    };
    
    // draw bottom
	glColor3f(0.1,.9,0.1); 
    glBegin(GL_POLYGON);
    glColor3f(.5,.9,0.1); 
    glVertex3f(vertexList[0], vertexList[1], vertexList[2]);
    glColor3f(.7,.9,0.1); 
    glVertex3f(vertexList[3], vertexList[4], vertexList[5]);
    glColor3f(.6,.9,0.1); 
    glVertex3f(vertexList[6], vertexList[7], vertexList[8]);
    glColor3f(.6,.8,0.1); 
    glVertex3f(vertexList[9], vertexList[10], vertexList[11]);
    glColor3f(.6,.9,0.1); 
    glVertex3f(vertexList[12], vertexList[13], vertexList[14]);
    glColor3f(.7,.9,0.1); 
    glVertex3f(vertexList[15], vertexList[16], vertexList[17]);
    glColor3f(.5,.8,0.1); 
    glVertex3f(vertexList[18], vertexList[19], vertexList[20]);
    glColor3f(.7,.9,0.1); 
    glVertex3f(vertexList[21], vertexList[22], vertexList[23]);
	glEnd();
	
    // draw top
	glBegin(GL_POLYGON);
	glColor3f(.7,.9,0.1); 
    glVertex3f(vertexList[24], vertexList[25], vertexList[26]);
	glColor3f(.5,.8,0.1); 
    glVertex3f(vertexList[27], vertexList[28], vertexList[29]);
    glColor3f(.7,.9,0.1); 
    glVertex3f(vertexList[30], vertexList[31], vertexList[32]);
    glColor3f(.6,.9,0.1); 
    glVertex3f(vertexList[33], vertexList[34], vertexList[35]);
    glColor3f(.6,.8,0.1); 
    glVertex3f(vertexList[36], vertexList[37], vertexList[38]);
    glColor3f(.6,.9,0.1); 
	glVertex3f(vertexList[39], vertexList[40], vertexList[41]);
	glColor3f(.5,.9,0.1); 
	glVertex3f(vertexList[42], vertexList[43], vertexList[44]);
	glColor3f(.7,.9,0.1); 
    glVertex3f(vertexList[45], vertexList[46], vertexList[47]);
	glEnd();
	
	for(int q = 0; q < 8; ++q)
	{
	    int ind0 = indexList[q * 4];
	    int ind1 = indexList[q * 4 + 1];
	    int ind2 = indexList[q * 4 + 2];
	    int ind3 = indexList[q * 4 + 3];
	    
	    glBegin(GL_QUADS);
	    glColor3f( .1 + q * .07, .9, .1);
	    glVertex3f(vertexList[ind0 * 3], vertexList[ind0 * 3 + 1], vertexList[ind0 * 3 + 2]);
	    glColor3f( .1 + q * .08, .8, .1);
	    glVertex3f(vertexList[ind1 * 3], vertexList[ind1 * 3 + 1], vertexList[ind1 * 3 + 2]);
	    glColor3f( .1 + q * .09, .9, .1);
	    glVertex3f(vertexList[ind2 * 3], vertexList[ind2 * 3 + 1], vertexList[ind2 * 3 + 2]);
	    glColor3f( .1 + q * .1, .8, .1);
	    glVertex3f(vertexList[ind3 * 3], vertexList[ind3 * 3 + 1], vertexList[ind3 * 3 + 2]);
	    glEnd();
	}
}

void drawFruit3D(GLfloat redLevel)
{
    GLfloat vertexList[] = {
        1.0, 1.0, 0.0, //top
        1.0, 0.0, 1.0, // top diamond
        0.0, 0.0, 0.0, // left diamond
        1.0, 0.0, -1.0, //bot diamond
        2.0, 0.0, 0.0, //right diamond
        1.0, -1.0, 0.0 //bot
    };
    
    int indexList[] = {
        0, 4, 1,
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        5, 1, 4,
        5, 2, 1,
        5, 3, 2,
        5, 4, 3
    };
    
    for (int q = 0; q < 8; ++q)
    {
        //glColor3f( (q%3 == 0 ? 1 : 0), (q%3 == 1 ? 1 : 0), (q%3 == 2 ? 1 : 0));
        glColor3f(redLevel + q * .03, (1 - redLevel) + q * .03, .10);
        int index0 = indexList[q * 3];
        int index1 = indexList[q * 3 + 1];
        int index2 = indexList[q * 3 + 2];
        
        std::cout<<index0<<" "<<index1<<" "<<index2<<std::endl;
        //print_vec(&vertexList[index0], 3);
        //print_vec(&vertexList[index1], 3);
        //print_vec(&vertexList[index2], 3);
        glBegin(GL_TRIANGLES);
        glVertex3f(vertexList[index0 * 3], vertexList[index0 * 3 + 1], vertexList[index0 * 3 + 2]);
        glVertex3f(vertexList[index1 * 3], vertexList[index1 * 3 + 1], vertexList[index1 * 3 + 2]);
        glVertex3f(vertexList[index2 * 3], vertexList[index2 * 3 + 1], vertexList[index2 * 3 + 2]);
        glEnd();
    }
}

void drawFlower3D()
{
    GLfloat vertexList[] = {
        1.0, 1.0, 0.0, //top
        1.0, 0.0, 1.0, // top diamond
        0.0, 0.0, 0.0, // left diamond
        1.0, 0.0, -1.0, //bot diamond
        2.0, 0.0, 0.0, //right diamond
        1.0, -1.0, 0.0 //bot
    };
    
    int indexList[] = {
        0, 4, 1,
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        5, 1, 4,
        5, 2, 1,
        5, 3, 2,
        5, 4, 3
    };
    
    for (int q = 0; q < 8; ++q)
    {
        //glColor3f( (q%3 == 0 ? 1 : 0), (q%3 == 1 ? 1 : 0), (q%3 == 2 ? 1 : 0));
        glColor3f(.5, .50 + q * .03, 0.0);
        int index0 = indexList[q * 3];
        int index1 = indexList[q * 3 + 1];
        int index2 = indexList[q * 3 + 2];
        
        std::cout<<index0<<" "<<index1<<" "<<index2<<std::endl;
        //print_vec(&vertexList[index0], 3);
        //print_vec(&vertexList[index1], 3);
        //print_vec(&vertexList[index2], 3);
        glBegin(GL_TRIANGLES);
        glVertex3f(vertexList[index0 * 3], vertexList[index0 * 3 + 1], vertexList[index0 * 3 + 2]);
        glVertex3f(vertexList[index1 * 3], vertexList[index1 * 3 + 1], vertexList[index1 * 3 + 2]);
        glVertex3f(vertexList[index2 * 3], vertexList[index2 * 3 + 1], vertexList[index2 * 3 + 2]);
        glEnd();
    }
}

void drawBranch(int i, GLfloat* vec) {
	/* ADD YOUR CODE to make the 2D branch a 3D extrusion */
	if (i == 0)
	{
	    glColor3f(0.54,0.27,0.07); 
	    glBegin(GL_POLYGON);
	    glVertex2f(1.0,0.0);
	    glVertex2f(1.0,6.0);
	    glVertex2f(-1.0,6.0);
	    glVertex2f(-1.0,0.0);
	    glEnd();
	    
	    translate(vec[0], vec[1], vec[2]);
    }
    else
    {
        //grow();
        drawBranch(i - 1, vec);
    }
}

void drawBranch3D(GLfloat height, GLfloat botArea, GLfloat topArea)
{
    glColor3f(0.54,0.27,0.07); 
    std::cout << "drawBranch3D(" << height << " , " << botArea << ", " << topArea << ")" << std::endl;
	GLfloat botSideLen = sqrt(botArea);
	GLfloat topSideLen = sqrt(topArea);
    GLfloat vertexList[] = { 
         botSideLen/2, 0, botSideLen/2,
         botSideLen/2, 0, -botSideLen/2,
         -botSideLen/2, 0, -botSideLen/2,
         -botSideLen/2, 0, botSideLen/2,

         topSideLen/2, height, topSideLen/2,
         -topSideLen/2, height, topSideLen/2,
         -topSideLen/2, height, -topSideLen/2,
         topSideLen/2, height, -topSideLen/2
	 };
    GLfloat indexList[] = {
     	  0, 4, 7, 1,
	  1, 7, 6, 2, 
	  2, 6, 5 , 3, 
	  3, 5, 4, 0
    };

    glBegin(GL_QUADS);
    
    glVertex3f( vertexList[0], vertexList[1], vertexList[2]);
    glVertex3f( vertexList[3], vertexList[4], vertexList[5]);
    glVertex3f( vertexList[6], vertexList[7], vertexList[8]);
    glVertex3f( vertexList[9], vertexList[10], vertexList[11]);
    
    glEnd();
        
    glBegin(GL_QUADS);
    
    glVertex3f( vertexList[12], vertexList[13], vertexList[14]);
    glVertex3f( vertexList[15], vertexList[16], vertexList[17]);
    glVertex3f( vertexList[18], vertexList[19], vertexList[20]);
    glVertex3f( vertexList[21], vertexList[22], vertexList[23]);
    
    glEnd();
    for(int q = 0; q < 4; ++q)
	{
	    glColor3f(.68 - q * .1, .40 - q * .1, .07);
	    int ind1 = indexList[q * 4];
	    int ind2 = indexList[q * 4 + 1];
	    int ind3 = indexList[q * 4 + 2];
	    int ind4 = indexList[q * 4 + 3];

	    glBegin(GL_QUADS);
	    glVertex3f(vertexList[ind1 * 3], vertexList[ind1 * 3 + 1], vertexList[ind1 * 3 + 2]);
	    glVertex3f(vertexList[ind2 * 3], vertexList[ind2 * 3 + 1], vertexList[ind2 * 3 + 2]);
	    glVertex3f(vertexList[ind3 * 3], vertexList[ind3 * 3 + 1], vertexList[ind3 * 3 + 2]);
	    glVertex3f(vertexList[ind4 * 3], vertexList[ind4 * 3 + 1], vertexList[ind4 * 3 + 2]);
	    glEnd();
	}
}

void drawBranch3DSmooth(GLfloat height, GLfloat botRad, GLfloat topRad, int base_tri)
{
    double theta = 2 * PI / base_tri;
    
    GLfloat colorVertex[] = {
        139.0 / 256.0, 69.0 / 256.0, 19.0 / 256.0, // these three for the outside
        255.0 / 256.0, 127.0 / 256.0, 36.0 / 256.0,
        160.0 / 256.0, 82.0 / 256.0, 45.0 / 256.0,
        205.0 / 256.0, 133.0 / 256.0, 63.0 / 256.0 // for the inside
    };
    
    int botColorInd = 0;
    int topColorInd = 2;
    
    for (int q = 0; q < base_tri; ++q)
    {
        GLfloat* c0;
        GLfloat* c1;
        GLfloat* c2;
        GLfloat* c3;
    
        // begin drawing base
        glBegin(GL_TRIANGLES);
        glColor3fv(&colorVertex[3 * 3]);
        glVertex3f(0, 0, 0);
        
        c0 = &colorVertex[ botColorInd * 3 ];
        glColor3fv( c0 );
        GLfloat quad0[] = {botRad * cos(q * theta), 0, botRad * sin(q * theta)}; 
        glVertex3fv( quad0 );
        
        //botColorInd = botColorInd == 0 ? 1 : 0;
        
        c1 = &colorVertex[ botColorInd * 3 ];
        glColor3fv( c1 );
        GLfloat quad1[] = {botRad * cos( (q + 1)  * theta), 0, botRad * sin( (q + 1) * theta)};
    	glVertex3fv( quad1 );
        glEnd();

        // begin drawing top
        glBegin(GL_TRIANGLES);
        glColor3fv(&colorVertex[3 * 3]);
        glVertex3f(0, height, 0);
        
        c3 = &colorVertex[ topColorInd * 3 ];
        glColor3fv( c3 );
        GLfloat quad3[] = {topRad * cos(q * theta), height, topRad * sin(q * theta)};
        glVertex3fv( quad3 );
        
        //topColorInd = topColorInd == 2 ? 1 : 2;
        
        c2 = &colorVertex[ topColorInd * 3 ];
        glColor3fv( c2 );
        GLfloat quad2[] = {topRad * cos( (q + 1)  * theta), height, topRad * sin( (q + 1) * theta)};
    	glVertex3fv( quad2 );
        glEnd();      
        
        glBegin(GL_QUADS);
        glColor3fv(c0);
        glVertex3fv(quad0);
        
        glColor3fv(c1);
        glVertex3fv(quad1);
        
        glColor3fv(c2);
        glVertex3fv(quad2);
        
        glColor3fv(c3);
        glVertex3fv(quad3);
        glEnd();
    }
}


void drawNextTopGrowth(int i, GLfloat xangle, GLfloat zangle)
{
    push();
    rotateAtOriginz(zangle);
    rotateAtOriginx(xangle);
    drawRecBranch(i -1, 0);
    //print_mat(curMatrix, 4);
    push();
    drawRecLeaf(i - 1);
    pop();
    pop();
}

void drawNextSideGrowth(int i, GLfloat xangle, GLfloat zangle)
{
    push();
    rotateAtOriginz(zangle);
    rotateAtOriginx(xangle);
    drawRecLeaf(i - 1);
    pop();
}

void drawRecBranch (int i, int growth)
{
    std::cout<<"drawRecBranch("<<i<<")"<<std::endl;
    if (i == 0)
    {
        GLfloat s = 1.0 + growth * 1.25;
        //drawBranch3D(6.0 * s, 4.0 * s, 2.0 * s * 1.25);
        drawBranch3DSmooth(6.0 * s, .5 * s, .4 * s, 16);
        
        // move the turtle along the correct vector
        GLfloat fullBranchVec[] = {0, 6.0 * s, 0, 0};
        GLfloat curBranchVec[] = {0, 0, 0, 0};
        mat_multiplyv(curMatrix, fullBranchVec, 4, curBranchVec);
        //vec_copy(fullBranchVec, curBranchVec, 4);

        translate(curBranchVec[0], curBranchVec[1], curBranchVec[2]);
    }
    else
    {
        drawRecBranch(i - 1, growth + 1);
    }
}

void drawRecLeaf (int i)
{
    std::cout<<"drawRecLeaf("<<i<<")"<<std::endl;
    if (i == 0)
    {
        push();
        //translate(cos(xangle * PI / 180.0) * 2, 0.0, sin(zangle * PI / 180.0) * 2);
        drawFruit3D(1);
        pop();
        push();
        drawLeaf3D(0);
        pop();
    }
    else
    {
        drawRecBranch(i - 1, 0);
        drawNextTopGrowth(i, 0, 10);
        drawNextSideGrowth(i, 15, 0);
        drawNextSideGrowth(i, -10, -35);
        drawNextSideGrowth(i, 0, 40);
        drawNextTopGrowth(i, -25, 20);
        drawNextTopGrowth(i, -25, 0);
        
    }
}

void drawTree(int i)
{
    //initMatrixStack();
    push();
    drawLeaf(i);
    pop();
}

void drawTree3D(int i)
{
    drawRecLeaf(i);
}

/*
 * Draws the plant.
 *
 * ADD YOUR CODE and modify the function to take an L-system depth and
 * any other necessary arguments.
 */
void drawPlant(int i, float roty, float rotz, int step) {
    std::cout<<"drawPlant("<<i<<", "<<roty<<", "<<step<<")"<<std::endl;

    push();
    translate(0, -40, 0);
    
    // this rotation takes care of the global rotation
    rotateAtOriginy(roty);
    rotateAtOriginz(rotz);
    //rotatey(roty);
#if 1
    drawTree3D(step);
#else
    for (int q = 0; q <= 10; ++q)
    {
        push();
        translate(-5 + q * 2, -5 + q * 2, 0);
        //drawFruit3D(q * .1);
        std::cout<<q * .1<<std::endl;
        pop();
    }
    
    //drawBranch3DSmooth(height, GLfloat botRad, GLfloat topRad, int base_tri)
    drawBranch3DSmooth(25, 10, 5, 16);
    
    //drawFlower3D();
#endif
    pop();
}



#if 1
void zero_mat(GLfloat* vec, int dim){
     for(int i=0; i<dim*dim; ++i){
     	vec[i] = 0.0;
     }
}


void mat_multiplyv(GLfloat* matrix, GLfloat* vector, int dim, GLfloat* result ){
    for (int q = 0; q < dim; ++q)
        result[q] = 0;
    
    for(int row=0; row<dim; ++row)
        for(int col=0; col<dim; ++col)
            result[row] += matrix[row * dim + col] * vector[col];
}

void mat_multiplym(GLfloat* m1, GLfloat* m2, int dim, GLfloat* result ){
    zero_mat(result, dim);
    //print_mat(result, 4);
    //print_mat(m1, 4);
    //print_mat(m2, 4);
    
    for(int r_row = 0; r_row < dim; ++r_row)
        for(int r_col=0; r_col < dim; ++r_col)
            for(int k = 0; k < dim; ++k)
                result[r_row * dim + r_col] += m1[r_row * dim + k] * m2[k * dim + r_col];
                
    //print_mat(result, 4);
}

void mat_copy(GLfloat* m1, const GLfloat* m2, int dim)
{
    for(int i=0; i<dim*dim; ++i){
        m1[i] = m2[i];
    }
}

void vec_copy(GLfloat* v1, const GLfloat* v2, int dim)
{
    for(int i=0; i<dim; ++i){
        v1[i] = v2[i];
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
#endif
/* end of drawplant.c */
