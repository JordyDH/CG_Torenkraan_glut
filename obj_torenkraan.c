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
#define TK_WINDOW_SEGW	4
#define TK_WINDOW_SEGH	6
// ----------------------------------------------------------------------------------
#define TK_ARM_WIDTH	TK_CAB_WIDTH
#define TK_ARM_LENGHT	2.0
#define TK_ARM_TUBESIZE	0.2
#define TK_ARM_SLICES	10
#define TK_ARM_BACKPIECES 4
// ----------------------------------------------------------------------------------
#define TK_LAST_PLATE_DEPTH	0.1
#define TK_LAST_PLATE_WIDTH	1.0
#define TK_LAST_HEIGHT		1.0
#define TK_LAST_OFFSET		0.5
// ----------------------------------------------------------------------------------
#define TK_WEIGHT_HEIGHT	TK_CAB_HEIGHT
#define TK_WEIGHT_WIDTH		TK_ARM_WIDTH
#define TK_WEIGHT_DEPTH		1.0
// ----------------------------------------------------------------------------------

// Local prototyping for internal function
void renderBalk(double x0, double y0, double z0, double x1, double y1, double z1);

// Kraan functie: zorgt voor alle allocaties in de render lijst voor en kraan op het scherm te krijgen
Kraan_struct* torenkraan_AllocObj(double x, double y, double z, uint16_t height, uint16_t lenght, GLenum Light_ID)
{
	//Generate a new structure
	Kraan_struct *kraan_obj = (Kraan_struct*)calloc(1,sizeof(Kraan_struct));
	(*kraan_obj).shinniness = 10;	//Default value
	//2 pointer to help with linking obj
	glutGameObjectobject *obj;
	glutGameObjectobject *obj_prev;
	//Draw the koloms -----------------------------------------------------------------------------------------------------
	for(uint16_t i = 0; i < height; i++)
	{
		obj = glutGameObjectsAlloc_object();	//Allocate memoty in buffer
		(*obj).callback = torenkraan_kolom;	//Bind drawing callback
		if(i == 0)				//First element is parent
		{
			(*obj).x = x;			//Set to given x position
			(*obj).y = y;			//Set to given y position
			(*obj).z = z;			//Set to given z postiiton
			(*kraan_obj).obj_kolom = obj;	//Store pointer for later use
		}
		else					//Next objects are childs
		{
			(*obj).y = TK_KOLOM_HEIGHT;	//Set y offeset to height of model
			(*obj).linkedobj = obj_prev;	//Link object to previous object
		}
		(*obj).pf_mem[0] = &(*kraan_obj).shinniness;	//Link global shinniness value to obj
		(*obj).p_mem[1] = &(*kraan_obj).flag_localaxis;	//link flag to enable local axis
		obj_prev = obj;					//This object is now previous object.
	}
	//Draw disks ---------------------------------------------------------------------------------------------------------
	for(uint16_t i = 0 ; i < 2; i++)		//Draw 2 Disk
	{
		obj = glutGameObjectsAlloc_object();	//Allocate memory in buffer
		(*obj).callback = torenkraan_disk;	//Bind drawing callback
		if(i == 0)
		{
			(*kraan_obj).obj_disk2 = obj;	//Store first disk
			(*obj).y = TK_KOLOM_HEIGHT;	//Set offset if parent
		}
		else       (*obj).y = 0.05;		//Else this is a child
		(*obj).linkedobj = obj_prev;		//Link the previous object
		obj_prev = obj;				//This is now the previous object
		(*obj).pf_mem[0] = &(*kraan_obj).shinniness;	//Link global shinniness value to obj
		(*obj).p_mem[1] = &(*kraan_obj).flag_localaxis;	//link flag to enable local axis
	}
	// Store last disk object, used for rotation.
	(*kraan_obj).obj_disk = obj_prev;
	//Draw kabine ---------------------------------------------------------------------------------------------------------
	obj = glutGameObjectsAlloc_object();		//Allocate memory in buffer
	(*obj).callback = torenkraan_kabine;		//Bind drawing callback
	(*obj).linkedobj = obj_prev;			//Link previous obj
	obj_prev = obj;					//This obj is now the prev obj
	(*kraan_obj).obj_cab = obj;			//Store adress to kabine
	(*obj).pf_mem[0] = &(*kraan_obj).shinniness;	//Link global shinniness value to obj
	(*obj).p_mem[1] = &(*kraan_obj).flag_localaxis;	//link flag to enable local axis
	// Draw light point ---------------------------------------------------------------------------------------------------
	(*kraan_obj).light = glutGameObjectsAlloc_light();	//Allocate a light model
	(*(*kraan_obj).light).id = Light_ID;			//Bind given light id to model (ex GL_LIGHT0)
	(*(*kraan_obj).light).enable = 1;			//Enable the light model
	(*(*kraan_obj).light).debug = 1;			//Enable debug view
	(*(*kraan_obj).light).linkedobj = obj_prev;		//link light to the kabine
	(*(*kraan_obj).light).spot_enable = 1;			//Enable spot effect
	(*(*kraan_obj).light).ambient  = geel_ambient;		//set ambient
	(*(*kraan_obj).light).diffuse  = geel_diffuse;		//set diffuse
	(*(*kraan_obj).light).specular = geel_specular;		//set specular
	(*(*kraan_obj).light).y = TK_CAB_HEIGHT/2;		//Set height offset
	(*(*kraan_obj).light).w = 1.0;				//Set w
	(*(*kraan_obj).light).spot_direction[0] = 00.0;		//X direction
	(*(*kraan_obj).light).spot_direction[1] = -5.0;		//Y direction
	(*(*kraan_obj).light).spot_direction[2] = 10;		//Z direction
	(*(*kraan_obj).light).spot_cutoff  = 30.0;		//Cutoff param
	(*(*kraan_obj).light).spot_exponent = 20.0;		//Exponent param
	//Draw Arm -------------------------------------------------------------------------------------------------------------
	//front piece
	uint32_t *pointer;
	pointer = (uint32_t *)calloc(1,sizeof(uint32_t));
	for(uint16_t i = 0; i < lenght; i++)		//Draw lenght amount of arm pieces
	{
		obj = glutGameObjectsAlloc_object();	//Allocate memory in buffer
		(*obj).callback = torenkraan_arm;	//Bind drawing callback
		(*obj).linkedobj = obj_prev;		//link previous obj
		(*obj).p_mem[0] = pointer;		//Shared memory between childeren
		if(i == 0)				//If this is the parent
			(*obj).y = TK_CAB_HEIGHT;	//translate in y of the cabine height
		else					//Else this is child (little basterd)
			(*obj).z = TK_ARM_LENGHT;	//translate z with lengt of the arm
		obj_prev = obj;				//this is now the previous obj
		(*obj).pf_mem[0] = &(*kraan_obj).shinniness;	//Link global shinniness value to obj
		(*obj).p_mem[1] = &(*kraan_obj).flag_localaxis;	//link flag to enable local axis
	};
	(*kraan_obj).obj_arm = obj_prev;		//Store arm pointer
	//Draw last --------------------------------------------------------------------------------------------------------------
	obj = glutGameObjectsAlloc_object();	//Allocate memory in buffer
	(*obj).callback = torenkraan_last;	//Bind drawing callback
	(*obj).linkedobj = obj_prev;		//Bind object to previous object
	(*obj).i_mem[0] = lenght;		//i_mem[0] stores the lengt of the arm (objects are self aware not like terminator)
	(*obj).rot_y = 180;			//Rotate axis 180° easyer to move
	(*kraan_obj).obj_last = obj;		//Store pointer to this object
	(*obj).pf_mem[0] = &(*kraan_obj).shinniness;	//Link global shinniness value to obj
	(*obj).p_mem[1] = &(*kraan_obj).flag_localaxis;	//link flag to enable local axis
	//back piece of arm ---------------------------------------------------------------------------------------------------------
	for(uint16_t i = 0; i < TK_ARM_BACKPIECES; i++)
	{
		obj = glutGameObjectsAlloc_object();	//Allocate memory in buffer
		(*obj).callback = torenkraan_arm;	//Bind drawing callback
		(*obj).p_mem[0] = pointer;		//Shared memory between childeren
		if(i == 0)				//If this object is parent (first obj)
		{
			(*obj).linkedobj = (*kraan_obj).obj_disk;	//Link to disk
			(*obj).rot_y = 180;				//rotate y axis 180° (draw behind)
			(*obj).y = TK_CAB_HEIGHT;			//translate y with kabine height
		}
		else					//Else this object is a child (so many childs here)
		{
			(*obj).linkedobj = obj_prev;	//hold hands with parent or other children
			(*obj).z = TK_ARM_LENGHT;	//Translate z with lengt of arm (arm distance between childs for COVID)
		}
		obj_prev = obj;		//This is now the previous object
		(*obj).pf_mem[0] = &(*kraan_obj).shinniness;	//Link global shinniness value to obj
		(*obj).p_mem[1] = &(*kraan_obj).flag_localaxis;	//link flag to enable local axis
		//Skip the first arm
		if(i != 0)
		{
			//Draw gewicht
			obj = glutGameObjectsAlloc_object();	//Allocate memory in buffer
			(*obj).callback = torenkraan_gewicht;	//Bind drawing callback
			(*obj).linkedobj = obj_prev;		//Link object
			(*obj).z = TK_ARM_LENGHT/2;
			(*obj).pf_mem[0] = &(*kraan_obj).shinniness;	//Link global shinniness value to obj
			(*obj).p_mem[1] = &(*kraan_obj).flag_localaxis;	//link flag to enable local axis
		}
	}
	return kraan_obj;	//Kraan fully described in buffer and return the generated struct
}

