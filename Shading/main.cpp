/*
* Shading
*
* Shading demo
*
* 2016 Yuzo(Huang Yuzhong)
*
*/

#define _USE_MATH_DEFINES
#ifdef  _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif //  _WIN32

#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include "../common/textfile.hpp"
#include "../common/shader.hpp"

#ifdef _WIN32
#include <GL/wglew.h>
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#else
#include <GL/glxew.h>
#endif

using namespace std;
using namespace glm;

GLint windowWidth = 800;
GLint windowHeight = 600;
GLchar windowTitle[32] = "Shading";
GLint frameCount = 0;
GLint nextTime = 0, nowTime = 0, lastTime = 0;
GLfloat lightAmbient[4] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat lightDiffuse[4] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat lightSpecular[4] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat lightPosition[4] = { -3.0, 4.0, 5.0, 1.0 };
GLfloat materialAmbient[4] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat materialDiffuse[4] = { 0.8, 0.8, 0.8, 1.0 };
GLfloat materialSpecular[4] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat materialEmission[4] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat materialShiness = 128;
GLdouble cameraEye[3] = { 3.0, 4.0, 5.0 };
GLdouble cameraCenter[3] = { 0.0, 0.0, 0.0 };
GLdouble cameraUp[3] = { 0.0, 1.0, 0.0 };
bool spinEnabled = true;
GLint spinTime = 0;
GLfloat spinSpeed = 0.01;
GLfloat spinVector[3] = { -1.0, 1.0, 1.0 };
GLuint ivoryProgram, goochProgram, programId = 0;
GLuint displayList[9];
GLuint helperList[2];
int displayIndex = 0;
int buttonId, clickX, clickY;
GLfloat rotateSpeed = 0.1, translateSpeed = 0.1, scaleSpeed = 0.01, scrollSpeed = 0.1;

void buildIvory();
void buildGooch();
void compileDisplay();
void onDisplay();
void onIdle();
void onReshape(int width, int height);
void onKeyboard(unsigned char key, int x, int y);
void onSpecial(int key, int x, int y);
void onMouse(int button, int state, int x, int y);
void onMotion(int x, int y);
void onMouseWheel(int button, int direction, int x, int y);

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow(windowTitle);
	glCheckError();

	if (glewInit() != GLEW_OK) fprintf(stderr, "Failed to initialize GLEW");
	if (!glewIsSupported("GL_VERSION_2_0")) fprintf(stderr, "OpenGL 2.0 not supported");
	glCheckError();

#ifdef _WIN32
	wglSwapIntervalEXT(-1);
#else
	glXSwapIntervalEXT(-1);
#endif

	buildIvory();
	buildGooch();
	compileDisplay();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, materialEmission);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &materialShiness);

	glutDisplayFunc(onDisplay);
	glutIdleFunc(onIdle);
	glutReshapeFunc(onReshape);
	glutKeyboardFunc(onKeyboard);
	glutSpecialFunc(onSpecial);
	glutMouseFunc(onMouse);
	glutMotionFunc(onMotion);
	glutMouseWheelFunc(onMouseWheel);

	gluLookAt(cameraEye[0], cameraEye[1], cameraEye[2], cameraCenter[0], cameraCenter[1], cameraCenter[2], cameraUp[0], cameraUp[1], cameraUp[2]);

	glutMainLoop();

	return EXIT_SUCCESS;
}

