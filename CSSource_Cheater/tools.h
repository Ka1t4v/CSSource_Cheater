#pragma once
#include "head.h"

DWORD getpidbyname(const char* ProcessName);
HMODULE getmodulehandlebyname(const char* ModuleName, DWORD pid);
float getbotangle_horizontal(float subx, float suby);
float getbotangle_vertical(float subz, float dis_xy);
float gethorizontalgap(float x, float y);
float getverticalgap(float x, float y);
float getxpoint(float b, float fov, float image_x);
float getypoint(float c, float fov_y, float image_x, float image_y);