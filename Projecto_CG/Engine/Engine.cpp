#define _USE_MATH_DEFINES
#include <math.h>
#include "SceneReader.h"
#include <stdlib.h>
#include <GL/glut.h>
#include <stdio.h>

 char* scene;

 //camera control
 static float alfa = 0, beta = 0, raio = 15, k = 0.02;

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(raio*cos(beta)*sin(alfa), raio*sin(beta), raio*cos(beta)*cos(alfa),
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);

	// put the geometric transformations here


	// put drawing instructions here
		
	drawScene(scene);

	// End of frame
	glutSwapBuffers();
}



// write function to process keyboard events
void keyboardSpecial(int key, int x, int y) {
	switch (key) {

	case GLUT_KEY_UP:
		if (beta < (M_PI / 2 - k))
			beta += k;
		break;

	case GLUT_KEY_DOWN:
		if (beta > -(M_PI / 2 - k))
			beta -= k;
		break;

	case GLUT_KEY_LEFT:
		alfa -= k;
		break;

	case GLUT_KEY_RIGHT:
		alfa += k;
		break;

	}
	glutPostRedisplay();
}

void KeyBoard(unsigned char key, int x, int y) {
	switch (key) {
		case '+':
			raio -= 0.5;
			break;
			
		case '-':
			raio += 0.5;
			break;
	}	
	glutPostRedisplay();
}


// write function to process menu events




int main(int argc, char **argv) {


	scene = argv[1];
	// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Projecto de CG");
	glutIdleFunc(renderScene);


	// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutSpecialFunc(keyboardSpecial);
	glutKeyboardFunc(KeyBoard);

	// put here the registration of the keyboard and menu callbacks



	// put here the definition of the menu 


	readScene(scene);

	//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	

	// enter GLUT's main cycle
	glutMainLoop();

	return 1;
}
