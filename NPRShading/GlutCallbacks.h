#ifndef _GLUT_CALLBACKS_H_
#define _GLUT_CALLBACKS_H_

void display();
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);

void mouse(int button, int state, int x, int y);
void motion(int x, int y);

#endif
