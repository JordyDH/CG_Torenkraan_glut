////////////////////////////////////////////////////////////////////
//
//	glutGameControl by Jordy de Hoon
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

//////////////////////////////////// [LIB VARS] //////////////////////////////////////////////////////
static uint64_t GLUTGAME_CONTROL_REG = 0;
static glutGameObjectplayer *mainplayer;

static double *rotation_lr;
static double *rotation_ud;

static uint8_t mouse_state_left = 0, mouse_state_right = 0;
static uint32_t mouse_x_old = 0, mouse_y_old = 0;

static double *xl, *yl, *zl;
static double *xPos, *yPos, *zPos;
static double speed_mul = 0.1;

static uint8_t controlEnable = 0;

void (*keyboard_fnc)(unsigned int) = 0x00;

//////////////////////////////////// [GLUTGAME CONTROL FUNCTIONS] ////////////////////////////////////

/*
* Function: void glutGameControlInit(glutGameObjectplayer *player)
* ----------------------------------------------------------------
* linking the player obj to internal pointers
*/
void glutGameControlInit(glutGameObjectplayer *player)
{
	mainplayer = player;
	xPos = &(*player).base.x;
	yPos = &(*player).base.y;
	zPos = &(*player).base.z;
	rotation_ud = &(*player).rotation_y;
	rotation_lr = &(*player).rotation_xz;

	glutGameMouseInit();
	glutGameKeyboardInit();
	glutTimerFunc(GLUTGAME_CONTROL_TIMER,glutGameControlUpdate,0);
}

/*
* Function: void glutGameControlEnable()
* --------------------------------------
* Enable control functions to modify player locations
*/
void glutGameControlEnable()
{
	controlEnable = 1;
}

/*
* Function: void glutGameControlDisable()
* ---------------------------------------
* Disable control functions to modify player location
*/
void glutGameControlDisable()
{
	controlEnable = 0;
}

/*
* Function: uint64_t glutGameControlGetRegister()
* -----------------------------------------------
* Returns the control register value,
* This stores the states of the tracked keys
*/
uint64_t glutGameControlGetRegister()
{
	return GLUTGAME_CONTROL_REG;
}

/*
* Function: void glutGameControlUpdate()
* --------------------------------------
* Control update service only used with
* state tracking
*/
void glutGameControlUpdate()
{
	#ifdef GLUTGAME_CONTROL_USEREG
	glutGameMoveCamera(0);
	#endif
	glutTimerFunc(GLUTGAME_CONTROL_TIMER,glutGameControlUpdate,0);
}

/*
* Function: void glutGameMouseInit()
* ----------------------------------------------------------------
* Mouse init function, 3 functions to track mouse button state
* and delta position between updates
*/
void glutGameMouseInit()
{
	glutMouseFunc(glutGameMouseKeys);
	glutMotionFunc(glutGameMouseMove);
	glutPassiveMotionFunc(glutGameMouseMove);
}

/*
* Function: void glutGameMouseKeys()
* -----------------------------
* Tracks the state of the buttons of the mouse.
* - mouse_left = state tracking of the left mouse button
* - mouse_right = state tracking of the right mouse button
* Return nothing.
*/
void glutGameMouseKeys(int button, int state, int x, int y)
{
        if((state == GLUT_DOWN)&&(button == GLUT_LEFT_BUTTON))
                mouse_state_left = 1;
        else
                mouse_state_left = 0;
	if((state == GLUT_DOWN)&&(button == GLUT_RIGHT_BUTTON))
                mouse_state_right = 1;
        else
                mouse_state_right = 0;
}

/*
* Function: void glutGameMouseMove()
* -----------------------------
* Tracks the delta movement of the mouse on the window.
* Can call callback functions when a mouse state is active
* Returns nothing.
*/
void glutGameMouseMove(int x, int y)
{
	int delta_x = 0, delta_y = 0;
	delta_x = x - mouse_x_old;
	delta_y = y - mouse_y_old;
	if(mouse_state_left)
	{
		if((delta_x!=0)||(delta_y!=0))
		{
			glutGameRotateCamera(((double)delta_x/100),((double)delta_y/100));
		}
	}
	//#Save location in old value
	mouse_x_old = x;
	mouse_y_old = y;
}

