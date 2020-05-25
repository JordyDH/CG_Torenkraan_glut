#ifndef __GLUTGAME_CONFIG_H__
#define __GLUTGAME_CONFIG_H__

#define GLUTGAME_VERSION "dev.0.3"
//#define  GLUTGAME_DEBUG_INFO
//////////////////////////////////// CONTROLS ////////////////////////////////////
#define GLUTGAME_CONTROL_FORW	'x'	//Moves the camera forwards	--Bit 0 in control reg
#define GLUTGAME_CONTROL_BACK	'X'	//Moves the camera backwards	--Bit 1 in control reg
#define GLUTGAME_CONTROL_LEFT	'z'	//Moves the camera left		--Bit 2 in control reg
#define GLUTGAME_CONTROL_RIGHT	'Z'	//Moves the camera right	--Bit 3 in control reg
#define GLUTGAME_CONTROL_UP	'u'	//Moves the camera up		--Bit 4 in control reg
#define GLUTGAME_CONTROL_DOWN	'U'	//Moves the camera down		--Bit 5 in control reg
#define GLUTGAME_CONTROL_AXIS	'i'	//Renders the axis		--Bit 6 in control reg
#define GLUTGAME_CONTROL_USEREG		//Define to use state tracking on keyboard
#define GLUTGAME_CONTROL_TIMER	1	//time in ms to check input of keyboard control bits
//////////////////////////////////// PLAYER MODEL ////////////////////////////////////
#define GLUTGAME_PLAYER_HEIGHT		1.0	//Default player height, possition of the camera
#define GLUTGAME_PLAYER_BASESPEED	0.2	//Default step size
#define GLUTGAME_PLAYER_NEARSIGHT	0.01	//Near sight param of the camera
#define GLUTGAME_PLAYER_FARSIGHT	1000	//Far sight param of the camera

//////////////////////////////////// RENDER SETTINGS ////////////////////////////////////
#define GLUTGAME_RENDER_FAST			//Renders the scene as fast a possible.
 #define GLUTGAME_RENDER_DUBBELBUFFER		//Enable dubbel buffering for render.
#define GLUTGAME_SYSTICK_INTERVAL	25	//Interval between systicks in ms
#define GLUTGAME_RENDER_LIMITER		17	//Render limiter, time between frames in ms

#endif
