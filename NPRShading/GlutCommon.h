#ifndef _GLUT_COMMON_H_
#define _GLUT_COMMON_H_

#include <string>

#include "Quaternion.hpp"

// extern 変数群
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

// vertexシェーダ用のファイル
extern std::string vertFile;

// Mainで定義すべきextern変数群
extern double origin[3];
extern double eye[3];
extern const int WIN_WIDTH;
extern const int WIN_HEIGHT;

#endif
