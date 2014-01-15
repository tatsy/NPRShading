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
};

#endif


