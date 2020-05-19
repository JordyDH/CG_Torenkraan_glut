#ifndef __GLUTGAME_CAMERA_H__
#define __GLUTGAME_CAMERA_H__
void glutGameCameraInit(glutGameObjectplayer *player, double x, double y, double z);
void glutGameCameraRender();
void glutGameCameraPlace(double x, double y, double z, double rotation_xz, double rotation_y);
void glutGameRotateCamera(double dxa, double dya);
#endif
