////////////////////////////////////////////////////////////////////
//
//	glutGameRender by Jordy de Hoon
//	MIT LICENSE , goto www.github.com/JordyDH/glutGameControl
//
////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <GL/glut.h>
#include "glutGameLibs.h"

//////////////////////////////////// FUNCTION POINTERS ////////////////////////////////////
void (*RenderScene_fnc)() = 0x00;	//Callback function to render the 3D scene
void (*RenderScene2D_fnc)() = 0x00;	//Callback function to render the 2D scene

//////////////////////////////////// [GLUTGAME RENDER FUNCTION]  ////////////////////////////////////
static double	framecounter = 0;
static int	timebase = 0;
static double	fps = 0;
static uint64_t screen_width = 0, screen_height = 0;
static uint64_t world_buffer = 0;
static uint64_t rendertimebase = 0;
static uint8_t	perspective_mode = 0;	//0-Perspective 1-Orho
/*
* Function : void glutGameRenderService()
* RenderService routine
*/
void glutGameRenderService()
{
	glutPostRedisplay();
	glutTimerFunc(GLUTGAME_RENDER_LIMITER,glutGameRenderService,0);
}

/*
* Function : uint64_t glutGameRenderScreenWidth()
* Returns the screen width in pixels
*/
uint64_t glutGameRenderScreenWidth()
{
	return screen_width;
}

/*
* Function : uint64_t glutGameRenderScreenHeight()
* Returns the screen height in pixels
*/
uint64_t glutGameRenderScreenHeight()
{
	return screen_height;
}

/*
* Function: void glutGameRenderSceneSet()
* Binds a callback fucntion to render the world
*/
void glutGameRenderSceneSet(void (*fnc_p)())
{
	RenderScene_fnc = fnc_p;
}

/*
* Function: void glutGameRenderScene()
* Calls the callback function to render or define the 3D Scene
*/
void glutGameRenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glutGameCameraRender();

	if(RenderScene_fnc!=0x00)
		(*RenderScene_fnc)();
	glutGameRenderLocalAxis();
}

/*
* Function : void glutGameRender2DScene()
* Calls the callback function to render the 2D Scene
*/
void glutGameRender2DScene()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0,screen_width,screen_height,0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_CULL_FACE);
	glClear(GL_DEPTH_BUFFER_BIT);

	if(RenderScene2D_fnc != 0x00)
		(*RenderScene2D_fnc)();

	glutGameRenderOnScreenInfo();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

/*
* Function: void glutGameRender()
* The main function glut backend uses to render the world
* and player view.
*/
void glutGameRender()
{
	glutGameRenderScene();
	glutGameRender2DScene();
	#ifdef GLUTGAME_RENDER_DUBBELBUFFER
		glFinish();
		glutSwapBuffers();
	#else
		//glFinish();
		glFlush();
	#endif
	glutGameRenderFPS();
	#ifdef GLUTGAME_RENDER_SHOWFPS
	printf("[GLUTGAME][RENDER] FPS: %.3lf\n",glutGameRenderGetFPS());
	#endif
}

/*
* Functions to set the perspective camera mode
*/

void glutGameRenderSetPerspective()
{
	//Set the mode and force call Rescale
	perspective_mode=0;
	glutGameRescale(screen_width,screen_height);
}
void glutGameRenderSetOrtho()
{
	//Set the mode and force call Rescale
	perspective_mode=1;
	glutGameRescale(screen_width,screen_height);
}
void glutGameRenderSetFrustum()
{
	//Set the mode and force call Rescale
	perspective_mode=2;
	glutGameRescale(screen_width,screen_height);
}

