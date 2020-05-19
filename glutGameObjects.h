#ifndef __GLUTGAME_OBJECTS_H__
#define __GLUTGAME_OBJECTS_H__
//Structs for boundingboxes, objects and players. [EXPERIMENTAL]
typedef struct {
	double xmin, ymin, zmin;	//First point of the bounding box
	double xmax, ymax, zmax;	//Seccond point of the boinding box
} boundingbox;

typedef struct {
	double x,y,z;
} vector3;

#define GLUTGAME_STRUCTID_OBJECT	1
typedef struct object {
	struct object *linkedobj;		//Linked Object: move to default 0,0,0 point to the values of the linked object
	void (*callback)(struct object *self);	//Linked callback function (ex 3dmodel description)
	uint16_t id;				//Id of the objects (handy for VBO's)
	uint16_t state;				//Bit register to store states or oher factors
	double x, y, z;				//x y z translate of the object
	double rot_x, rot_y, rot_z;		//x y z rotation of the object
	uint8_t physics;			//Physics enable register
	boundingbox bb;				//bounding box
	vector3 velocity;			//velocity vector
	double mass;				//mass value of the object
	double lf_mem[3];			//Double memory space
	uint32_t i_mem[3];			//Int memory space
} glutGameObjectobject;

#define GLUTGAME_STRUCTID_PLAYER	2
typedef struct {
	glutGameObjectobject base;	//Player is a base object
	double xl, yl, zl;		//Looking point of the player
	double rotation_xz;		//Head rotation (left/right) of the player
	double rotation_y;		//Head rotation (up/down) of the player
} glutGameObjectplayer;

typedef struct ObjectListnode {		//Linked list to store pointers to objects (or anly element)
	uint8_t struct_id;		//Type id, what type of struct is this pointing to.
	void *object;			//Void pointer to any entry of the memory
	struct ObjectListnode *next;	//The next object in the list.

} glutGameObjectList;

glutGameObjectList *glutGameObjectsGetList();
glutGameObjectobject *glutGameObjectsAlloc_object();
glutGameObjectplayer *glutGameObjectsAlloc_player();
void glutGameObjectsAddList(void *element, uint8_t id);


#endif


