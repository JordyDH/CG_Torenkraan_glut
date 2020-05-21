#include <stdio.h>
#include <stdlib.h>
#include "obj_torenkraan.h"

#define TK_KOLOM_STUD SIZE

void renderBalk(double x, double y, double z, double xsize,double ysize,double zsize, double angle_x, double angle_y)
{
	glBegin(GL_QUADS);
		glVertex3d(x,y,z);
		glVertex3d(x,y+ysize/cos(angle_y),z);
		glVertex3d(x+xsize,y+ysize/cos(angle_y),z);
		glVertex3d(x+xsize,y,z);

		glVertex3d(x,y,z+zsize);
		glVertex3d(x,y+ysize/cos(angle_y),z+zsize);
		glVertex3d(x+xsize,y+ysize/cos(angle_y),z+zsize);
		glVertex3d(x+xsize,y,z+zsize);
	glEnd();
}

void torenkraan_kolom(glutGameObjectobject *self)
{
	glBegin(GL_QUADS);
}
