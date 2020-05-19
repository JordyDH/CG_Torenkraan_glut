#include "glutGameObjects.h"

#ifndef __GLUTGAME_CORE_H__
#define __GLUTGAME_CORE_H__

void glutGameInit();
void glutGameMainLoop();
void glutGameSetIdleFunc(void (*fnc)());
void glutGameIdle();
void glutGameSystickService(unsigned int systick_old);
unsigned int glutGameSystickGet();
void glutGameRenderLocalAxis();
void glutGameRenderOnScreenInfo();
glutGameObjectplayer *glutGameMainPlayerGet();

#endif