void torenkraan_kolom(glutGameObjectobject *self)
{
	if( (*(*self).p_mem[1]) ) (*self).local_axis = 1;
	else (*self).local_axis = 0;
	//Set materials ---------------------------------------------------------
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,brons_ambient);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,brons_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,brons_specular);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS ,(*(*self).pf_mem[0]));
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
	//i_mem[0] : to show wire frame of disk
	//p_mem[1] : to enable local axis
	if( (*(*self).p_mem[1]) ) (*self).local_axis = 1;
	else (*self).local_axis = 0;
	//Set materials ---------------------------------------------------------
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,wit_ambient);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,wit_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,wit_specular);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS ,(*(*self).pf_mem[0]));
	//Body of object --------------------------------------------------------
	glPushMatrix();
	glRotatef(90,1,0,0);
	GLUquadric* quad = gluNewQuadric();
	if((*self).i_mem[0]) gluQuadricDrawStyle(quad,GLU_LINE);
	gluDisk(quad,TK_DISK_ISIZE, TK_DISK_OSIZE, TK_DISK_SLICES, 4);
	glPopMatrix();
}

void torenkraan_kabine(glutGameObjectobject *self)
{
	//i_mem[0] to enable wire mesh in window
	//i_mem[1] to enable point rendering mesh
	//i_mem[2] to enbale transparantie
	//p_mem[1] : to enable local axis
	if( (*(*self).p_mem[1]) ) (*self).local_axis = 1;
	else (*self).local_axis = 0;
	//Set materials ---------------------------------------------------
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,bruin_ambient);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,bruin_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,bruin_specular);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS ,(*(*self).pf_mem[0]));
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
	//Window functions ---------------------------------------------
	float mesh[TK_WINDOW_SEGH][TK_WINDOW_SEGW][3];
	//Generate points for mesh window
	for(uint16_t i = 0; i < TK_WINDOW_SEGH; i++)
	{
		for(uint16_t j = 0; j < TK_WINDOW_SEGW; j++)
		{
			mesh[i][j][0] = j * TK_CAB_WIDTH / (TK_WINDOW_SEGW-1);
			mesh[i][j][1] = i * TK_CAB_HEIGHT / (TK_WINDOW_SEGH-1);
			mesh[i][j][2] = (i * sqrt( pow(TK_CAB_DEPTH1/2,2)+pow(TK_CAB_DEPTH0/2,2) ) / (TK_WINDOW_SEGH-1)) * cos(tanh(TK_CAB_HEIGHT/((TK_CAB_DEPTH1-TK_CAB_DEPTH0)/2)))
			+ TK_CAB_DEPTH0 + sin(i*M_PI/(TK_WINDOW_SEGH-1)) *sin(j*M_PI/(TK_WINDOW_SEGW-1));
		}
	}
	//Enable transparity
	if((*self).i_mem[2])
	{
		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);
	}
	//Material specs
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,bruin_ambient);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,bruin_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,bruin_specular);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS ,(*(*self).pf_mem[0]));
	//Draw window ------------------------------------------------------
	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, TK_WINDOW_SEGW, 0, 1, TK_WINDOW_SEGW*3, TK_WINDOW_SEGH, &mesh[0][0][0]);
	glEnable(GL_MAP2_VERTEX_3);
	glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
	glEvalMesh2(GL_FILL, 0, 20, 0, 20);
	//Disable transparity
	if((*self).i_mem[2])
	{
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
	}
	//Draw mesh is flag is enabled
	if((*self).i_mem[0])
	{
		glEvalMesh2(GL_LINE, 0, 20, 0, 20);
	}
	glDisable(GL_MAP2_VERTEX_3);
	//Draw mesh points -------------------------------------------------
	if((*self).i_mem[1])
	{
		for(uint16_t i = 0; i < TK_WINDOW_SEGH; i++)
			for(uint16_t j = 0; j < TK_WINDOW_SEGW; j++)
			{
				glPushMatrix();
				glTranslatef(mesh[i][j][0],mesh[i][j][1],mesh[i][j][2]);
				glutSolidCube(0.05);
				glPopMatrix();
			}
	}
	glPopMatrix();
}

