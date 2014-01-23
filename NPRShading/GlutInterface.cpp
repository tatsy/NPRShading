#include <iostream>
#include <fstream>
using namespace std;

#include "GlutInterface.h"
#include "GlutCallbacks.h"
#include "GlutCommon.h"

// extern �ϐ��Q
double zoom            =  1.0;
double depth           = -50.0;
double angle           =  0.0;
double axis[3]         =  {0.0, 1.0, 0.0};
double offset[3]       =  {0};
int    viewport[4]     =  {0};
double modelMatrix[16] =  {0};
double projMatrix[16]  =  {0};
Quaternion quate;

int    mouseButton;
bool   isPress;
int    prevX;
int    prevY;

// GLSL�p�v���O����
GLuint shadeProgramId = UNDEF_PROGRAM;
GLuint edgeProgramId  = UNDEF_PROGRAM;

// �`���Ԃ�\���ϐ�
bool drawEdge      = false;
bool drawHighlight = false;

// Main�Œ�`���ׂ�extern�ϐ��Q
double origin[3] = { 0.0, 0.0, 0.0 };
double eye[3] = { 10.0, 10.0, 10.0 };
const int WIN_WIDTH  = 640;
const int WIN_HEIGHT = 480;

// glew�p�̕ϐ�
GLuint vShaderId;
GLuint fShaderId;
GLuint programId;

GlutInterface::GlutInterface()
{
}

GlutInterface::GlutInterface(int* argcp, char** argv)
{
	glutInit(argcp, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	glutCreateWindow("GLUT Interface");

	glClearColor(0.5, 0.5, 0.5, 1);

	// �r���[�̏�����
	offset[0] = 0.0;
	offset[1] = 0.0;
	offset[2] = depth;

	quate = Quaternion::identity() * zoom;
	quate = quate.rotate(0.0, 1.0, 0.0, 0.0).rotate(0.0, 0.0, 1.0, 0.0).rotate(0.0, 0.0, 0.0, 1.0);
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	glGetIntegerv(GL_VIEWPORT, viewport);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

	glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);

	glEnable(GL_SMOOTH);
	glShadeModel(GL_SMOOTH);

    glewInit();
    if(!glewIsSupported("GL_VERSION_2_0")) {
        cerr << "Cannot use GLSL" << endl;
        exit(-1);
    }
    shadeProgramId = setGlsl("glsl.vert", "glsl.frag");

	description();
	
	glutMainLoop();
}

GlutInterface::GlutInterface(const GlutInterface& glutInterface)
{
}

GlutInterface& GlutInterface::operator=(const GlutInterface& glutInterface)
{
}

GlutInterface& GlutInterface::getInstance(int* argcp, char** argv)
{
	static GlutInterface singleton(argcp, argv);
	return singleton;
}

void printShaderLogInfo(GLuint shader) {
	GLsizei bufSize;

	/* �V�F�[�_�̃R���p�C�����̃��O�̒������擾���� */
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH , &bufSize);
	if (bufSize > 1) {
		GLchar *infoLog = (GLchar *)malloc(bufSize);

		if (infoLog != NULL) {
			GLsizei length;

			/* �V�F�[�_�̃R���p�C�����̃��O�̓��e���擾���� */
			glGetShaderInfoLog(shader, bufSize, &length, infoLog);
			fprintf(stderr, "InfoLog:\n%s\n\n", infoLog);
			free(infoLog);
		}
		else
			fprintf(stderr, "Could not allocate InfoLog buffer.\n");
	}
}

// setup glsl
GLuint GlutInterface::setGlsl(string vertFile, string fragFile) {
	GLint compiled, linked;

	// get glsl version
	printf("%s\n", glGetString(GL_VERSION));
	printf("%s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    // vertex shader
    vShaderId = glCreateShader(GL_VERTEX_SHADER);
    ifstream ifVert(vertFile);
	if(!ifVert.is_open()) {
		cerr << "Failed to open vertex shader program !!" << endl;
		exit(-1);
	}
    istreambuf_iterator<char> vdataBegin(ifVert);
    istreambuf_iterator<char> vdataEnd;
    string vertData(vdataBegin, vdataEnd);
    const char* vfile = vertData.c_str();
    ifVert.close();

    glShaderSource(vShaderId, 1, &vfile, 0);
    glCompileShader(vShaderId);
	glGetShaderiv(vShaderId, GL_COMPILE_STATUS, &compiled);
	if(compiled == GL_FALSE) {
		cerr << "Vertex shader compile error!!" << endl;
		printShaderLogInfo(vShaderId);
	}

    // fragment shader
    fShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    ifstream ifFrag(fragFile);
	if(!ifFrag.is_open()) {
		cerr << "Failed to open fragment shader program !!" << endl;
		exit(-1);
	}
    istreambuf_iterator<char> fdataBegin(ifFrag);
    istreambuf_iterator<char> fdataEnd;
    string fragData(fdataBegin, fdataEnd);
    const char* ffile = fragData.c_str();
    ifFrag.close();

    glShaderSource(fShaderId, 1, &ffile, 0);
    glCompileShader(fShaderId);
	glGetShaderiv(fShaderId, GL_COMPILE_STATUS, &compiled);
	if(compiled == GL_FALSE) {
		cerr << "Fragment shader compile error!!" << endl;
		printShaderLogInfo(fShaderId);
	}

    // create program object
    programId = glCreateProgram();
    glAttachShader(programId, vShaderId);
    glAttachShader(programId, fShaderId);

    glLinkProgram(programId);
    glGetProgramiv(programId, GL_LINK_STATUS, &linked);

	if(linked == GL_FALSE) {
        cerr << "Link error !!" << endl;

		GLsizei msgLen;
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &msgLen);
		if(msgLen > 0) {
			char* logBuf = new char[msgLen];
			GLsizei length;
			glGetProgramInfoLog(programId, msgLen, &length, logBuf);
			printf("%s\n", logBuf);
			delete[] logBuf;
		}
    }
    glUseProgram(programId);
    return programId;
}

