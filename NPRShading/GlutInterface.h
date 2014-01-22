#ifndef _GLUT_INTERFACE_H_
#define _GLUT_INTERFACE_H_

class GlutInterface {
public:
	static GlutInterface& getInstance(int* argcp, char** argv);

private:
	GlutInterface();
	GlutInterface(int* argcp, char** argv);
	GlutInterface(const GlutInterface& glutInterface);
	GlutInterface& operator=(const GlutInterface& glutInterface);

	// 使用方法関数
	static void description();

	// glsl用関数
	static void setGlsl();

	// callback関数群
	static void display();
	static void reshape(int w, int h);
	static void keyboard(unsigned char key, int x, int y);
};

#endif


