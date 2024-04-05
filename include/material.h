#ifndef MATERIAL_H
#define MATERIAL_H

#include "GLinclude.h"

enum MATERIAL{
    OBJECT, WALL, FLOOR
};

void SetMaterial(MATERIAL, float, float, float);

#endif