/*
* Function: void glutGameKeyboardInit()
* -----------------------------
* Initialise the keyboard functions.
* When GLUTGAME_CONTROL_USEREG is defined
* this will enable keyboard state tracking on the used buttons
* Returns nothing
*/
void glutGameKeyboardInit()
{
	#ifdef GLUTGAME_CONTROL_USEREG
	glutIgnoreKeyRepeat(0x01);
	glutKeyboardFunc(glutGameKeyboardPressed);
	glutKeyboardUpFunc(glutGameKeyboardReleased);
	#else
	glutKeyboardFunc(glutGameKeyboardPressed);
	#endif
}

void glutGameKeyboardSetFunc(void (*fnc)(unsigned int))
{
	keyboard_fnc = fnc;
}

/*
* Function: void glutGameKeyboardPressed()
* -----------------------------
* Function to track when a key is pressed.
* Returns nothing
*/
void glutGameKeyboardPressed(unsigned  char key, int x, int y)
{
	#ifdef GLUTGAME_DEBUG_INFO
	printf("[GLUTGAME][KEYBOARD] Key pressed :%c\n",key);
	#endif
	#ifdef GLUTGAME_CONTROL_USEREG
	switch(key)
	{
		case GLUTGAME_CONTROL_FORW:
			if(controlEnable) GLUTGAME_CONTROL_REG = GLUTGAME_CONTROL_REG | (0x01 << 0);
			break;
		case GLUTGAME_CONTROL_BACK:
			if(controlEnable) GLUTGAME_CONTROL_REG = GLUTGAME_CONTROL_REG | (0x01 << 1);
			break;
		case GLUTGAME_CONTROL_LEFT:
			if(controlEnable) GLUTGAME_CONTROL_REG = GLUTGAME_CONTROL_REG | (0x01 << 2);
			break;
		case GLUTGAME_CONTROL_RIGHT:
			if(controlEnable) GLUTGAME_CONTROL_REG = GLUTGAME_CONTROL_REG | (0x01 << 3);
			break;
		case GLUTGAME_CONTROL_UP:
			if(controlEnable) GLUTGAME_CONTROL_REG = GLUTGAME_CONTROL_REG | (0x01 << 4);
			break;
		case GLUTGAME_CONTROL_DOWN:
			if(controlEnable) GLUTGAME_CONTROL_REG = GLUTGAME_CONTROL_REG | (0x01 << 5);
			break;
		case GLUTGAME_CONTROL_AXIS:
			GLUTGAME_CONTROL_REG = GLUTGAME_CONTROL_REG ^ (0x01 << 6);	//Toggle function
			break;
	}
	#else
	glutGameMoveCamera(key);
	#endif
	if(keyboard_fnc != 0x00)(*keyboard_fnc)(key);
}

/*
* Function: void glutGameKeyboardReleased()
* -----------------------------
* Function to track when a key is released.
* Returns nothing
*/
void glutGameKeyboardReleased(unsigned char key, int x, int y)
{
	#ifdef GLUTGAME_DEBUG_INFO
	printf("[GLUTGAME][KEYBOARD] Key released :%c\n",key);
	#endif
	switch(key)
	{
		case GLUTGAME_CONTROL_FORW:
			if(controlEnable) GLUTGAME_CONTROL_REG = GLUTGAME_CONTROL_REG & (~(0x01 << 0));
			break;
		case GLUTGAME_CONTROL_BACK:
			if(controlEnable) GLUTGAME_CONTROL_REG = GLUTGAME_CONTROL_REG & (~(0x01 << 1));
			break;
		case GLUTGAME_CONTROL_LEFT:
			if(controlEnable) GLUTGAME_CONTROL_REG = GLUTGAME_CONTROL_REG & (~(0x01 << 2));
			break;
		case GLUTGAME_CONTROL_RIGHT:
			if(controlEnable) GLUTGAME_CONTROL_REG = GLUTGAME_CONTROL_REG & (~(0x01 << 3));
			break;
		case GLUTGAME_CONTROL_UP:
			if(controlEnable) GLUTGAME_CONTROL_REG = GLUTGAME_CONTROL_REG & (~(0x01 << 4));
			break;
		case GLUTGAME_CONTROL_DOWN:
			if(controlEnable) GLUTGAME_CONTROL_REG = GLUTGAME_CONTROL_REG & (~(0x01 << 5));
			break;
		case GLUTGAME_CONTROL_AXIS:
			//GLUTGAME_CONTROL_REG = GLUTGAME_CONTROL_REG & (~(0x01 << 6));
			break;
	}
}

