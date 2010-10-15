#ifndef _DRAWPLANT_H_
#define _DRAWPLANT_H_

const GLfloat ident[16] = {1.0, 0, 0, 0, 
                                0, 1.0, 0, 0,
                                0, 0, 1.0, 0,
                                0, 0, 0, 1.0};

/* Functions implemented in drawplant.cpp */

void drawPlant(void);
void zero_mat(GLfloat* vec, int dim);
void print_mat(GLfloat* mat, int dim);
void print_vec(GLfloat* mat, int dim);
void mat_multiplyv(GLfloat* matrix, GLfloat* vector, int dim, GLfloat* result );
void mat_multiplym(GLfloat* m1, GLfloat* m2, int dim, GLfloat* result );
void mat_copy(GLfloat* m1, const GLfloat* m2, int dim = 4);
void push();
void pop();
void initMatrixStack ();

#endif	/* _DRAWPLANT_H_ */
