#include <iostream>
using namespace std;

#include <GL/freeglut.h>
#include "GlutInterface.h"
#include "GlutCallbacks.h"
#include "GlutCommon.h"

void GlutInterface::description() 
{
	cout << " ** Glut Viewer **" << endl;
	cout << "  [0] gold material (default) " << endl;
	cout << "  [1] normal map " << endl;
	cout << "  [2] NPR Shading by Gooch et al. 98 " << endl;
	cout << endl;
}

void GlutInterface::display() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set light position
    static float lightPos[] = { 5.0f, 5.0f , 5.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	// オブジェクトの描画
	quate = quate.rotate(angle, axis[0], axis[1], axis[2]);
	quate.setToMatrix(modelMatrix, offset);
	glLoadIdentity();
	glMultMatrixd(modelMatrix);
	gluProject(0, 0, 0, modelMatrix, projMatrix, viewport, &origin[0], &origin[1], &origin[2]);

	glPushMatrix();
	glScaled(zoom, zoom, zoom);

	glutSolidTeapot(1.0);
	//glutSolidSphere(1.0, 128, 128);
	
	glPopMatrix();

	glutSwapBuffers();
}

void GlutInterface::reshape(int width, int height) 
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(20.0, (double)width / height, 1, 100);

	glMatrixMode(GL_MODELVIEW);
}

void GlutInterface::keyboard(unsigned char key, int x, int y)
{
	switch(key) {
	case '0':
		vertFile = "glsl.vert";
		setGlsl();
		glutPostRedisplay();
		break;
	case '1':
		vertFile = "normal.vert";
		setGlsl();
		glutPostRedisplay();
		break;

	case '2':
		vertFile = "gooch.vert";
		setGlsl();
		glutPostRedisplay();
		break;

	case 0x1b:
		exit(-1);
		break;
	}
}

void mouse(int button, int state, int x, int y) {
	if(button == GLUT_LEFT_BUTTON || button == GLUT_MIDDLE_BUTTON || button == GLUT_RIGHT_BUTTON) {
		mouseButton = button;
		if(state == GLUT_DOWN) {
			isPress = true;
			prevX = x;
			prevY = y;
		}

		if(state == GLUT_UP) {
			isPress = false;
		}
	}
}

void motion(int x, int y) {
	static double transRatio = 5.0;
	static double rotRatio   = 50.0;
	if(isPress) {
		int dx = x - prevX;
		int dy = y - prevY;

		if(mouseButton == GLUT_LEFT_BUTTON) {
			angle = hypot(dx, dy) * rotRatio / min(WIN_WIDTH, WIN_HEIGHT);
			gluUnProject(origin[0] + dy, origin[1] + dx, origin[2], modelMatrix, projMatrix, viewport, &axis[0], &axis[1], &axis[2]);
		}

		else if(mouseButton == GLUT_MIDDLE_BUTTON) {
			offset[0] += dx * transRatio / WIN_HEIGHT;
			offset[1] -= dy * transRatio / WIN_HEIGHT;
		}

		else if(mouseButton == GLUT_RIGHT_BUTTON) {
			zoom += dy / 100.0;
		}

		prevX = x;
		prevY = y;
		glutPostRedisplay();
	}
}
