#ifndef _DRAWPLANT_H_
#define _DRAWPLANT_H_

const GLfloat ident[16] = {1.0, 0, 0, 0, 
                                0, 1.0, 0, 0,
                                0, 0, 1.0, 0,
                                0, 0, 0, 1.0};

/* Functions implemented in drawplant.cpp */

void drawPlant(int, float, float);
void drawTree(int i);
void drawLeaf(int i);
void drawBranch(int i, GLfloat*);
void zero_mat(GLfloat* vec, int dim);
void print_mat(GLfloat* mat, int dim);
void print_vec(GLfloat* mat, int dim);
void mat_multiplyv(GLfloat* matrix, GLfloat* vector, int dim, GLfloat* result );
void mat_multiplym(GLfloat* m1, GLfloat* m2, int dim, GLfloat* result );
void mat_copy(GLfloat* m1, const GLfloat* m2, int dim = 4);
void vec_copy(GLfloat* v1, const GLfloat* v2, int dim = 4);
void push();
void pop();
void initMatrixStack ();
void translate(GLfloat x, GLfloat y, GLfloat z = 0.0);
void scale(GLfloat x, GLfloat y, GLfloat z = 1.0);
void rotatez(GLfloat angle);
void rotatey(GLfloat angle);
void testRotate();
void load3DMatrix(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);

void load3DMatrixp(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);

void loadIdentity();
#endif	/* _DRAWPLANT_H_ */
