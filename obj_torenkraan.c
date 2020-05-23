// JORDY DE HOON
// OBJECT FUNCTIES VOOR TEKENEN EN PARAMETRISEREN VAN EEN KRAAN
// WERKT OP GLUTGAME DEV0.3

#include <stdio.h>
#include <stdlib.h>
#include "obj_torenkraan.h"
// ----------------------------------------------------------------------------------
#define TK_KOLOM_STUDSIZE	0.05
#define TK_KOLOM_WIDTH		2.0
#define TK_KOLOM_HEIGHT		2.0
// ----------------------------------------------------------------------------------
#define TK_DISK_OSIZE	2.0
#define TK_DISK_ISIZE	0.2
#define TK_DISK_SLICES	20
// ----------------------------------------------------------------------------------
#define TK_CAB_WIDTH	2.0
#define TK_CAB_HEIGHT	1.5
#define TK_CAB_DEPTH1	1.5
#define TK_CAB_DEPTH0	1.0
// ----------------------------------------------------------------------------------
// Local prototyping for internal function
void renderBalk(double x0, double y0, double z0, double x1, double y1, double z1);

Kraan_struct* torenkraan_AllocObj(double x, double y, double z, uint16_t height)
{
	Kraan_struct *kraan_obj = (Kraan_struct*)calloc(1,sizeof(Kraan_struct));
	glutGameObjectobject *obj;
	glutGameObjectobject *obj_prev;
	//Draw the koloms ---------------------------------------------------------
	for(uint16_t i = 0; i < height; i++)
	{
		// Alloc mem space for object
		obj = glutGameObjectsAlloc_object();
		// Set callback function to draw body
		(*obj).callback = torenkraan_kolom;
		// If first element : parent
		if(i == 0)
		{
			(*obj).x = x;
			(*obj).y = y;
			(*obj).z = z;
			(*kraan_obj).obj_kolom = obj;
		}
		//Child of previous object
		else
		{
			//Shift the height
			(*obj).y = TK_KOLOM_HEIGHT;
			//Set Parent object
			(*obj).linkedobj = obj_prev;
		}
		//track previous object
		obj_prev = obj;
	}
	//Draw disks ---------------------------------------------------------
	for(uint16_t i = 0 ; i < 2; i++)
	{
		obj = glutGameObjectsAlloc_object();
		(*obj).callback = torenkraan_disk;
		if(i == 0) (*obj).y = TK_KOLOM_HEIGHT;
		else       (*obj).y = 0.05;
		(*obj).linkedobj = obj_prev;
		obj_prev = obj;
	}
	// Store last disk object, used for rotation.
	(*kraan_obj).obj_disk = obj_prev;
	//Draw kabine ---------------------------------------------------------
	obj = glutGameObjectsAlloc_object();
	(*obj).callback = torenkraan_kabine;
	(*obj).linkedobj = obj_prev;
	obj_prev = obj;
	return kraan_obj;
}