/*
* Function: void glutGameKeyboardReleased()
* -----------------------------
* Function to track when a key is released.
* Returns nothing
*/
void glutGameMoveCamera(unsigned char key)
{
	//#HACK : overwrite xl, zl globals with fix xz plane values
	double xl = sin((*rotation_lr));
	double zl = -cos((*rotation_lr));
	#ifdef GLUTGAME_CONTROL_USEREG
		if(GLUTGAME_CONTROL_REG & (0x01 << 0))		//GLUTGAME_CONTROL_FORW
		{
			(*xPos) += xl * GLUTGAME_PLAYER_BASESPEED * speed_mul;
			(*zPos) += zl * GLUTGAME_PLAYER_BASESPEED * speed_mul;
		}
		if(GLUTGAME_CONTROL_REG & (0x01 << 1))		//GLUTGAME_CONTROL_BACKW
		{
			(*xPos) -= xl * GLUTGAME_PLAYER_BASESPEED * speed_mul;
			(*zPos) -= zl * GLUTGAME_PLAYER_BASESPEED * speed_mul;
		}
		if(GLUTGAME_CONTROL_REG & (0x01 << 2))		//GLUTGAME_CONTROL_LEFT
		{
			(*xPos) += zl * GLUTGAME_PLAYER_BASESPEED * speed_mul;
			(*zPos) -= xl * GLUTGAME_PLAYER_BASESPEED * speed_mul;
		}
		if(GLUTGAME_CONTROL_REG & (0x01 << 3))		//GLUTGAME_CONTROL_RIGHT
		{
			(*xPos) -= zl * GLUTGAME_PLAYER_BASESPEED * speed_mul;
			(*zPos) += xl * GLUTGAME_PLAYER_BASESPEED * speed_mul;
		}
		if(GLUTGAME_CONTROL_REG & (0x01 << 4))		//GLUTGAME_CONTROL_UP
		{
			(*yPos) += GLUTGAME_PLAYER_BASESPEED * speed_mul;
		}
		if(GLUTGAME_CONTROL_REG & (0x01 << 5))		//GLUTGAME_CONTROL_DOWN
		{
			(*yPos) -= GLUTGAME_PLAYER_BASESPEED * speed_mul;
		}
		//glutTimerFunc(1,glutGameMoveCamera,0);
	#else
	switch(key)
	{
		case GLUTGAME_CONTROL_LEFT :
			(*xPos) += (*zl) * GLUTGAME_PLAYER_BASESPEED * speed_mul;
			(*zPos) -= (*xl) * GLUTGAME_PLAYER_BASESPEED * speed_mul;
			break;
		case GLUTGAME_CONTROL_RIGHT :
			(*xPos) -= (*zl) * GLUTGAME_PLAYER_BASESPEED * speed_mul;
			(*zPos) += (*xl) * GLUTGAME_PLAYER_BASESPEED * speed_mul;
			break;
		case GLUTGAME_CONTROL_FORW :
			(*xPos) += (*xl) * GLUTGAME_PLAYER_BASESPEED * speed_mul;
			(*zPos) += (*zl) * GLUTGAME_PLAYER_BASESPEED * speed_mul;
			break;
		case GLUTGAME_CONTROL_BACK :
			(*xPos) -= (*xl) * GLUTGAME_PLAYER_BASESPEED * speed_mul;
			(*zPos) -= (*zl) * GLUTGAME_PLAYER_BASESPEED * speed_mul;
			break;
		case GLUTGAME_CONTROL_UP :
			(*yPos) += GLUTGAME_PLAYER_BASESPEED * speed_mul;
			break;
		case GLUTGAME_CONTROL_DOWN :
			(*yPos) -= GLUTGAME_PLAYER_BASESPEED * speed_mul;
			break;
		default:
			break;
	}
	#endif
}
