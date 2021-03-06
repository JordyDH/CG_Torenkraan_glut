#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "glutGameLibs.h"
#include "InitJPG.h"
#include "obj_torenkraan.h"
#include "materiaal.h"

#define NO_FANCY

#define MAX 3
static glutGameObjectlight *lichten[MAX];	//Pointer to light models for the global scene
static Kraan_struct *kranen[MAX];		//Pointer array to store aantal kranen
static Kraan_struct *kraan_select = 0x00;	//Pointer to select a kraan in the array
static uint32_t last_systick = 0;		//Local variable to store last systick
static uint8_t flag_smooth = 0;			//Flag for smooth lighting
uint8_t kraan_actief[MAX] = {0,0,0};		//The active flag for this kraan
static double kraan_location[MAX][3] = {	//Location data to enable 3 kranen
	{0.0, 0.0, 0.0},
	{15.0, 0.0, -10.0},
	{-10.0, 0.0, -15.0}
	};
static uint16_t kraan_size[MAX][2] = {		//The height and lenght parameters
	{5, 10},
	{5, 8},
	{7, 9}
	};
static GLenum kraan_light[MAX] = {		//Light id's for the kabine
	GL_LIGHT7, GL_LIGHT6, GL_LIGHT5};

static double camera_locations[3][5] = {	//3 predefine locations for the camera
	{12.0, 26.0, 50.00, 0.00, -0.36},
	{12.0, -8.0, -42.0, 3.157, 0.23},
	{-14.0, 18.5, 0.00, 2.21, -0.49}
	};

// INIT functie -----------------------------------------------------------------
void myinit(void)
{
	glClearColor(0,0,0,0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	for(uint16_t i = 0; i < MAX; i++) kranen[i] = 0x00;
	glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
}

// KEYBOARD functie -----------------------------------------------------------------
void keyboard(unsigned int key)
{
	//With 1/2/3 a specific crane can be selected ---------------------------------
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
	// With 4 5 6 you can change the perspective mode ---------------------------
	if(key == '4') glutGameRenderSetPerspective();
	if(key == '5') glutGameRenderSetOrtho();
	if(key == '6') glutGameRenderSetFrustum();
	// With 7 8 9 you can change the location of the camera ----------------------
	if( key == '7')	glutGameCameraPlace(camera_locations[0][0], camera_locations[0][1], camera_locations[0][2], camera_locations[0][3], camera_locations[0][4]);
	if( key == '8')	glutGameCameraPlace(camera_locations[1][0], camera_locations[1][1], camera_locations[1][2], camera_locations[1][3], camera_locations[1][4]);
	if( key == '9')	glutGameCameraPlace(camera_locations[2][0], camera_locations[2][1], camera_locations[2][2], camera_locations[2][3], camera_locations[2][4]);
	//Kraan specifieke controle --------------------------------------------------
	if(kraan_select != 0x00)
	{
		if(key == 'l')	//Set quad mode to wireframe or fill
		{
			(*(*kraan_select).obj_cab).i_mem[0] = ~(*(*kraan_select).obj_cab).i_mem[0];
			(*(*kraan_select).obj_disk).i_mem[0] = ~(*(*kraan_select).obj_disk).i_mem[0];
			(*(*kraan_select).obj_disk2).i_mem[0] = ~(*(*kraan_select).obj_disk2).i_mem[0];
			(*(*kraan_select).obj_last).i_mem[1] = ~(*(*kraan_select).obj_last).i_mem[1];
			(*(*(*kraan_select).obj_arm).p_mem[0]) = ~(*(*(*kraan_select).obj_arm).p_mem[0]);
		}
		if(key == 'r')	//Control rotation animation
			(*kraan_select).flag_anim_cab = ~(*kraan_select).flag_anim_cab;
		if(key == 'g')	//Control horizontal animation
			(*kraan_select).flag_anim_lastH = ~(*kraan_select).flag_anim_lastH;
		if(key == 'G')	//Control vertical animation
			(*kraan_select).flag_anim_lastV = ~(*kraan_select).flag_anim_lastV;
		if( key == 'd') (*(*kraan_select).light).enable = 1;		//Enable light on the crane
		if( key == 'D') (*(*kraan_select).light).enable = 0;		//Disable light on the crane
		if( key == 'v') (*(*kraan_select).light).spot_cutoff += 1;	//Increment spot angle with 1°
		if( key == 'V') (*(*kraan_select).light).spot_cutoff -= 1;	//Decrement spot angle with 1°
		if( key == 'w') (*(*kraan_select).light).spot_exponent += 5;	//Increment spot exponent with 5
		if( key == 'W') (*(*kraan_select).light).spot_exponent -= 5;	//Decrement spot exponent with 5
		if( key == 'e') (*kraan_select).shinniness += 5;		//Decrement spot exponent with 5
		if( key == 'E') (*kraan_select).shinniness -= 5;		//Decrement spot exponent with 5
		if( key == 'j') (*kraan_select).flag_localaxis = 1;		//Enable local axis render
		if( key == 'J') (*kraan_select).flag_localaxis = 0;		//Disable local axis render
		if( key == 'h') (*(*kraan_select).light).spot_direction[1] += 0.5;		//Increment y of spot direction
		if( key == 'H') (*(*kraan_select).light).spot_direction[1] -= 0.5;		//Decrement y of spot direction
		if( key == 'k') (*(*kraan_select).obj_cab).i_mem[1] = ~(*(*kraan_select).obj_cab).i_mem[1];	//Mesh point rendering
		if( key == 'f') (*(*kraan_select).obj_cab).i_mem[2] = ~(*(*kraan_select).obj_cab).i_mem[2];	//Control transparantie
	}
	if( key == 'a') (*lichten[0]).enable = 1;	//Enable first light
	if( key == 'A') (*lichten[0]).enable = 0;	//Disable first light
	if( key == 'b') (*lichten[1]).enable = 1;	//Enable Second light
	if( key == 'B') (*lichten[1]).enable = 0;	//Disable Second light
	if( key == 'c') (*lichten[2]).enable = 1;	//Enable Third light
	if( key == 'C') (*lichten[2]).enable = 0;	//Disable Third light
	if( key == 's') flag_smooth = 1;		//Enable smooth lighting
	if( key == 'S') flag_smooth = 0;		//Enable flatt lighting
	if( key == 'n')					//Add one more kraan to the scene
	{
		for(uint8_t i = 0; i < MAX; i++)
		{
			//Search for empty spot
			if(kraan_actief[i] == 0x00)
			{
				//Allocate a kraan model
				kranen[i] = torenkraan_AllocObj(kraan_location[i][0],kraan_location[i][1],kraan_location[i][2],kraan_size[i][0],kraan_size[i][1],kraan_light[i]);
				kraan_actief[i] = 1;
				kraan_select = kranen[i];
				printf("Kraan geplaats met id:%d druk op %d om te selecteren\n",i,i);
				break;
			}
		}
	}
}

// Animatie functie -----------------------------------------------------------------
void animation()
{
	if( last_systick == glutGameSystickGet()) return;
	last_systick = glutGameSystickGet();
	for(uint16_t i = 0; i < MAX; i++)
	{
		if(kranen[i] == 0x00) continue;	//Skip this crane
		//Cab movement if flag is set
		if((*kranen[i]).flag_anim_cab)
		{
 			(*(*kranen[i]).obj_disk).rot_y = 30 * sin((*kranen[i]).anim_R/10.0);
			(*kranen[i]).anim_R += 0.1;
		}
		if((*kranen[i]).flag_anim_lastH)
		{
			(*(*kranen[i]).obj_last).lf_mem[0] = 10 * fabs(sin((*kranen[i]).anim_H/10.0));
			(*kranen[i]).anim_H += 0.1;
		}
		if((*kranen[i]).flag_anim_lastV)
		{
			(*(*kranen[i]).obj_last).lf_mem[1] = 5 * fabs(sin((*kranen[i]).anim_V/10.0));
			(*kranen[i]).anim_V += 0.1;
		}
	}
}
// WORLD functie -----------------------------------------------------------------
void world()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	if(flag_smooth) glShadeModel(GL_SMOOTH);
	else glShadeModel(GL_FLAT);
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
	//Render light
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glutGameRenderAllLights();
	//Render static elements (floors)

	//Render objecten
	glutGameRenderAllObjects();
	glDisable(GL_LIGHTING);
}

