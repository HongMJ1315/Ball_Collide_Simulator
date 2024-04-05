#ifndef CUBE_H
#define CUBE_H

#include "object.h"

class cube : public object{
private:
    float l, w, h;
public:
    cube(glm::vec3, float, float, float);
    void draw(MATERIAL, float, float, float);
    void draw(MATERIAL);
    bool isCollide(object &);
    float getL();
    float getW();
    float getH();
};

#endif