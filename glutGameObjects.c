////////////////////////////////////////////////////////////////////
//
//	glutGameObjects by Jordy de Hoon
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

//////////////////////////////////// [LIB VARS] /////////////////////////////////////////////////////
static glutGameObjectList *objectList = 0x00;
//////////////////////////////////// [GLUTGAME OBJECT FUNCTIONS] ////////////////////////////////////

/*
* Function: glutGameObjectList *glutGameObjectsGetList()
* -----------------------------
* Return the adress off the objectList
*/
glutGameObjectList *glutGameObjectsGetList()
{
	return objectList;
}

/*
* Function: glutGameObjectobject *glutGameObjectsAlloc_object()
* -----------------------------
* Allocate memory and append it to the objectList
*/
glutGameObjectobject *glutGameObjectsAlloc_object()
{
	glutGameObjectobject *p;
	p = (glutGameObjectobject*)calloc(1,sizeof(glutGameObjectobject));
	glutGameObjectsAddList(p,GLUTGAME_STRUCTID_OBJECT);
	return p;
}

/*
* Function: glutGameObjectplayer *glutGameObjectsAlloc_player()
* -----------------------------
* Allocate memory for a player model and append it to the objectList
*/
glutGameObjectplayer *glutGameObjectsAlloc_player()
{
	glutGameObjectplayer *p;
	p = (glutGameObjectplayer*)calloc(1,sizeof(glutGameObjectplayer));
	glutGameObjectsAddList(p,GLUTGAME_STRUCTID_PLAYER);
	return p;
}

/*
* Function: glutGameObjectplayer *glutGameObjectsAlloc_player()
* -----------------------------
* Allocate memory for a player model and append it to the objectList
*/
glutGameObjectlight *glutGameObjectsAlloc_light()
{
	glutGameObjectlight *p;
	p = (glutGameObjectlight*)calloc(1,sizeof(glutGameObjectlight));
	glutGameObjectsAddList(p,GLUTGAME_STRUCTID_LIGHT);
	return p;
}

/*
* Function: void glutGameObjectsAddList(void *element, uint8_t id)
* -----------------------------
* Append a given object to the objectList wwith ID
*/
void glutGameObjectsAddList(void *element, uint8_t id)
{
	//If there are no elements in the list --> Add one empty element, first element
	if(objectList == 0)
		objectList = calloc(1,sizeof(glutGameObjectList));
	//Place a moving pointer at the start of the list
	glutGameObjectList *p;
	p  = objectList;
	//Search for the end of the list
	while((*p).next != 0x00)
		p = (*p).next;
	//End found
	(*p).struct_id = id;
	(*p).object = element;	//Load element in list.
	(*p).next = calloc(1,sizeof(glutGameObjectList));	//Allocate new space for a new entry (future)
}