/*
* Function: void glutGameRescale()
* Used to update the camera aspect ratio and parameters
* when the screen size is changed.
* Inputs:
*	n_w : new width in pixels of the window
*	n_h : new height in pixels of the window
*/
void glutGameRescale(GLint n_w, GLint n_h)
{
	float aspect = (float)n_w/n_h;
	float scale = 10.0;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(perspective_mode == 0) gluPerspective(60.0, aspect,  GLUTGAME_PLAYER_NEARSIGHT, GLUTGAME_PLAYER_FARSIGHT);
	if(perspective_mode == 1)
	{
		if(n_w <= n_h)  glOrtho(-scale, scale,-scale/aspect,scale/aspect,  GLUTGAME_PLAYER_NEARSIGHT, GLUTGAME_PLAYER_FARSIGHT);
		else		glOrtho(-scale*aspect,scale*aspect,-scale,scale,  GLUTGAME_PLAYER_NEARSIGHT, GLUTGAME_PLAYER_FARSIGHT);
	}
	if(perspective_mode == 2)
	{
		glFrustum(-GLUTGAME_PLAYER_NEARSIGHT,GLUTGAME_PLAYER_NEARSIGHT,-GLUTGAME_PLAYER_NEARSIGHT,GLUTGAME_PLAYER_NEARSIGHT,GLUTGAME_PLAYER_NEARSIGHT,GLUTGAME_PLAYER_FARSIGHT);
	}
	glViewport(0, 0, n_w, n_h);
	screen_width = n_w; screen_height = n_h;
}

/*
* Function: void glutGameRenderFPS()
* Calculate the FPS of the current session.
*/
void glutGameRenderFPS()
{
	framecounter++;
	int time = glutGet(GLUT_ELAPSED_TIME);
	if(time - timebase > 100)
	{
		fps = framecounter*1000.0/(time-timebase);
		timebase = time;
		framecounter = 0;
	}
}

/*
* Function: void glutGameGetFPS()
* Returns the current FPS of the running session.
*/
double glutGameRenderGetFPS()
{
	return fps;
}

/*
* Function : void glutGameRenderAllObjects()
* Renders all the objects in the object list in the 3D scene
*/
void glutGameRenderAllObjects()
{
	glutGameObjectList *objectList;
	glutGameObjectobject *p;

	objectList = glutGameObjectsGetList();
	for(;objectList!=0;objectList = (*objectList).next)
	{
		if((*objectList).struct_id == GLUTGAME_STRUCTID_OBJECT)
		{
			p = (*objectList).object;
			glutGameRenderObject(p);
		}
	}
}

/*
* Function : void glutGameRenderAllObjects()
* Renders all the objects in the object list in the 3D scene
*/
void glutGameRenderAllLights()
{
	glutGameObjectList *objectList;
	glutGameObjectlight *p;

	objectList = glutGameObjectsGetList();
	for(;objectList!=0;objectList = (*objectList).next)
	{
		if((*objectList).struct_id == GLUTGAME_STRUCTID_LIGHT)
		{
			p = (*objectList).object;
			glutGameRenderLight(p);
		}
	}
}