void torenkraan_arm(glutGameObjectobject *self)
{
	//(*p_mem[0]) for wireframe mode
	//p_mem[1] : to enable local axis
	if( (*(*self).p_mem[1]) ) (*self).local_axis = 1;
	else (*self).local_axis = 0;
	//Set materials ---------------------------------------------------
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,chroom_ambient);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,chroom_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,chroom_specular);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS ,(*(*self).pf_mem[0]));
	//Body of object --------------------------------------------------
	GLUquadric* quad = gluNewQuadric();
	if( (*(*self).p_mem[0]) ) gluQuadricDrawStyle(quad,GLU_LINE);
	for(uint16_t i = 0; i < 3; i++)
	{
		glPushMatrix();
		glRotatef(90*i,0,0,1);
		glTranslatef(TK_ARM_WIDTH/2,0,0);
		gluCylinder(quad, TK_ARM_TUBESIZE, TK_ARM_TUBESIZE, TK_ARM_LENGHT, TK_ARM_SLICES,TK_ARM_SLICES/2);
		gluDisk(quad, 0, TK_ARM_TUBESIZE, TK_ARM_SLICES, 1);
		glTranslatef(0,0,TK_ARM_LENGHT);
		gluDisk(quad, 0, TK_ARM_TUBESIZE, TK_ARM_SLICES, 1);
		glPopMatrix();
	}
	glPushMatrix();
		glTranslatef(-TK_ARM_WIDTH/2,0,TK_ARM_LENGHT/2);
		glRotatef(90,0,1,0);
		gluCylinder(quad, TK_ARM_TUBESIZE/2, TK_ARM_TUBESIZE/2, TK_ARM_WIDTH, 5, 1);
		glRotatef(-45,1,0,0);
		gluCylinder(quad, TK_ARM_TUBESIZE/2, TK_ARM_TUBESIZE/2, sqrt(pow(TK_ARM_WIDTH/4,2)+pow(TK_ARM_WIDTH/2,2))+TK_ARM_TUBESIZE, 5,1);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(TK_ARM_WIDTH/2,0,TK_ARM_LENGHT/2);
		glRotatef(-90,0,1,0);
		glRotatef(-45,1,0,0);
		gluCylinder(quad, TK_ARM_TUBESIZE/2, TK_ARM_TUBESIZE/2, sqrt(pow(TK_ARM_WIDTH/4,2)+pow(TK_ARM_WIDTH/2,2))+TK_ARM_TUBESIZE, 5,1);
	glPopMatrix();
}

