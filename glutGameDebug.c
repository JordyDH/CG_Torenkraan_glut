////////////////////////////////////////////////////////////////////
//
//	glutGameDebug by Jordy de Hoon
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

//////////////////////////////////// [LIB VARS] ////////////////////////////////////
static double	*rotation_lr;
static double	*rotation_ud;
static double	*xl, *yl, *zl;
static double	*xPos, *yPos, *zPos;
//////////////////////////////////// [GLUTGAME IN GAME DEBUG FUNCTIONS] ////////////////////////////////////

/*
* Function: void glutGameDebugInit(glutGameObjectplayer *player)
* --------------------------------------------------------------
* Init pointer linking for onscreen debug info of player obj
*/
void glutGameDebugInit(glutGameObjectplayer *player)
{
	xPos = &(*player).base.x;
	yPos = &(*player).base.y;
	zPos = &(*player).base.z;
	xl = &(*player).xl;
	yl = &(*player).yl;
	zl = &(*player).zl;
	rotation_lr = &(*player).rotation_xz;
	rotation_ud = &(*player).rotation_y;
}

/*
* Function: void glutGameRenderLocalAxis()
* ----------------------------------------
* Renders a local axis in front of the camera view (allways in center of the screen)
*/
void glutGameRenderLocalAxis()
{
	//Draw the world axis in front of the camera
	//TODO Change lengt into define
	double lengt = 0.04;
	double mod_x, mod_y, mod_z;
	if(glutGameControlGetRegister() & (0x01 << 6))
	{
		glDisable(GL_DEPTH_TEST);
		glLineWidth(4);
		glBegin(GL_LINES);
			glColor3f(0,0,0);
			glVertex3d((*xPos)+((*xl)),(*yPos)+((*yl))+GLUTGAME_PLAYER_HEIGHT,(*zPos)+((*zl)));
			glVertex3d((*xPos)+((*xl))+lengt,(*yPos)+((*yl))+GLUTGAME_PLAYER_HEIGHT,(*zPos)+((*zl)));
			glColor3f(0,0,0);
			glVertex3d((*xPos)+((*xl)),(*yPos)+((*yl))+GLUTGAME_PLAYER_HEIGHT,(*zPos)+((*zl)));
			glVertex3d((*xPos)+((*xl)),(*yPos)+((*yl))+GLUTGAME_PLAYER_HEIGHT+lengt,(*zPos)+((*zl)));
			glColor3f(0,0,0);
			glVertex3d((*xPos)+((*xl)),(*yPos)+((*yl))+GLUTGAME_PLAYER_HEIGHT,(*zPos)+((*zl)));
			glVertex3d((*xPos)+((*xl)),(*yPos)+((*yl))+GLUTGAME_PLAYER_HEIGHT,(*zPos)+lengt+((*zl)));
		glEnd();
		glLineWidth(2);
		glBegin(GL_LINES);
			glColor3f(1,0,0);
			glVertex3d((*xPos)+((*xl)),(*yPos)+((*yl))+GLUTGAME_PLAYER_HEIGHT,(*zPos)+((*zl)));
			glVertex3d((*xPos)+((*xl))+lengt,(*yPos)+((*yl))+GLUTGAME_PLAYER_HEIGHT,(*zPos)+((*zl)));
			glColor3f(0,1,0);
			glVertex3d((*xPos)+((*xl)),(*yPos)+((*yl))+GLUTGAME_PLAYER_HEIGHT,(*zPos)+((*zl)));
			glVertex3d((*xPos)+((*xl)),(*yPos)+((*yl))+GLUTGAME_PLAYER_HEIGHT+lengt,(*zPos)+((*zl)));
			glColor3f(0,0,1);
			glVertex3d((*xPos)+((*xl)),(*yPos)+((*yl))+GLUTGAME_PLAYER_HEIGHT,(*zPos)+((*zl)));
			glVertex3d((*xPos)+((*xl)),(*yPos)+((*yl))+GLUTGAME_PLAYER_HEIGHT,(*zPos)+lengt+((*zl)));
		glEnd();
		glEnable(GL_DEPTH_TEST);

	}
}

/*
* Function: void glutBitmapString(void *font, char *text)
* --------------------------------------------------------
* Renders a string to bitmap chars.
*/
void glutBitmapString(void *font, char *text)
{
	if(text!=0x00)
	{
		for(;(*text)!='\0';text++)
		{
			glutBitmapCharacter(font,(*text));
		}
	}
}

/*
* Function: void glutGameRenderOnScreenInfo()
* ----------------------------------------
* Renders the onscreen info
*/
void glutGameRenderOnScreenInfo()
{
	glColor3f(1,1,1);
	char text[30];
	//itoa(systick,text,10);
	glRasterPos2f(5,20);
	sprintf(text,"%s%s","glutGameCore Version ",GLUTGAME_VERSION);
	glutBitmapString(GLUT_BITMAP_9_BY_15,text);
	glRasterPos2f(5,40);
	sprintf(text,"%s%d","systick: ",glutGameSystickGet());
	glutBitmapString(GLUT_BITMAP_9_BY_15,text);
	glRasterPos2f(5,60);
	sprintf(text,"[%s%.3f]","xPos: ",(*xPos));
	glutBitmapString(GLUT_BITMAP_9_BY_15,text);
	sprintf(text,"[%s%.3f]"," yPos: ",(*yPos));
	glutBitmapString(GLUT_BITMAP_9_BY_15,text);
	sprintf(text,"[%s%.3f]"," zPos: ",(*zPos));
	glutBitmapString(GLUT_BITMAP_9_BY_15,text);
	glRasterPos2f(5,80);
	sprintf(text,"[%s%.3f]","rotation_xz: ",(*rotation_lr));
	glutBitmapString(GLUT_BITMAP_9_BY_15,text);
	sprintf(text,"[%s%.3f]"," rotation_y: ",(*rotation_ud));
	glutBitmapString(GLUT_BITMAP_9_BY_15,text);
	glRasterPos2f(5,100);
	sprintf(text,"[%s%3.2lf]","FPS: ",glutGameRenderGetFPS());
	glutBitmapString(GLUT_BITMAP_9_BY_15,text);
}
