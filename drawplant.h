#ifndef _DRAWPLANT_H_
#define _DRAWPLANT_H_

/* Functions implemented in drawplant.cpp */

void drawPlant(void);
void  mat_multiplyv(GLfloat *[4], GLfloat*, int , GLfloat*);
void mat_multiplym(GLfloat **, GLfloat** , int, GLfloat** );
void zero_vec(GLfloat* vec, int dim);
void zero_mat(GLfloat** mat, int dim);
void print_mat(GLfloat** mat, int dim);
void print_vec(GLfloat* mat, int dim);

#endif	/* _DRAWPLANT_H_ */