void torenkraan_last(glutGameObjectobject *self)
{
	//p_mem[1] : to enable local axis
	if( (*(*self).p_mem[1]) ) (*self).local_axis = 1;
	else (*self).local_axis = 0;
	//Set material ----------------------------------------------
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,lila_ambient);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,lila_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,lila_specular);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS ,(*(*self).pf_mem[0]));
	//Body of object --------------------------------------------
	//x y z van last wordt niet aangepast, translatie gebeurd intern in deze functie
	//enkel z translatie nodig met check voor niet verder te bewegen dan mogelijk is.
	//lf_mem[0] heeft de z translatie, i_mem[0] heeft aantal stukken van de arm
	//lf_mem[1] voor last op en neer te bewegen
	//i_mem[1] voor wireframe mode
	//Check of translatie mogelijk is
	//Upper limit check
	if((*self).lf_mem[0] > (*self).i_mem[0]*TK_ARM_LENGHT) (*self).lf_mem[0] = (*self).i_mem[0]*TK_ARM_LENGHT;
	//Lower limit check
	if((*self).lf_mem[0] < 0) (*self).lf_mem[0] = 0;
	if((*self).lf_mem[1] < TK_LAST_HEIGHT+TK_LAST_OFFSET) (*self).lf_mem[1] = TK_LAST_HEIGHT+TK_LAST_OFFSET;
	//Translate object
	GLUquadric* quad = gluNewQuadric();
	if((*self).i_mem[1]) gluQuadricDrawStyle(quad,GLU_LINE);
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
		gluSphere(quad, TK_LAST_HEIGHT, 10 , 10);
		//Draw connectie punt
		glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,lila_ambient);
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,lila_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,lila_specular);
		glColor3f(1,1,1);
		glRotatef(-90,1,0,0);
		gluCylinder(quad, TK_ARM_TUBESIZE/2, TK_ARM_TUBESIZE/2, (*self).lf_mem[1], 5, 1);
	glPopMatrix();
}

