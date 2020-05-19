#include "glutGameObjects.h"

#ifndef __GLUTGAME_CONTROL_H__
#define __GLUTGAME_CONTROL_H__

void glutGameControlInit(glutGameObjectplayer *player);
uint64_t glutGameControlGetRegister();
void glutGameControlUpdate();
void glutGameMouseInit();
void glutGameMouseKeys(int button, int state, int x, int y);
void glutGameMouseMove(int x, int y);
void glutGameKeyboardInit();
void glutGameKeyboardPressed(unsigned  char key, int x, int y);
void glutGameKeyboardReleased(unsigned char key, int x, int y);
void glutGameMoveCamera(unsigned char key);
void glutGameControlEnable();
void glutGameControlDisable();
void glutGameKeyboardSetFunc(void (*fnc)(unsigned int));

#endif