// main functie -----------------------------------------------------------------
int main( int argc, char * argv[])
{
	//backend Glut init ---------------------------------------------------------
	glutInit(&argc, argv);

	#ifndef NO_FANCY
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	#else
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	#endif

	glutInitWindowPosition(0,0);
	glutInitWindowSize(680, 480);
	glutCreateWindow("JORDY DE HOON - TORENKRAAN");
	//GlutGame Engine init ------------------------------------------------------
	myinit();
	glutGameInit();
	glutGameKeyboardSetFunc(keyboard);
	glutGameControlEnable();
	glutGameRenderSceneSet(world);
	glutGameSetIdleFunc(animation);
	//Define Global light sources for the scene ---------------------------------
	// [ Licht 0 ]
	lichten[0] = glutGameObjectsAlloc_light();
	(*lichten[0]).ambient  = wit_ambient;
	(*lichten[0]).id = GL_LIGHT0;
	(*lichten[0]).enable = 1;
	(*lichten[0]).debug = 1;
	(*lichten[0]).z = 10.0;
	(*lichten[0]).y = 5.0;
	(*lichten[0]).w = 0.0;
	// [ Licht 1 ]
	lichten[1] = glutGameObjectsAlloc_light();
	(*lichten[1]).diffuse  = blauw_diffuse;
	(*lichten[1]).id = GL_LIGHT1;
	(*lichten[1]).enable = 1;
	(*lichten[1]).debug = 1;
	(*lichten[1]).z = 10;
	(*lichten[1]).z = -10;
	(*lichten[1]).y = 10;
	(*lichten[1]).w = 0.0;
	// [ Licht 2 ]
	lichten[2] = glutGameObjectsAlloc_light();
	(*lichten[2]).specular  = rood_specular;
	(*lichten[2]).id = GL_LIGHT2;
	(*lichten[2]).enable = 1;
	(*lichten[2]).debug = 1;
	(*lichten[2]).z = -10;
	(*lichten[2]).z = -10;
	(*lichten[2]).y = -10;
	(*lichten[2]).w = 0.0;
	//Place camera at the default location came_location[0] ---------------------
	glutGameCameraPlace(camera_locations[0][0], camera_locations[0][1], camera_locations[0][2], camera_locations[0][3], camera_locations[0][4]);
	//Start all services and run engine -----------------------------------------
	glutGameMainLoop();
}