void torenkraan_kolom(glutGameObjectobject *self)
{
	//Set materials

	//Body of object
	for(uint16_t i = 0; i < 4; i++)
	{
		glPushMatrix();
		glRotatef(90*i,0,1,0);
		//Draw box
		glTranslatef(TK_KOLOM_WIDTH/2, 0, TK_KOLOM_WIDTH/2);
		renderBalk(-(TK_KOLOM_STUDSIZE/2),0,-(TK_KOLOM_STUDSIZE/2),(TK_KOLOM_STUDSIZE/2),TK_KOLOM_HEIGHT,(TK_KOLOM_STUDSIZE/2));
		renderBalk(-(TK_KOLOM_STUDSIZE/2),0,-(TK_KOLOM_STUDSIZE/2),-TK_KOLOM_WIDTH+(TK_KOLOM_STUDSIZE/2),TK_KOLOM_STUDSIZE,(TK_KOLOM_STUDSIZE/2));
		renderBalk(-(TK_KOLOM_STUDSIZE/2),TK_KOLOM_HEIGHT-TK_KOLOM_STUDSIZE,-(TK_KOLOM_STUDSIZE/2),-TK_KOLOM_WIDTH+(TK_KOLOM_STUDSIZE/2),TK_KOLOM_HEIGHT,(TK_KOLOM_STUDSIZE/2));
		//Support material
		//Only draw 4 faces (2 obsolete, less vertexes to render, better for laurel server)
		glColor3f(1,1,1);
		glBegin(GL_QUADS);
			glVertex3d(TK_KOLOM_STUDSIZE/2, TK_KOLOM_HEIGHT, -TK_KOLOM_STUDSIZE/2);
			glVertex3d(TK_KOLOM_STUDSIZE/2, TK_KOLOM_HEIGHT-TK_KOLOM_STUDSIZE, -TK_KOLOM_STUDSIZE/2);
			glVertex3d(TK_KOLOM_STUDSIZE/2, 0, -TK_KOLOM_WIDTH-TK_KOLOM_STUDSIZE/2);
			glVertex3d(TK_KOLOM_STUDSIZE/2, TK_KOLOM_STUDSIZE, -TK_KOLOM_WIDTH-TK_KOLOM_STUDSIZE/2);

			glVertex3d(-TK_KOLOM_STUDSIZE/2, TK_KOLOM_HEIGHT, -TK_KOLOM_STUDSIZE/2);
			glVertex3d(-TK_KOLOM_STUDSIZE/2, TK_KOLOM_HEIGHT-TK_KOLOM_STUDSIZE, -TK_KOLOM_STUDSIZE/2);
			glVertex3d(-TK_KOLOM_STUDSIZE/2, 0, -TK_KOLOM_WIDTH-TK_KOLOM_STUDSIZE/2);
			glVertex3d(-TK_KOLOM_STUDSIZE/2, TK_KOLOM_STUDSIZE, -TK_KOLOM_WIDTH-TK_KOLOM_STUDSIZE/2);

			glVertex3d(TK_KOLOM_STUDSIZE/2,  TK_KOLOM_HEIGHT,   -TK_KOLOM_STUDSIZE/2);
			glVertex3d(TK_KOLOM_STUDSIZE/2,  TK_KOLOM_STUDSIZE, -TK_KOLOM_WIDTH-TK_KOLOM_STUDSIZE/2);
			glVertex3d(-TK_KOLOM_STUDSIZE/2, TK_KOLOM_STUDSIZE, -TK_KOLOM_WIDTH-TK_KOLOM_STUDSIZE/2);
			glVertex3d(-TK_KOLOM_STUDSIZE/2, TK_KOLOM_HEIGHT,   -TK_KOLOM_STUDSIZE/2);

			glVertex3d(TK_KOLOM_STUDSIZE/2,  TK_KOLOM_HEIGHT-TK_KOLOM_STUDSIZE,   -TK_KOLOM_STUDSIZE/2);
			glVertex3d(TK_KOLOM_STUDSIZE/2,  0, -TK_KOLOM_WIDTH-TK_KOLOM_STUDSIZE/2);
			glVertex3d(-TK_KOLOM_STUDSIZE/2, 0, -TK_KOLOM_WIDTH-TK_KOLOM_STUDSIZE/2);
			glVertex3d(-TK_KOLOM_STUDSIZE/2, TK_KOLOM_HEIGHT-TK_KOLOM_STUDSIZE,   -TK_KOLOM_STUDSIZE/2);
		glEnd();
		glPopMatrix();
	}
}

void torenkraan_disk(glutGameObjectobject *self)
{
	//Set materials

	//Body of object
	glPushMatrix();
	glRotatef(90,1,0,0);
	GLUquadric* quad = gluNewQuadric();
	gluDisk(quad,TK_DISK_ISIZE, TK_DISK_OSIZE, TK_DISK_SLICES, 1);
	glPopMatrix();
}

