#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "glutGameLibs.h"
#include "InitJPG.h"


void myinit(void)
{
	glClearColor(0,0,0,0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
}


void keyboard(unsigned int key)
{

}


void world()
{

}

int main( int argc, char * argv[])
{
	//backend Glut init
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(1920, 1080);
	glutCreateWindow("JORDY DE HOON - TORENKRAAN");
	//GlutGame Engine init
	myinit();
	glutGameInit();
	glutGameKeyboardSetFunc(keyboard);
	glutGameControlEnable();
	glutGameRenderSceneSet(world);
	//Start all services and run engine
	glutGameMainLoop();
}
