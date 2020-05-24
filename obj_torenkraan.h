#ifndef __OBJ_TORENKRAAN_H__
#define __OBJ_TORENKRAAN_H__
#include "glutGameLibs.h"

typedef struct{
	glutGameObjectobject *obj_kolom;	//Pointer to kolom object (can be used to translate the crane)
	glutGameObjectobject *obj_disk;		//Pointer to rotating disk
	glutGameObjectobject *obj_disk2;	//Pointer to rotating disk
	glutGameObjectobject *obj_cab;		//Pointer to kabine
	glutGameObjectobject *obj_arm;		//Pointer to arm
	glutGameObjectobject *obj_last;		//Pointer to last
	glutGameObjectlight  *light;		//Pointer to light model
	uint8_t flag_anim_lastH;		//Flag for Horizontal animation
	uint8_t flag_anim_lastV;		//Flag for Vertical animation
	uint8_t flag_anim_cab;			//Flag for rotation animation
	uint32_t flag_localaxis;		//Flag to enable local axis for each object
	double anim_R;				//Value of rotation
	double anim_H;				//Value of Horizontal
	double anim_V;				//Value of Vertical
	float shinniness;			//Global object shinniness value
} Kraan_struct;

Kraan_struct* torenkraan_AllocObj(double x, double y, double z, uint16_t height, uint16_t lenght, GLenum Light_ID);
void torenkraan_kolom(glutGameObjectobject *self);
void torenkraan_disk(glutGameObjectobject *self);
void torenkraan_kabine(glutGameObjectobject *self);
void torenkraan_arm(glutGameObjectobject *self);
void torenkraan_last(glutGameObjectobject *self);

#endif
