#include "ball.h"
#include "cube.h"
#include "material.h"
#include "GLinclude.h"

ball::ball(glm::vec3 loc, float r) :object(loc){
    this->loc = loc, this->r = r;
}
void ball::draw(MATERIAL m, float r, float g, float b){
    SetMaterial(m, r, g, b);
    glPushMatrix();
    glScalef(this->r, this->r, this->r);
    glTranslatef(loc.x, loc.y, loc.z);
    glutSolidSphere(1.0, 20, 20);
    glPopMatrix();
}
bool ball::isCollide(object &obj){
    cube *c = dynamic_cast<cube *>(&obj);
    if(c != nullptr){
        glm::vec2 cx = glm::vec2(loc.x - r, loc.x + r);
        glm::vec2 cy = glm::vec2(loc.y - r, loc.y + r);
        glm::vec2 cz = glm::vec2(loc.z - r, loc.z + r);
        glm::vec2 cx2 = glm::vec2(c->getLoc().x - c->getL() / 2.0f, c->getLoc().x + c->getL() / 2.0f);
        glm::vec2 cy2 = glm::vec2(c->getLoc().y - c->getW() / 2.0f, c->getLoc().y + c->getW() / 2.0f);
        glm::vec2 cz2 = glm::vec2(c->getLoc().z - c->getH() / 2.0f, c->getLoc().z + c->getH() / 2.0f);
        if(((cx.x - cx2.x < ESP && cx2.x - cx.y < ESP) || (cx2.x - cx.x < ESP && cx.x - cx2.y < ESP)) &&
            ((cy.x - cy2.x < ESP && cy2.x - cy.y < ESP) || (cy2.x - cy.x < ESP && cy.x - cy2.y < ESP)) &&
            ((cz.x - cz2.x < ESP && cz2.x - cz.y < ESP) || (cz2.x - cz.x < ESP && cz.x - cz2.y < ESP))){
            return 1;
        }
        else return 0;
    }
    ball *b = dynamic_cast<ball *>(&obj);
    if(b != nullptr){
        glm::vec2 bx = glm::vec2(b->getLoc().x - b->getR(), b->getLoc().x + b->getR());
        glm::vec2 by = glm::vec2(b->getLoc().y - b->getR(), b->getLoc().y + b->getR());
        glm::vec2 bz = glm::vec2(b->getLoc().z - b->getR(), b->getLoc().z + b->getR());
        glm::vec2 cx = glm::vec2(loc.x - r, loc.x + r);
        glm::vec2 cy = glm::vec2(loc.y - r, loc.y + r);
        glm::vec2 cz = glm::vec2(loc.z - r, loc.z + r);
        if(((cx.x - bx.x < ESP && bx.x - cx.y < ESP) || (bx.x - cx.x < ESP && cx.x - bx.y < ESP)) &&
            ((cy.x - by.x < ESP && by.x - cy.y < ESP) || (by.x - cy.x < ESP && cy.x - by.y < ESP)) &&
            ((cz.x - bz.x < ESP && bz.x - cz.y < ESP) || (bz.x - cz.x < ESP && cz.x - bz.y < ESP))){
            return 1;
        }
        else return 0;
    }
    return 0;
}

float ball::getR(){
    return r;
}