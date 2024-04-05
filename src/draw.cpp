#include "draw.h"


int keyState[GLFW_KEY_LAST] = { 0 };
int directionKey[4] = { 0 };
void glInit(){
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glEnable(SUN_LIGHT);
}

void initObjects(std::vector<object *> &objs){
    objs.push_back(new cube(glm::vec3(0, 0, 0), 1, 1, 1));
    objs.push_back(new ball(glm::vec3(5, 0, 0), 5 / 2.0f));
    objs.push_back(new ball(glm::vec3(3, 2, 0), 1 / 2.0f));
    objs.push_back(new ball(glm::vec3(7, 0, 0), 2 / 2.0f));
    objs.push_back(new ball(glm::vec3(2, 3, 0), 4 / 2.0f));

    objs[0]->setV(glm::vec3(1, 0, 0));
    objs[1]->setV(glm::vec3(0, 2 * 10, 2.5 * 10));
    objs[2]->setV(glm::vec3(0, -3 * 10, 3.5 * 10));
    objs[3]->setV(glm::vec3(0, -1 * 10, -0.5 * 10));
    objs[4]->setV(glm::vec3(0, 0, 1.0 * 10));

    objs[0]->setM(100.0f);
    objs[1]->setM(5.0f);
    objs[2]->setM(1.0f);
    objs[3]->setM(2.0f);
    objs[4]->setM(4.0f);
}

void move(float dx, float dy, float dz, glm::vec3 &frontPos, glm::vec3 &cameraPos){
    frontPos.x += dx;
    frontPos.y += dy;
    frontPos.z += dz;
    cameraPos.x += dx;
    cameraPos.y += dy;
    cameraPos.z += dz;
}

void update(glm::vec3 &frontPos, glm::vec3 &cameraPos){
    // front and back
    glm::vec3 dl = glm::normalize(glm::vec3(frontPos.x, 0, frontPos.z) - glm::vec3(cameraPos.x, 0, cameraPos.z)) * MOVE_SPEED;
    // right and left
    glm::vec3 dr = glm::normalize(glm::cross(frontPos - cameraPos, glm::vec3(0, 1, 0))) * MOVE_SPEED;
    if(keyState['W']){
        cameraPos += dl * MOVE_SPEED;
        frontPos += dl * MOVE_SPEED;
    }
    if(keyState['S']){
        cameraPos -= dl * MOVE_SPEED;
        frontPos -= dl * MOVE_SPEED;
    }
    // Left
    if(keyState['A']){
        cameraPos -= dr * MOVE_SPEED;
        frontPos -= dr * MOVE_SPEED;
    }
    // Right
    if(keyState['D']){
        cameraPos += dr * MOVE_SPEED;
        frontPos += dr * MOVE_SPEED;
    }


    // Rotate
    if(keyState['I']){
        frontPos = moveCameraUD(cameraPos, frontPos, -ROTATE_SPEED);
    }
    if(keyState['K']){
        frontPos = moveCameraUD(cameraPos, frontPos, ROTATE_SPEED);
    }
    if(keyState['J']){
        frontPos = moveCameraLR(cameraPos, frontPos, -ROTATE_SPEED);
    }
    if(keyState['L']){
        frontPos = moveCameraLR(cameraPos, frontPos, ROTATE_SPEED);
    }

    if(directionKey[0]){
        cameraPos.y += MOVE_SPEED;
        frontPos.y += MOVE_SPEED;
    }
    if(directionKey[1]){
        cameraPos.y -= MOVE_SPEED;
        frontPos.y -= MOVE_SPEED;
    }
}

void drawCoordinateString(glm::vec3 cameraPos, glm::vec3 frontPos, int width, int height, float dt, int fps){
    std::string cameraPosSt = "Camera Position: (" + std::to_string(cameraPos.x) + ", " + std::to_string(cameraPos.y) + ", " + std::to_string(cameraPos.z) + ")";
    std::string frontPosSt = "Front Position: (" + std::to_string(frontPos.x) + ", " + std::to_string(frontPos.y) + ", " + std::to_string(frontPos.z) + ")";
    std::string dtFpsSt = "dt: " + std::to_string(dt) + " fps: " + std::to_string(fps);
    glMatrixMode(GL_PROJECTION);
    glDisable(GL_LIGHTING);
    glPushMatrix();
    gluOrtho2D(0, width, 0, height);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glRasterPos2f(10, height - 30);
    for(int i = 0; i < cameraPosSt.length(); i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, cameraPosSt[i]);
    }
    glRasterPos2f(10, height - 60);
    for(int i = 0; i < frontPosSt.length(); i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, frontPosSt[i]);
    }
    glRasterPos2f(10, height - 90);
    for(int i = 0; i < dtFpsSt.length(); i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, dtFpsSt[i]);
    }
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void display(GLFWwindow *window, int width, int height, float dt, int fps, glm::vec3 &cameraPos, glm::vec3 &frontPos, std::vector<object *> &objs){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    drawCoordinateString(cameraPos, frontPos, width, height, dt, fps);

    glViewport(0, 0, width, height);
    gluPerspective(60.0f, (float) width / (float) height, 0.1f, 10000.0f);
    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0);
    glEnable(GL_LIGHTING);
    gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, frontPos.x, frontPos.y, frontPos.z, 0.0f, 1.0f, 0.0f);

    for(auto &obj : objs){
        obj->draw(MATERIAL::OBJECT, 1.0f, 0.0f, 0.0f);
    }
    glfwSwapBuffers(window);
    GLenum error = glGetError();
    if(error != GL_NO_ERROR){
        // 在這裡處理錯誤，例如輸出錯誤信息
        std::cerr << "OpenGL error: " << gluErrorString(error) << std::endl;
    }
}


void keyboard(GLFWwindow *window, int key, int scancode, int action, int mods){
    if(action == GLFW_PRESS){
        keyState[key] = true;

        switch(key){
            case GLFW_KEY_UP:
            directionKey[0] = true;
            break;
            case GLFW_KEY_DOWN:
            directionKey[1] = true;
            break;
            case GLFW_KEY_LEFT:
            directionKey[2] = true;
            break;
            case GLFW_KEY_RIGHT:
            directionKey[3] = true;
            break;

        }
    }
    else if(action == GLFW_RELEASE){
        keyState[key] = false;
        switch(key){
            case GLFW_KEY_UP:
            directionKey[0] = false;
            break;
            case GLFW_KEY_DOWN:
            directionKey[1] = false;
            break;
            case GLFW_KEY_LEFT:
            directionKey[2] = false;
            break;
            case GLFW_KEY_RIGHT:
            directionKey[3] = false;
            break;
        }
    }
}

void updatePhysics(float dt, std::vector<object *> &objs){
    // objs[1] 對 objs[0] 圓周運動
    glm::vec3 loc0 = objs[0]->getLoc();
    float m0 = objs[0]->getM();  // 獲取objs[0]的質量
    for(int i = 1; i < objs.size(); i++){
        glm::vec3 v = objs[i]->getV();
        glm::vec3 a = objs[i]->getA();
        glm::vec3 loc = objs[i]->getLoc();
        glm::vec3 r = loc - loc0;
        float m = objs[i]->getM();  // 獲取objs[i]的質量
        glm::vec3 F = glm::vec3(5, 5, 5) * m0 * m / (glm::length(r) * glm::length(r));  // 使用牛頓的萬有引力定律計算力
        a = F / m;  // 使用牛頓第二運動定律計算加速度
        objs[i]->setA(a);
    }

    for(auto &obj : objs){
        obj->update(dt);
    }
}