void buildIvory() {
	char* ivoryCode[] = { textFileRead("shader/ivory.vert"), textFileRead("shader/ivory.frag") };
	GLenum ivoryType[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
	ivoryProgram = glBuildProgram(ivoryCode, ivoryType, 2);

	delete[] ivoryCode[0];
	delete[] ivoryCode[1];
	glCheckError();
}

void buildGooch() {
	char* goochCode[] = { textFileRead("shader/gooch.vert"), textFileRead("shader/gooch.frag") };
	GLenum goochType[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
	goochProgram = glBuildProgram(goochCode, goochType, 2);

	delete[] goochCode[0];
	delete[] goochCode[1];
	glCheckError();
}

void compileDisplay() {
	int base = glGenLists(9);
	for (int i = 0; i < 9; i++) {
		glNewList(base + i, GL_COMPILE);
		switch (i) {
		case 0:
			glutSolidCube(2);
			break;
		case 1:
			glutSolidSphere(2, 24, 30);
			break;
		case 2:
			glutSolidCone(2, 4, 24, 30);
			break;
		case 3:
			glutSolidTorus(1, 2, 24, 30);
			break;
		case 4:
			glutSolidDodecahedron();
			break;
		case 5:
			glutSolidOctahedron();
			break;
		case 6:
			glutSolidTetrahedron();
			break;
		case 7:
			glutSolidIcosahedron();
			break;
		case 8:
			glutSolidTeapot(2);
			break;
		}
		glEndList();
		displayList[i] = base + i;
	}

	base = glGenLists(2);
	for (int i = 0; i < 2; i++) {
		glNewList(base + i, GL_COMPILE);
		switch (i) {
			case 0:
				glBegin(GL_LINES);
				glColor3f(1.0, 0.0, 0.0);
				glVertex3f(0.0, 0.0, 0.0);
				glVertex3f(1000.0, 0.0, 0.0);
				glColor3f(0.0, 1.0, 0.0);
				glVertex3f(0.0, 0.0, 0.0);
				glVertex3f(0.0, 1000.0, 0.0);
				glColor3f(0.0, 0.0, 1.0);
				glVertex3f(0.0, 0.0, 0.0);
				glVertex3f(0.0, 0.0, 1000.0);
				glColor3f(1.0, 1.0, 1.0);
				glEnd();
				break;
			case 1:
				glutSolidSphere(0.1, 24, 30);
				break;
		}
		glEndList();
		helperList[i] = base + i;
	}
}

void onDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();

	nextTime = glutGet(GLUT_ELAPSED_TIME);
	if (spinEnabled) spinTime += nextTime - nowTime;
	nowTime = nextTime;
	glRotatef(spinTime * spinSpeed, spinVector[0], spinVector[1], spinVector[2]);

	glUseProgram(programId);
	glCallList(displayList[displayIndex]);
	glUseProgram(0);

	glDisable(GL_LIGHTING);
	glCallList(helperList[0]);

	glPushMatrix();
	glTranslatef(lightPosition[0], lightPosition[1], lightPosition[2]);
	glCallList(helperList[1]);
	glPopMatrix();

	glEnable(GL_LIGHTING);

	glPopMatrix();
	glutSwapBuffers();

	if (++frameCount >= 60) {
		snprintf(windowTitle, 32, "Shading - FPS: %.2f", frameCount / ((nowTime - lastTime) / 1000.0));
		glutSetWindowTitle(windowTitle);
		lastTime = nowTime;
		frameCount = 0;
	}
}

void onIdle() {
	glutPostRedisplay();
}

void onReshape(int width, int height) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);
	gluPerspective(45, width / GLdouble(height), 0.1, 1000);
	glMatrixMode(GL_MODELVIEW);
}

void onKeyboard(unsigned char key, int x, int y) {
	if (key >= '1' && key <= '9') {
		displayIndex = key - '1';
	} else {
		switch (key) {
			case 's':
				spinEnabled = !spinEnabled;
				break;
			case 'r':
				spinTime = 0;
				break;
			case 'i':
				programId = ivoryProgram;
				break;
			case 'g':
				programId = goochProgram;
				break;
			case 'n':
				programId = 0;
				break;
		}
	}
}

void onSpecial(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_RIGHT:
			lightPosition[0] += 0.1;
			break;
		case GLUT_KEY_LEFT:
			lightPosition[0] -= 0.1;
			break;
		case GLUT_KEY_UP:
			lightPosition[2] -= 0.1;
			break;
		case GLUT_KEY_DOWN:
			lightPosition[2] += 0.1;
			break;
		case GLUT_KEY_PAGE_UP:
			lightPosition[1] += 0.1;
			break;
		case GLUT_KEY_PAGE_DOWN:
			lightPosition[1] -= 0.1;
			break;
	}
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

void doMotion(int x, int y) {
	vec2 vector(x - clickX, y - clickY);
	switch (buttonId) {
		case GLUT_LEFT_BUTTON:
			vector *= rotateSpeed;
			glRotatef(length(vector), vector.x, vector.y, 0);
			break;
		case GLUT_MIDDLE_BUTTON:
			vector *= translateSpeed;
			glTranslatef(vector.x, vector.y, 0);
			break;
		case GLUT_RIGHT_BUTTON:
			vector *= scaleSpeed;
			GLfloat ratio = length(vector) + 1;
			if (vector.x + vector.y < 0) ratio = 1.0 / ratio;
			glScalef(ratio, ratio, ratio);
			break;
	}
}

void onMouse(int button, int state, int x, int y) {
	switch (state) {
		case GLUT_DOWN:
			buttonId = button;
			clickX = x;
			clickY = y;
			glPushMatrix();
			break;
		case GLUT_UP:
			glPopMatrix();
			doMotion(x, y);
			break;
	}
}

void onMotion(int x, int y) {
	glPopMatrix();
	glPushMatrix();
	doMotion(x, y);
}

void onMouseWheel(int button, int direction, int x, int y) {
	if (direction > 0) {
		GLfloat ratio = 1 + scrollSpeed;
		glScalef(ratio, ratio, ratio);
	} else {
		GLfloat ratio = 1 - scrollSpeed;
		glScalef(ratio, ratio, ratio);
	}
}