void torenkraan_kabine(glutGameObjectobject *self)
{
	//self parameter to control the opacity of the windows
	//Set materials ---------------------------------------------------

	//Body of object --------------------------------------------------
	//Translate to center object
	glPushMatrix();
	glTranslatef(-TK_CAB_WIDTH/2,0,-TK_CAB_DEPTH0/2);
	glBegin(GL_QUADS);
		//BACK
		glVertex3d(0,0,0);
		glVertex3d(TK_CAB_WIDTH,0,0);
		glVertex3d(TK_CAB_WIDTH,TK_CAB_HEIGHT,0);
		glVertex3d(0,TK_CAB_HEIGHT,0);
		//SIDE
		glVertex3d(0,0,0);
		glVertex3d(0,TK_CAB_HEIGHT,0);
		glVertex3d(0,TK_CAB_HEIGHT,TK_CAB_DEPTH1);
		glVertex3d(0,0,TK_CAB_DEPTH0);
		//OTHER SIDE
		glVertex3d(TK_CAB_WIDTH,0,0);
		glVertex3d(TK_CAB_WIDTH,TK_CAB_HEIGHT,0);
		glVertex3d(TK_CAB_WIDTH,TK_CAB_HEIGHT,TK_CAB_DEPTH1);
		glVertex3d(TK_CAB_WIDTH,0,TK_CAB_DEPTH0);
		//TOP
		glVertex3d(0,TK_CAB_HEIGHT,0);
		glVertex3d(TK_CAB_WIDTH,TK_CAB_HEIGHT,0);
		glVertex3d(TK_CAB_WIDTH,TK_CAB_HEIGHT,TK_CAB_DEPTH1);
		glVertex3d(0,TK_CAB_HEIGHT,TK_CAB_DEPTH1);
	glEnd();
	glLineWidth(3);
	glColor3f(0,0,0);
	glBegin(GL_LINES);
		//BACK
		glVertex3d(0,0,0);
		glVertex3d(TK_CAB_WIDTH,0,0);
		glVertex3d(TK_CAB_WIDTH,TK_CAB_HEIGHT,0);
		glVertex3d(0,TK_CAB_HEIGHT,0);
		//SIDE
		glVertex3d(0,0,0);
		glVertex3d(0,TK_CAB_HEIGHT,0);
		glVertex3d(0,TK_CAB_HEIGHT,TK_CAB_DEPTH1);
		glVertex3d(0,0,TK_CAB_DEPTH0);
		//OTHER SIDE
		glVertex3d(TK_CAB_WIDTH,0,0);
		glVertex3d(TK_CAB_WIDTH,TK_CAB_HEIGHT,0);
		glVertex3d(TK_CAB_WIDTH,TK_CAB_HEIGHT,TK_CAB_DEPTH1);
		glVertex3d(TK_CAB_WIDTH,0,TK_CAB_DEPTH0);
		//TOP
		glVertex3d(0,TK_CAB_HEIGHT,0);
		glVertex3d(TK_CAB_WIDTH,TK_CAB_HEIGHT,0);
		glVertex3d(TK_CAB_WIDTH,TK_CAB_HEIGHT,TK_CAB_DEPTH1);
		glVertex3d(0,TK_CAB_HEIGHT,TK_CAB_DEPTH1);
	glEnd();
	glPopMatrix();
}


void renderBalk(double x0, double y0, double z0, double x1, double y1, double z1)
{
	glColor3f(1,1,1);
	glBegin(GL_QUADS);
		glVertex3d(x0, y0, z0);
		glVertex3d(x1 ,y0 ,z0);
		glVertex3d(x1 ,y1 ,z0);
		glVertex3d(x0 ,y1 ,z0);

		glVertex3d(x0, y0, z1);
		glVertex3d(x1 ,y0 ,z1);
		glVertex3d(x1 ,y1 ,z1);
		glVertex3d(x0 ,y1 ,z1);

		glVertex3d(x0, y0, z0);
		glVertex3d(x0 ,y1 ,z0);
		glVertex3d(x0 ,y1 ,z1);
		glVertex3d(x0 ,y0 ,z1);

		glVertex3d(x1, y0, z0);
		glVertex3d(x1 ,y1 ,z0);
		glVertex3d(x1 ,y1 ,z1);
		glVertex3d(x1 ,y0 ,z1);

		glVertex3d(x0 ,y0 ,z0);
		glVertex3d(x1 ,y0 ,z0);
		glVertex3d(x1 ,y0 ,z1);
		glVertex3d(x0 ,y0 ,z1);

		glVertex3d(x0 ,y1 ,z0);
		glVertex3d(x1 ,y1 ,z0);
		glVertex3d(x1 ,y1 ,z1);
		glVertex3d(x0 ,y1 ,z1);
	glEnd();
	glColor3f(0,0,0);
	glBegin(GL_LINES);
		glVertex3d(x0, y0, z0);
		glVertex3d(x1 ,y0 ,z0);
		glVertex3d(x1 ,y1 ,z0);
		glVertex3d(x0 ,y1 ,z0);

		glVertex3d(x0, y0, z1);
		glVertex3d(x1 ,y0 ,z1);
		glVertex3d(x1 ,y1 ,z1);
		glVertex3d(x0 ,y1 ,z1);

		glVertex3d(x0, y0, z0);
		glVertex3d(x0 ,y1 ,z0);
		glVertex3d(x0 ,y1 ,z1);
		glVertex3d(x0 ,y0 ,z1);

		glVertex3d(x1, y0, z0);
		glVertex3d(x1 ,y1 ,z0);
		glVertex3d(x1 ,y1 ,z1);
		glVertex3d(x1 ,y0 ,z1);

		glVertex3d(x0 ,y0 ,z0);
		glVertex3d(x1 ,y0 ,z0);
		glVertex3d(x1 ,y0 ,z1);
		glVertex3d(x0 ,y0 ,z1);

		glVertex3d(x0 ,y1 ,z0);
		glVertex3d(x1 ,y1 ,z0);
		glVertex3d(x1 ,y1 ,z1);
		glVertex3d(x0 ,y1 ,z1);
	glEnd();
}
