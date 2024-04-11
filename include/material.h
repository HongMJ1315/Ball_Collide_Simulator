#ifndef MATERIAL_H
#define MATERIAL_H

#include "GLinclude.h"

enum MATERIAL{
    M_OBJECT, M_WALL, M_FLOOR, M_NONE
};

void SetMaterial(MATERIAL, float, float, float);

#endif