/*
* Function : void glutGameRenderLight(glutGameObjectlight *object)
* Render a single light source in the 3D Scene
* With linked obj support
*/
void glutGameRenderLight(glutGameObjectlight *object)
{
	float no_color[4] = {0.0, 0.0, 0.0 ,0.0};
	uint64_t depth = 0;
	glPushMatrix();
	glutGameObjectobject *reff;
	//Depth scan of the linked object list
	for(reff = (*object).linkedobj; reff != 0x00; reff = (*reff).linkedobj) depth++;
	//Go from the bottom of the list to the top
	for(int64_t i = depth; i >= 1; i--)
	{
		reff = (*object).linkedobj;
		//Start from the bottom and move up
		for(uint64_t j = 0; j < i-1; j++) reff = (*reff).linkedobj;
		//Translate axis to the reff object
		glTranslatef((*reff).x,(*reff).y,(*reff).z);
		//Rotate axis to the reff object
		if((*reff).rot_x != 0) glRotatef((*reff).rot_x,1,0,0);
		if((*reff).rot_y != 0) glRotatef((*reff).rot_y,0,1,0);
		if((*reff).rot_z != 0) glRotatef((*reff).rot_z,0,0,1);
	}
	//Render light
	//Set color (#TODO track change status in light to speed up process)
	if((*object).ambient != 0x00)  glLightfv((*object).id,GL_AMBIENT,(*object).ambient);
	else glLightfv((*object).id,GL_AMBIENT,&no_color[0]);
	if((*object).diffuse != 0x00)  glLightfv((*object).id,GL_DIFFUSE,(*object).diffuse);
	else glLightfv((*object).id,GL_DIFFUSE,&no_color[0]);
	if((*object).specular != 0x00) glLightfv((*object).id,GL_SPECULAR,(*object).specular);
	else glLightfv((*object).id,GL_SPECULAR,&no_color[0]);
	//Spot effect of the light
	if((*object).spot_enable)
	{
		glLightf ( (*object).id , GL_SPOT_CUTOFF, (*object).spot_cutoff);
		glLightfv( (*object).id , GL_SPOT_DIRECTION, &(*object).spot_direction[0]);
		glLightf ( (*object).id , GL_SPOT_EXPONENT, (*object).spot_exponent);
	}
	//Enable or disable the light
	if((*object).enable) glEnable((*object).id);
	else glDisable((*object).id);
	//Place light at correct possition
	//#TODO Change xyzw to array -> pos array not needed then
	float pos[4];
	pos[0] = (*object).x; pos[1] = (*object).y; pos[2] = (*object).z; pos[3]= (*object).w;
	glLightfv((*object).id ,GL_POSITION ,&pos[0]);
	//Render debug cube of light source
	if((*object).debug && (*object).enable)
	{
		glPushAttrib(GL_CURRENT_BIT|GL_LIGHTING_BIT);
		glDisable(GL_LIGHTING);
		if((*object).ambient != 0x00) glColor4fv((*object).ambient);
		else if((*object).diffuse != 0x00)  glColor4fv((*object).diffuse);
		else if((*object).specular != 0x00) glColor4fv((*object).specular);
		//Render light direction
		glBegin(GL_LINES);
			//If spot enable different end point
			if((*object).spot_enable) glVertex3fv(&(*object).spot_direction[0]);
			else glVertex3f(0,0,0);
			glVertex3f(pos[0],pos[1],pos[2]);
		glEnd();
		glTranslatef(pos[0],pos[1],pos[2]);
		glutSolidCube(0.25);
		glPopAttrib();
		glEnable(GL_LIGHTING);
	}
	glPopMatrix();
}

/*
* Function : void glutGameRenderObject(glutGameObjectobject *object)
* Render a single object in the 3D Scene
* With linked obj support
*/
void clearAllMaterials()
{
	const float no_color[4] = {0.0, 0.0, 0.0, 0.0};
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT, no_color);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, no_color);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, no_color);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS , 0.0);

}
void glutGameRenderObject(glutGameObjectobject *object)
{
	uint64_t depth = 0;
	glPushMatrix();
	glutGameObjectobject *reff;
	//Depth scan of the linked object list
	for(reff = (*object).linkedobj; reff != 0x00; reff = (*reff).linkedobj) depth++;
	//Go from the bottom of the list to the top
	for(int64_t i = depth; i >= 0; i--)
	{
		reff = object;
		//Start from the bottom and move up
		for(uint64_t j = 0; j < (i); j++) reff = (*reff).linkedobj;
		//Translate axis to the reff object
		glTranslatef((*reff).x,(*reff).y,(*reff).z);
		//Rotate axis to the reff object
		if((*reff).rot_x != 0) glRotatef((*reff).rot_x,1,0,0);
		if((*reff).rot_y != 0) glRotatef((*reff).rot_y,0,1,0);
		if((*reff).rot_z != 0) glRotatef((*reff).rot_z,0,0,1);
	}
	//Callback for drawing the object
	(*(*object).callback)(object);
	if((*object).local_axis)
	{
		clearAllMaterials();
		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_LIGHTING);
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
		glPopAttrib();
	}
	glPopMatrix();
}

/*
* Function : void glutGameRenderCompileList()
* Compile the object list in a single render list for speed
* Only use for static elements
*/
void glutGameRenderCompileList()
{
	glutGameObjectList *objectList;
	glutGameObjectobject *p;

	objectList = glutGameObjectsGetList();
	if(world_buffer!=0) glDeleteLists(world_buffer,1);
	world_buffer = glGenLists(1);
	printf("world id: %ld\n",world_buffer);
	glNewList(world_buffer, GL_COMPILE);
	for(;objectList!=0;objectList = (*objectList).next)
	{
	if((*objectList).struct_id == GLUTGAME_STRUCTID_OBJECT)
	{
		p = (*objectList).object;
		glutGameRenderObject(p);
	}
	}
	glEndList();
}
