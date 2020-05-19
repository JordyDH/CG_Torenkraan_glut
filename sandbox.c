#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <GL/glut.h>
#include "glutGameLibs.h"
#include "InitJPG.h"

int togle = 0;
void kubus();
glutGameObjectobject *reff_obj;
uint64_t systick = 0;
uint16_t points = 300;

glutGameObjectobject *obj1;
glutGameObjectobject *obj2;
glutGameObjectobject *obj3;

void myinit(void)
{
	glClearColor(0,0,0,0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);

}

void cube(glutGameObjectobject *self)
{
	glLineWidth(3);
	glColor3f(1,1,1);
	glutSolidCube(1);
	glColor3f(0,0,0);
	glutWireCube(1);
}

void keyboard(unsigned int key)
{
	if( key == '5')
	{
		(*obj1).x = 0;
		(*obj1).z = 0;
		(*obj1).rot_x = 0;
		(*obj1).rot_y = 0;
	}
	if( key == '8')
		(*obj1).x += 0.5;
	if( key == '2')
		(*obj1).x -= 0.5;
	if( key == '4')
		(*obj1).z += 0.5;
	if( key == '6')
		(*obj1).z -= 0.5;
	if( key == '7')
		(*obj1).rot_x += 5;
	if( key == '9')
		(*obj1).rot_x -= 5;
	if( key == '1')
		(*obj1).rot_y += 5;
	if( key == '3')
		(*obj1).rot_y -= 5;

}


void world()
{
	glBegin(GL_LINES);
		glColor3f(1,0,0);
		glVertex3f(0,0,0);
		glVertex3f(10,0,0);
		glColor3f(0,1,0);
		glVertex3f(0,0,0);
		glVertex3f(0,10,0);
		glColor3f(0,0,1);
		glVertex3f(0,0,0);
		glVertex3f(0,0,10);
	glEnd();
	glutGameRenderAllObjects();
}

int main( int argc, char * argv[])
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(1920, 1080);
	glutCreateWindow("SANDOX GLUTGAME");
	myinit();

	glutGameInit();
	glutGameKeyboardSetFunc(keyboard);
	glutGameControlEnable();
	glutGameRenderSceneSet(world);

	obj1 = glutGameObjectsAlloc_object();
	obj2 = glutGameObjectsAlloc_object();
	obj3 = glutGameObjectsAlloc_object();

	(*obj1).callback = cube;

	(*obj2).callback = cube;
	(*obj2).linkedobj = obj1;
	(*obj2).y = 1;

	(*obj3).callback = cube;
	(*obj3).linkedobj = obj2;
	(*obj3).y = 1;

	glutGameMainLoop();
}
