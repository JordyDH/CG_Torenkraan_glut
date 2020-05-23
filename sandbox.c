#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "glutGameLibs.h"
#include "InitJPG.h"
#include "obj_torenkraan.h"

#define MAX 3
static Kraan_struct *kranen[MAX];
static Kraan_struct *kraan_select;

void myinit(void)
{
	glClearColor(0,0,0,0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
}


void keyboard(unsigned int key)
{
	//With 1/2/3 a specific crane can be selected
	if(key == '1')
	{
		printf("Kraan 1 geselecteerd\n");
		kraan_select = kranen[0];
	}
	if(key == '2')
	{
		printf("Kraan 2 geselecteerd\n");
		kraan_select = kranen[1];
	}
	if(key == '3')
	{
		printf("Kraan 3 geselecteerd\n");
		kraan_select = kranen[2];
	}
	if(key == 'r')
		(*(*kraan_select).obj_disk).rot_y += 1;
	if(key == 'R')
		(*(*kraan_select).obj_disk).rot_y -= 1;
	if(key == 'g')
		(*(*kraan_select).obj_last).lf_mem[0] += 0.1;
	if(key == 'G')
		(*(*kraan_select).obj_last).lf_mem[0] -= 0.1;
	if(key == 'y')
		(*(*kraan_select).obj_last).lf_mem[1] += 0.1;
	if(key == 'Y')
		(*(*kraan_select).obj_last).lf_mem[1] -= 0.1;
}


void world()
{
	//Render light points
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glutGameRenderAllLights();
	//Debug axis
	glLineWidth(3);
	glBegin(GL_LINES);
		glColor3f(1,0,0);
		glVertex3f(0,0,0);
		glVertex3f(3,0,0);
		glColor3f(0,1,0);
		glVertex3f(0,0,0);
		glVertex3f(0,3,0);
		glColor3f(0,0,1);
		glVertex3f(0,0,0);
		glVertex3f(0,0,3);
	glEnd();

	//Render static elements (floors)

	//Render objecten
	glutGameRenderAllObjects();
	glDisable(GL_LIGHTING);
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
	//Define objects in render list
	kranen[0] = torenkraan_AllocObj(0,0,0,5,10);
	glutGameObjectlight *light_0 = glutGameObjectsAlloc_light();
	float color[4] = {1.0,1.0,1.0,0.0};
	(*light_0).ambient  = color;
	(*light_0).diffuse  = color;
	(*light_0).specular = color;
	(*light_0).id = GL_LIGHT0;
	(*light_0).enable = 1;
	(*light_0).z = 10;
	(*light_0).y = 5;
	//default selected crane
	kraan_select = kranen[0];
	//Start all services and run engine
	glutGameMainLoop();
}
