#ifndef _DRAWPLANT_H_
#define _DRAWPLANT_H_

/* Functions implemented in drawplant.cpp */

void drawPlant(void);
void zero_mat(GLfloat* vec, int dim);
void print_mat(GLfloat* mat, int dim);
void print_vec(GLfloat* mat, int dim);
void mat_multiplyv(GLfloat* matrix, GLfloat* vector, int dim, GLfloat* result );
void mat_multiplym(GLfloat* m1, GLfloat* m2, int dim, GLfloat* result );

#endif	/* _DRAWPLANT_H_ */
