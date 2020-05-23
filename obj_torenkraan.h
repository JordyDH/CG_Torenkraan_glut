#ifndef __OBJ_TORENKRAAN_H__
#define __OBJ_TORENKRAAN_H__
#include "glutGameLibs.h"

typedef struct{
	glutGameObjectobject *obj_kolom;	//Pointer to kolom object (can be used to translate the crane)
	glutGameObjectobject *obj_disk;		//Pointer to rotating disk
	glutGameObjectobject *obj_last;		//Pointer to last (can be used to translate the last on the crane)
} Kraan_struct;

Kraan_struct* torenkraan_AllocObj(double x, double y, double z, uint16_t height);
void torenkraan_kolom(glutGameObjectobject *self);
void torenkraan_disk(glutGameObjectobject *self);
void torenkraan_kabine(glutGameObjectobject *self);

#endif
