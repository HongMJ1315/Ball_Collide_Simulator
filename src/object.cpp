#include "material.h"
#include "GLinclude.h"
#include "object.h"
#include <iostream>


object::object(glm::vec3 loc, glm::vec3 v, glm::vec3 a, float m){
    this->loc = loc;
    this->v = v;
    this->a = a;
}

object::object(){
    this->loc = glm::vec3(0.0f, 0.0f, 0.0f);
    this->v = glm::vec3(0.0f, 0.0f, 0.0f);
    this->a = glm::vec3(0.0f, 0.0f, 0.0f);
}

object::object(glm::vec3 loc){
    this->loc = loc;
    this->v = glm::vec3(0.0f, 0.0f, 0.0f);
    this->a = glm::vec3(0.0f, 0.0f, 0.0f);
}

void object::draw(MATERIAL m, float r, float g, float b){
    std::cout << "object draw" << std::endl;
}

void object::draw(MATERIAL m){
    std::cout << "object draw" << std::endl;
}

glm::vec3 object::getLoc(){
    return this->loc;
}

bool object::isCollide(object &obj){
    std::cout << "object collide" << std::endl;
    return 0;
}

void object::update(float dt){
    this->v += this->a * dt;
    this->loc += this->v * dt;
}

void object::setV(glm::vec3 v){
    this->v = v;
}

void object::setA(glm::vec3 a){
    this->a = a;
}

void object::setName(std::string name){
    this->name = name;
}

void object::setLoc(glm::vec3 loc){
    this->loc = loc;
}

void object::setM(float m){
    this->m = m;
}

glm::vec3 object::getV(){
    return this->v;
}

glm::vec3 object::getA(){
    return this->a;
}

std::string object::getName(){
    return this->name;
}

float object::getM(){
    return this->m;
}

object::~object(){
    std::cout << "object destruct" << std::endl;
}

void object::setColor(glm::vec3 color){
    this->color = color;
}