void torenkraan_gewicht(glutGameObjectobject *self)
{
	if( (*(*self).p_mem[1]) ) (*self).local_axis = 1;
	else (*self).local_axis = 0;
	//Set material -------------------------------------------
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,brons_ambient);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,brons_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,brons_specular);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS ,(*(*self).pf_mem[0]));
	//Body of object -----------------------------------------
	glPushMatrix();
	glTranslatef(-TK_WEIGHT_WIDTH/2,-TK_WEIGHT_HEIGHT,-TK_WEIGHT_DEPTH/2);
	glBegin(GL_QUADS);
		//BACK
		glVertex3f(0,0,0);
		glVertex3f(TK_WEIGHT_WIDTH,0,0);
		glVertex3f(TK_WEIGHT_WIDTH,TK_WEIGHT_HEIGHT,0);
		glVertex3f(0.0,TK_WEIGHT_HEIGHT,0);
		//TOP
		glVertex3f(0,TK_WEIGHT_HEIGHT,0);
		glVertex3f(TK_WEIGHT_WIDTH,TK_WEIGHT_HEIGHT,0);
		glVertex3f(TK_WEIGHT_WIDTH,TK_WEIGHT_HEIGHT,TK_WEIGHT_DEPTH);
		glVertex3f(0,TK_WEIGHT_HEIGHT,TK_WEIGHT_DEPTH);
	glEnd();
	glBegin(GL_TRIANGLES);
		glVertex3f(0,0,0);
		glVertex3f(0,TK_WEIGHT_HEIGHT,0);
		glVertex3f(0,TK_WEIGHT_HEIGHT,TK_WEIGHT_DEPTH);
		glVertex3f(TK_WEIGHT_WIDTH,0,0);
		glVertex3f(TK_WEIGHT_WIDTH,TK_WEIGHT_HEIGHT,0);
		glVertex3f(TK_WEIGHT_WIDTH,TK_WEIGHT_HEIGHT,TK_WEIGHT_DEPTH);
	glEnd();
	float mesh[TK_WINDOW_SEGH][TK_WINDOW_SEGW][3];
	//Generate points for mesh window
	for(uint16_t i = 0; i < TK_WINDOW_SEGH; i++)
	{
		for(uint16_t j = 0; j < TK_WINDOW_SEGW; j++)
		{
			mesh[i][j][0] = j * TK_WEIGHT_WIDTH / (TK_WINDOW_SEGW-1);
			mesh[i][j][1] = i * TK_WEIGHT_HEIGHT / (TK_WINDOW_SEGH-1);
			mesh[i][j][2] = TK_WEIGHT_DEPTH * ((float)i/(TK_WINDOW_SEGH-1));
		}
	}
	//Draw window ------------------------------------------------------
	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, TK_WINDOW_SEGW, 0, 1, TK_WINDOW_SEGW*3, TK_WINDOW_SEGH, &mesh[0][0][0]);
	glEnable(GL_MAP2_VERTEX_3);
	glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
	glEvalMesh2(GL_FILL, 0, 20, 0, 20);
	//Draw mesh is flag is enabled
	if((*self).i_mem[0])
	{
		glEvalMesh2(GL_LINE, 0, 20, 0, 20);
	}
	glDisable(GL_MAP2_VERTEX_3);
	//Draw mesh points -------------------------------------------------
	if((*self).i_mem[1])
	{
		for(uint16_t i = 0; i < TK_WINDOW_SEGH; i++)
			for(uint16_t j = 0; j < TK_WINDOW_SEGW; j++)
			{
				glPushMatrix();
				glTranslatef(mesh[i][j][0],mesh[i][j][1],mesh[i][j][2]);
				glutSolidCube(0.05);
				glPopMatrix();
			}
	}
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

