/*
* Shading
*
* Ivory and gooch shading demo
*
* 2016 Yuzo(Huang Yuzhong)
*
*/

#ifdef  _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif //  _WIN32

#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "../common/textfile.hpp"
#include "../common/shader.hpp"

#ifdef _WIN32
#include <GL/wglew.h>
#pragma comment(lib, "Opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#else
#include <GL/glxew.h>
#endif

using namespace std;

int windowWidth = 800;
int windowHeight = 600;
float lightDiffuse[3] = { 0.95, 0.95, 0.95 };  /* White */
float lightAngle = -0.4;   /* Angle light rotates around scene. */
float time = 0.0;  /* Timing of bulge. */
GLuint outlineProgram;

void initOutline();
void onDisplay();
void onIdle();
void onReshape(int width, int height);
void onKeyboard(unsigned char key, int x, int y);

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("Outline");
	glCheckError();

	if (glewInit() != GLEW_OK) fprintf(stderr, "Failed to initialize GLEW");
	if (!glewIsSupported("GL_VERSION_2_0")) fprintf(stderr, "OpenGL 2.0 not supported");
	glCheckError();

#ifdef _WIN32
	wglSwapIntervalEXT(-1);
#else
	glXSwapIntervalEXT(-1);
#endif

	glutDisplayFunc(onDisplay);
	glutIdleFunc(onIdle);
	glutReshapeFunc(onReshape);
	glutKeyboardFunc(onKeyboard);

	initOutline();
	glCheckError();
	glUseProgram(outlineProgram);
	glCheckError();

	// setup status used in mainloop
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();

	return EXIT_SUCCESS;
}

void initOutline() {
	char* outlineCode[] = { textFileRead("shader/outline.vert"), textFileRead("shader/outline.frag") };
	GLenum outlineType[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
	outlineProgram = glBuildProgram(outlineCode, outlineType, 2);

	delete[] outlineCode[0];
	delete[] outlineCode[1];
	glCheckError();
}

void onDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(-3.0, -4.0, 9.0,
		0.0, 0.0, -1.0,
		0.0f, 1.0f, 0.0f);
	glRotatef(time * 5, 1, 1, 1);
	glutSolidTeapot(3);
	glutSwapBuffers();
}

void onIdle() {
	static float lightVelocity = 0.008;
	static float timeFlow = 0.01;
	static const double myPi = 3.14159265358979323846;

	/* Repeat rotating light around front 180 degrees. */
	if (lightAngle > myPi / 2) {
		lightAngle = myPi / 2;
		lightVelocity = -lightVelocity;
	} else if (lightAngle < -myPi / 2) {
		lightAngle = -myPi / 2;
		lightVelocity = -lightVelocity;
	}
	lightAngle += lightVelocity;  /* Add a small angle (in radians). */

								  /* Repeatedly advance and rewind time. */
	if (time > 10) {
		time = 10;
		timeFlow = -timeFlow;
	} else if (time < 0) {
		time = 0;
		timeFlow = -timeFlow;
	}
	time += timeFlow;  /* Add time delta. */

	glutPostRedisplay();
}

void onReshape(int width, int height) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (height == 0)
		height = 1;

	float ratio = 1.0* width / height;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, width, height);

	// Set the correct perspective.
	gluPerspective(45, ratio, 1, 1000);
	glMatrixMode(GL_MODELVIEW);

}
void onKeyboard(unsigned char key, int x, int y) {

}