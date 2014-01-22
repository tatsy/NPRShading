#ifndef _GLUT_COMMON_H_
#define _GLUT_COMMON_H_

#include <string>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Quaternion.hpp"

// extern �ϐ��Q
extern double zoom;
extern double depth;
extern double angle;
extern double offset[3];
extern double axis[3];
extern int    viewport[4];
extern double modelMatrix[16];
extern double projMatrix[16];
extern Quaternion quate;

extern int    mouseButton;
extern bool   isPress;
extern int    prevX;
extern int    prevY;

// GLSL�̃v���O����ID
const GLuint UNDEF_PROGRAM = 0xffff;
extern GLuint shadeProgramId;
extern GLuint edgeProgramId;

// �`���Ԃ�\���ϐ�
extern bool drawEdge;

// Main�Œ�`���ׂ�extern�ϐ��Q
extern double origin[3];
extern double eye[3];
extern const int WIN_WIDTH;
extern const int WIN_HEIGHT;

#endif
