#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>

#include "GlutCommon.h"
#include "GlutInterface.h"
#include "GlutCallbacks.h"

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
    static float lightPos1[] = { 5.0f, 5.0f, 5.0f, 0.0f };
    static float lightPos2[] = { -5.0f, -5.0f, 5.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos2);

	// オブジェクトの描画
	quate = quate.rotate(angle, axis[0], axis[1], axis[2]);
	quate.setToMatrix(modelMatrix, offset);
	glLoadIdentity();
	glMultMatrixd(modelMatrix);
	gluProject(0, 0, 0, modelMatrix, projMatrix, viewport, &origin[0], &origin[1], &origin[2]);

	glPushMatrix();
	glScaled(zoom, zoom, zoom);

    // 輪郭の描画
    if(drawEdge) {
        if(edgeProgramId == UNDEF_PROGRAM) {
            edgeProgramId = setGlsl("edge.vert", "edge.frag");
        } else {
            glUseProgram(edgeProgramId);
        }

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glPolygonMode(GL_FRONT, GL_LINE);
		glLineWidth(sqrt(zoom));
        glutSolidTeapot(1.0);
        glDisable(GL_CULL_FACE);
    }

    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0, 1.0);
    glUseProgram(shadeProgramId);

    // ハイライトの描画
    GLint uvId = glGetUniformLocation(shadeProgramId, "drawHighlight");
    glUniform1i(uvId, (GLint)drawHighlight);
    
    glutSolidTeapot(1.0);
    glDisable(GL_POLYGON_OFFSET_FILL);
	
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

void saveCurrentImageBuffer(string filename)
{
    uchar* pixelData = new uchar[WIN_WIDTH * WIN_HEIGHT * 3];
    glReadPixels(0, 0, WIN_WIDTH, WIN_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, pixelData);
    
    cv::Mat I(WIN_HEIGHT, WIN_WIDTH, CV_8UC3);
    for(int y=0; y<WIN_HEIGHT; y++) {
        for(int x=0; x<WIN_WIDTH; x++) {
            for(int c=0; c<3; c++) {
                I.at<uchar>(y, x*3+c) = pixelData[((WIN_HEIGHT-y-1)*WIN_WIDTH+x)*3+(2-c)];
            }
        }
    }
    cv::imwrite(filename, I);
    delete[] pixelData;
}


void GlutInterface::keyboard(unsigned char key, int x, int y)
{
    string vertFile = "glsl.vert";
    string fragFile = "glsl.frag";
    string savepath;
	switch(key) {
	case '0':
		vertFile = "glsl.vert";
        fragFile = "glsl.frag";
		break;
	case '1':
		vertFile = "glsl.vert";
        fragFile = "normal.frag";
		break;

	case '2':
        vertFile = "glsl.vert";
        fragFile = "gooch.frag";
		break;

    case '3':
        vertFile = "glsl.vert";
        fragFile = "rademacher.frag";
        break;

    case 'e':
        drawEdge = !drawEdge;
        glutPostRedisplay();
        return;

    case 'h':
        drawHighlight = !drawHighlight;
        glutPostRedisplay();
        return;

    case 's':
        cout << "save as: ";
        cin >> savepath;
        saveCurrentImageBuffer(savepath);
        cout << "saved successfully !!" << endl;
        return;

	case 0x1b:
		exit(-1);
		break;

    default:
        return;
	}

    shadeProgramId = setGlsl(vertFile, fragFile);
    glutPostRedisplay();
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
