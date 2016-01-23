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
float lightColor[3] = { 0.95, 0.95, 0.95 };  /* White */
float lightAngle = -0.4;   /* Angle light rotates around scene. */
float time = 0.0;  /* Timing of bulge. */
GLuint subdivisionProgram;

void initSubdivision();
void onDisplay();
void onIdle();
void onReshape(int width, int height);
void onKeyboard(unsigned char key, int x, int y);

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("subdivision");
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

	initSubdivision();
	glCheckError();
	glUseProgram(subdivisionProgram);
	glCheckError();

	// setup status used in mainloop
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();

	return EXIT_SUCCESS;
}

void initSubdivision() {
	char* subdivisionCode[] = { textFileRead("shader/subdivision.vert"), textFileRead("shader/subdivision.geom"), textFileRead("shader/subdivision.frag") };
	GLenum subdivisionType[] = { GL_VERTEX_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER };
	subdivisionProgram = glBuildProgram(subdivisionCode, subdivisionType, 3);

	glUseProgram(subdivisionProgram);
	glUniform1i(glGetUniformLocation(subdivisionProgram, "FpLevel"), 3);
	glUniform1f(glGetUniformLocation(subdivisionProgram, "Radius"), 1.5);
	glUseProgram(0);

	delete[] subdivisionCode[0];
	delete[] subdivisionCode[1];
	delete[] subdivisionCode[2];
	glCheckError();
}

float a = 0;
void onDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 5.0,
		0.0, 0.0, -1.0,
		0.0f, 1.0f, 0.0f);

	glRotatef(a, 0, 1, 1);
	glBegin(GL_TRIANGLES);
	glVertex3i(0, 0, 2);
	glVertex3i(2, 0, 0);
	glVertex3i(0, 2, 0);
	glEnd();
	a += 0.05;
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