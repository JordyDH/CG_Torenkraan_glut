// JORDY DE HOON
// OBJECT FUNCTIES VOOR TEKENEN EN PARAMETRISEREN VAN EEN KRAAN
// WERKT OP GLUTGAME DEV0.3

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "obj_torenkraan.h"
#include "materiaal.h"
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
#define TK_ARM_WIDTH	TK_CAB_WIDTH
#define TK_ARM_LENGHT	2.0
#define TK_ARM_TUBESIZE	0.2
#define TK_ARM_SLICES	20
#define TK_ARM_BACKPIECES 3
// ----------------------------------------------------------------------------------
#define TK_LAST_PLATE_DEPTH	0.1
#define TK_LAST_PLATE_WIDTH	1.0
#define TK_LAST_HEIGHT		1.0
#define TK_LAST_OFFSET		0.5
// ----------------------------------------------------------------------------------

// Local prototyping for internal function
void renderBalk(double x0, double y0, double z0, double x1, double y1, double z1);

Kraan_struct* torenkraan_AllocObj(double x, double y, double z, uint16_t height, uint16_t lenght)
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
	//Draw Arm ---------------------------------------------------------
	//front piece
	for(uint16_t i = 0; i < lenght; i++)
	{
		obj = glutGameObjectsAlloc_object();
		(*obj).callback = torenkraan_arm;
		(*obj).linkedobj = obj_prev;
		if(i == 0)
			(*obj).y = TK_CAB_HEIGHT;
		else
			(*obj).z = TK_ARM_LENGHT;
		obj_prev = obj;
	};
	//Draw last -----------------------------------------------------
	obj = glutGameObjectsAlloc_object();
	(*obj).callback = torenkraan_last;
	(*obj).linkedobj = obj_prev;
	(*obj).i_mem[0] = lenght;
	(*obj).rot_y = 180;
	(*kraan_obj).obj_last = obj;
	//back piece
	for(uint16_t i = 0; i < TK_ARM_BACKPIECES; i++)
	{
		obj = glutGameObjectsAlloc_object();
		(*obj).callback = torenkraan_arm;
		if(i == 0)
		{
			(*obj).linkedobj = (*kraan_obj).obj_disk;
			(*obj).rot_y = 180;
			(*obj).y = TK_CAB_HEIGHT;
		}
		else
		{
			(*obj).linkedobj = obj_prev;
			(*obj).z = TK_ARM_LENGHT;
		}
		obj_prev = obj;
	}

	return kraan_obj;
}

void torenkraan_kolom(glutGameObjectobject *self)
{
	//Set materials ---------------------------------------------------------
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,brons_ambient);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,brons_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,brons_specular);
	//Body of object --------------------------------------------------------
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
	//Set materials ---------------------------------------------------------
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,wit_ambient);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,wit_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,wit_specular);
	//Body of object --------------------------------------------------------
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
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,grijs_ambient);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,grijs_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,grijs_specular);
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

void torenkraan_arm(glutGameObjectobject *self)
{
	//Set materials ---------------------------------------------------
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,chroom_ambient);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,chroom_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,chroom_specular);
	//Body of object --------------------------------------------------
	for(uint16_t i = 0; i < 3; i++)
	{
		glPushMatrix();
		glRotatef(90*i,0,0,1);
		glTranslatef(TK_ARM_WIDTH/2,0,0);
		GLUquadric* quad = gluNewQuadric();
		gluCylinder(quad, TK_ARM_TUBESIZE, TK_ARM_TUBESIZE, TK_ARM_LENGHT, TK_ARM_SLICES,TK_ARM_SLICES/4);
		quad = gluNewQuadric();
		gluDisk(quad, 0, TK_ARM_TUBESIZE, TK_ARM_SLICES, 1);
		glTranslatef(0,0,TK_ARM_LENGHT);
		quad = gluNewQuadric();
		gluDisk(quad, 0, TK_ARM_TUBESIZE, TK_ARM_SLICES, 1);
		glPopMatrix();
	}
	glPushMatrix();
		glTranslatef(-TK_ARM_WIDTH/2,0,TK_ARM_LENGHT/2);
		glRotatef(90,0,1,0);
		GLUquadric* quad = gluNewQuadric();
		gluCylinder(quad, TK_ARM_TUBESIZE/2, TK_ARM_TUBESIZE/2, TK_ARM_WIDTH, 5, 1);
		glRotatef(-45,1,0,0);
		gluCylinder(quad, TK_ARM_TUBESIZE/2, TK_ARM_TUBESIZE/2, sqrt(pow(TK_ARM_WIDTH/4,2)+pow(TK_ARM_WIDTH/2,2))+TK_ARM_TUBESIZE, 5,1);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(TK_ARM_WIDTH/2,0,TK_ARM_LENGHT/2);
		glRotatef(-90,0,1,0);
		glRotatef(-45,1,0,0);
		quad = gluNewQuadric();
		gluCylinder(quad, TK_ARM_TUBESIZE/2, TK_ARM_TUBESIZE/2, sqrt(pow(TK_ARM_WIDTH/4,2)+pow(TK_ARM_WIDTH/2,2))+TK_ARM_TUBESIZE, 5,1);
	glPopMatrix();
}

void torenkraan_last(glutGameObjectobject *self)
{
	//Set material ----------------------------------------------
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,lila_ambient);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,lila_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,lila_specular);
	//Body of object --------------------------------------------
	//x y z van last wordt niet aangepast, translatie gebeurd intern in deze functie
	//enkel z translatie nodig met check voor niet verder te bewegen dan mogelijk is.
	//lf_mem[0] heeft de z translatie, i_mem[0] heeft aantal stukken van de arm
	//lf_mem[1] voor last op en neer te bewegen
	//Check of translatie mogelijk is

	//Upper limit check
	if((*self).lf_mem[0] > (*self).i_mem[0]*TK_ARM_LENGHT) (*self).lf_mem[0] = (*self).i_mem[0]*TK_ARM_LENGHT;
	//Lower limit check
	if((*self).lf_mem[0] < 0) (*self).lf_mem[0] = 0;
	if((*self).lf_mem[1] < TK_LAST_HEIGHT+TK_LAST_OFFSET) (*self).lf_mem[1] = TK_LAST_HEIGHT+TK_LAST_OFFSET;
	//Translate object
	glPushMatrix();
		glTranslatef(0,0,(*self).lf_mem[0]);
		//Draw plate
		renderBalk(-TK_ARM_WIDTH/2, 0, -TK_LAST_PLATE_WIDTH/2, +TK_ARM_WIDTH/2,  -TK_LAST_PLATE_DEPTH, TK_LAST_PLATE_WIDTH/2 );
		//Draw Last
		glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,geel_ambient);
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,geel_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,geel_specular);
		glColor3f(1,1,1);
		glTranslatef(0,-(*self).lf_mem[1],0);
		GLUquadric* quad = gluNewQuadric();
		gluSphere(quad, TK_LAST_HEIGHT, 10 , 10);
		//Draw connectie punt
		glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,lila_ambient);
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,lila_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,lila_specular);
		glColor3f(1,1,1);
		glRotatef(-90,1,0,0);
		quad = gluNewQuadric();
		gluCylinder(quad, TK_ARM_TUBESIZE/2, TK_ARM_TUBESIZE/2, (*self).lf_mem[1], 5, 1);
	glPopMatrix();
}

//Render een balk met 2 punten diagonaal.
void renderBalk(double x0, double y0, double z0, double x1, double y1, double z1)
{
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
}
