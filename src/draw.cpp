#include "draw.h"
#include <algorithm>
#define BOX_SIZE 21


int keyState[GLFW_KEY_LAST] = { 0 };
int directionKey[4] = { 0 };
bool phyActive = false;
unsigned int textName[10];
unsigned char floorText[TEXTURE_SIZE][TEXTURE_SIZE][4];
unsigned char emptyText[TEXTURE_SIZE][TEXTURE_SIZE][4];
int viewMode = 0;

void initTexture(){
    for(int i = 0; i < TEXTURE_SIZE; i++)
        for(int j = 0; j < TEXTURE_SIZE; j++)
            emptyText[i][j][0] = emptyText[i][j][1] =
            emptyText[i][j][2] = emptyText[i][j][3] = 255;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 10);
    glGenTextures(10, textName);
    GenerateTexture(floorText);
    TextureInit(TEXTURE::T_FLOOR, textName, floorText, TEXTURE_SIZE, TEXTURE_SIZE);
    // TextureInit(TEXTURE::T_WALL, textName, emptyText, TEXTURE_SIZE, TEXTURE_SIZE);
    setGLSLshaders("shader/Phong.vert", "shader/Phong.frag");
    int texLoc = glGetUniformLocation(ReturnProgramID(), "myTex");
    // std::cout << texLoc << std::endl;
    glUniform1i(texLoc, 0);
}
void glInit(){
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    //smooth shading
    glShadeModel(GL_SMOOTH);
}

void setLight(){
    glEnable(SUN_LIGHT);
    // GLfloat light_position[] = { 1.0, 0.0, 0.0, 0.0 };
    GLfloat light_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat light_diffuse[] = { 0.5, 0.5, 0.5, 1.0 };     // Diffuse light color (RGBA)
    GLfloat light_specular[] = { 0.5, 0.5, 0.5, 1.0 };    // Specular light color (RGBA)
    // glLightfv(SUN_LIGHT, GL_POSITION, light_position);
    glLightfv(SUN_LIGHT, GL_AMBIENT, light_ambient);
    glLightfv(SUN_LIGHT, GL_DIFFUSE, light_diffuse);
    glLightfv(SUN_LIGHT, GL_SPECULAR, light_specular);
}

void initObjects(std::vector<object *> &objs, int num){
    objs.push_back(new cube(glm::vec3(0, 0, 0), BOX_SIZE, 0.5, BOX_SIZE));
    objs.back()->setColor(glm::vec3(0.0f, 0.5f, 0.0f));
    objs.back()->setM(1e10);
    objs.back()->setName("Ground");
    objs.back()->setMaterial(MATERIAL::M_FLOOR);
    objs.back()->setTexture(TEXTURE::T_FLOOR, textName);
    // /*
    objs.push_back(new cube(glm::vec3(0, BOX_SIZE / 2, BOX_SIZE / 2 + 0.5), BOX_SIZE, BOX_SIZE * 1.5, 0.5));
    objs.back()->setColor(glm::vec3(0.5f, 0.0f, 0.5f));
    objs.back()->setM(1e10);
    objs.back()->setName("RWall");
    objs.back()->setMaterial(MATERIAL::M_FLOOR);
    // objs.back()->setTexture(TEXTURE::T_WALL, textName);
    objs.push_back(new cube(glm::vec3(0, BOX_SIZE / 2, -BOX_SIZE / 2 - 0.5), BOX_SIZE, BOX_SIZE * 1.5, 0.5));
    objs.back()->setColor(glm::vec3(0.5f, 0.0f, 0.5f));
    objs.back()->setM(1e10);
    objs.back()->setName("LWall");
    objs.back()->setMaterial(MATERIAL::M_FLOOR);
    // objs.back()->setTexture(TEXTURE::T_WALL, textName);
    objs.push_back(new cube(glm::vec3(BOX_SIZE / 2 + 0.5, BOX_SIZE / 2, 0), 0.5, BOX_SIZE * 1.5, BOX_SIZE));
    objs.back()->setColor(glm::vec3(0.5f, 0.5f, 0.0f));
    objs.back()->setM(1e10);
    objs.back()->setName("FWall");
    objs.back()->setMaterial(MATERIAL::M_FLOOR);
    // objs.back()->setTexture(TEXTURE::T_WALL, textName);
    objs.push_back(new cube(glm::vec3(-BOX_SIZE / 2 - 0.5, BOX_SIZE / 2, 0), 0.5, BOX_SIZE * 1.5, BOX_SIZE));
    objs.back()->setColor(glm::vec3(0.5f, 0.5f, 0.0f));
    objs.back()->setM(1e10);
    objs.back()->setName("BWall");
    objs.back()->setMaterial(MATERIAL::M_FLOOR);
    // objs.back()->setTexture(TEXTURE::T_WALL, textName);
    objs.push_back(new cube(glm::vec3(0, 3, 0), 6, 6, 6));
    objs.back()->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
    objs.back()->setM(1e10);
    objs.back()->setName("Cube");
    objs.back()->setMaterial(MATERIAL::M_FLOOR);
    // objs.back()->setTexture(TEXTURE::T_FLOOR, textName);

    std::random_device rd;
    std::mt19937 gen(1234);
    // int K; std::cin >> K;
    int K = num;
    int count = 0;

    // /*
    for(int j = 0; count < K; j++){
        for(int i = 1; i <= 20 && count < K; i++){
            for(int k = 0; k < 4 && count < K; k++){
                objs.push_back(new ball(glm::vec3(8 - k * 5, 10 + j, -BOX_SIZE / 2 - 0.5 + i), 0.3));
                objs.back()->setColor(glm::vec3(1.0f, 0, 0));
                objs.back()->setM(1);
                // objs.back()->setM(float(gen() % 5) + 1);
                objs.back()->setMaterial(MATERIAL::M_OBJECT);
                int rx = (gen() % 2) ? 1 : -1;
                int ry = (gen() % 2) ? 1 : -1;
                int rz = (gen() % 2) ? 1 : -1;
                objs.back()->setV(glm::vec3(float(gen() % 10) * rx, float(gen() % 10) * ry, float(gen() % 10) * rz));
                count++;

                if(count >= K){
                    break;
                }
            }
        }
    }
    // */

    // for(int i = 0; i < num - K; i++){
    //     int x = gen() % 4;
    //     int y = gen() % 5;
    //     int z = gen() % 20 + 1;
    //     int rx = (gen() % 2) ? 1 : -1;
    //     int ry = (gen() % 2) ? 1 : -1;
    //     int rz = (gen() % 2) ? 1 : -1;
    //     objs.push_back(new ball(glm::vec3(8 - x * 5, 16 + y, -BOX_SIZE / 2 - 0.5 + z), 0.3));
    //     objs.back()->setColor(glm::vec3(1.0f, 0, 0));
    //     objs.back()->setV(glm::vec3(float(gen() % 10) * rx, float(gen() % 10) * ry, float(gen() % 10) * rz));
    //     objs.back()->setM(float(gen() % 5) + 1);
    //     objs.back()->setMaterial(MATERIAL::M_OBJECT);
    // }
// */
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
    glUseProgram(0);
    std::string cameraPosSt = "Camera Position: (" + std::to_string(cameraPos.x) + ", " + std::to_string(cameraPos.y) + ", " + std::to_string(cameraPos.z) + ")";
    std::string frontPosSt = "Front Position: (" + std::to_string(frontPos.x) + ", " + std::to_string(frontPos.y) + ", " + std::to_string(frontPos.z) + ")";
    std::string dtFpsSt = "dt: " + std::to_string(dt) + " fps: " + std::to_string(fps);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glDisable(GL_LIGHTING);
    glPushMatrix();
    glViewport(0, 0, width, height);
    gluOrtho2D(0, width, 0, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glPushAttrib(GL_CURRENT_BIT);
    // Set text color
    glColor3f(1.0f, 1.0f, 1.0f); // Red color
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
    glPopAttrib();
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void drawSingleView(std::vector<object *> &objs, int width, int height, glm::vec3 &cameraPos, glm::vec3 &frontPos){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0);
    glEnable(GL_LIGHTING);
    gluPerspective(60.0f, (float) width / (float) height, 0.1f, 10000.0f);
    gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, frontPos.x, frontPos.y, frontPos.z, 0.0f, 1.0f, 0.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    setLight();
    for(auto &obj : objs){
        obj->draw();
    }
}

void drawMultiView(std::vector<object *> &objs, int width, int height, glm::vec3 &cameraPos, glm::vec3 &frontPos){
    int viewportWidth = width / 2;
    int viewportHeight = height / 2;

    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0);
    glEnable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);
    // Top Left View 
    glLoadIdentity();
    glViewport(0, height / 2, viewportWidth, viewportHeight);
    gluPerspective(60.0f, (float) width / (float) height, 0.1f, 10000.0f);
    gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, frontPos.x, frontPos.y, frontPos.z, 0.0f, 1.0f, 0.0f);
    setLight();
    for(auto &obj : objs){
        obj->draw();
    }

    // Top Right View
    glLoadIdentity();
    glViewport(width / 2, height / 2, viewportWidth, viewportHeight);
    glOrtho(-width / 50, width / 50, -height / 50, height / 50, -1000, 1000);
    gluLookAt(0 - 1, 0, 0, 0, 0, 0, 0.0f, 1.0f, 0.0f);
    setLight();
    for(auto &obj : objs){
        if(obj->getName() == "BWall") continue;
        obj->draw();
    }

    // Bottom Left View
    glLoadIdentity();
    glViewport(0, 0, viewportWidth, viewportHeight);
    glOrtho(-width / 50, width / 50, -height / 50, height / 50, -1000, 1000);
    gluLookAt(0, 0, 0 - 1, 0, 0, 0, 0.0f, 1.0f, 0.0f);
    setLight();
    for(auto &obj : objs){
        if(obj->getName() == "LWall") continue;
        obj->draw();
    }

    // Bottom Right View
    glLoadIdentity();
    glViewport(width / 2, 0, viewportWidth, viewportHeight);
    glOrtho(-width / 50, width / 50, -height / 50, height / 50, -1000, 1000);
    gluLookAt(0 - 0.001f, 0 + 1, 0, 0, 0, 0, 0.0f, 1.0f, 0.0f);
    setLight();
    for(auto &obj : objs){
        obj->draw();
    }
}



void display(GLFWwindow *window, int width, int height, float dt, int fps, glm::vec3 &cameraPos, glm::vec3 &frontPos, std::vector<object *> &objs){
    // glUseProgram(0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    drawCoordinateString(cameraPos, frontPos, width, height, dt, fps);

    glUseProgram(ReturnProgramID());
    if(viewMode == 0)
        drawSingleView(objs, width, height, cameraPos, frontPos);
    else if(viewMode == 1)
        drawMultiView(objs, width, height, cameraPos, frontPos);
    glfwSwapBuffers(window);
    GLenum error = glGetError();
    if(error != GL_NO_ERROR){
        std::cerr << std::hex << "OpenGL error: " << error << std::endl;
    }
}


void keyboard(GLFWwindow *window, int key, int scancode, int action, int mods){
    if(action == GLFW_PRESS){
        keyState[key] = true;
        if(key == GLFW_KEY_P){
            phyActive = !phyActive;
        }
        if(key == GLFW_KEY_V){
            viewMode = (viewMode + 1) % 2;
        }
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
    if(!phyActive) return;
    for(int i = 1; i < 6; i++){
        for(int j = 6; j < objs.size(); j++){
            if(objs[i]->isCollide(*(objs[j]))){
                cube *c = dynamic_cast<cube *>(objs[i]);
                if(c != nullptr && (c->getName() == "BWall" ||
                    c->getName() == "RWall" ||
                    c->getName() == "LWall" ||
                    c->getName() == "FWall")){
                    ball *b = dynamic_cast<ball *>(objs[j]);
                    if(b != nullptr){
                        float cl = c->getL() / 2;
                        float ch = c->getH() / 2;
                        if(ch > cl){
                            if(b->getLoc().x < c->getLoc().x){
                                b->setLoc(glm::vec3(c->getLoc().x - b->getR() - cl, b->getLoc().y, b->getLoc().z));
                            }
                            else{
                                b->setLoc(glm::vec3(c->getLoc().x + b->getR() + cl, b->getLoc().y, b->getLoc().z));
                            }
                            b->setV(glm::vec3(-b->getV().x, b->getV().y, b->getV().z));
                        }
                        else{
                            if(b->getLoc().z < c->getLoc().z){
                                b->setLoc(glm::vec3(b->getLoc().x, b->getLoc().y, c->getLoc().z - b->getR() - ch));
                            }
                            else{
                                b->setLoc(glm::vec3(b->getLoc().x, b->getLoc().y, c->getLoc().z + b->getR() + ch));
                            }
                            b->setV(glm::vec3(b->getV().x, b->getV().y, -b->getV().z));
                        }
                        b->setColor(glm::vec3(0, 0, 1));
                    }
                    continue;
                }
                if(c != nullptr && c->getName() == "Cube"){
                    ball *b = dynamic_cast<ball *>(objs[j]);
                    if(b != nullptr){
                        float cl = c->getL();
                        float ch = c->getH();
                        float cw = c->getW();
                        //撞到上面
                        if(b->getLoc().y > c->getLoc().y + ch / 2){
                            b->setLoc(glm::vec3(b->getLoc().x, c->getLoc().y + ch / 2 + b->getR(), b->getLoc().z));
                            b->setV(glm::vec3(b->getV().x, 0.8 * -b->getV().y, b->getV().z));
                        }
                        //撞到左右
                        else if(b->getLoc().x < c->getLoc().x - cl / 2){
                            b->setLoc(glm::vec3(c->getLoc().x - cl / 2 - b->getR(), b->getLoc().y, b->getLoc().z));
                            b->setV(glm::vec3(-b->getV().x, b->getV().y, b->getV().z));
                        }
                        else if(b->getLoc().x > c->getLoc().x + cl / 2){
                            b->setLoc(glm::vec3(c->getLoc().x + cl / 2 + b->getR(), b->getLoc().y, b->getLoc().z));
                            b->setV(glm::vec3(-b->getV().x, b->getV().y, b->getV().z));
                        }
                        //撞到前後
                        else if(b->getLoc().z < c->getLoc().z - cw / 2){
                            b->setLoc(glm::vec3(b->getLoc().x, b->getLoc().y, c->getLoc().z - cw / 2 - b->getR()));
                            b->setV(glm::vec3(b->getV().x, b->getV().y, -b->getV().z));
                        }
                        else if(b->getLoc().z > c->getLoc().z + cw / 2){
                            b->setLoc(glm::vec3(b->getLoc().x, b->getLoc().y, c->getLoc().z + cw / 2 + b->getR()));
                            b->setV(glm::vec3(b->getV().x, b->getV().y, -b->getV().z));
                        }

                    }
                    continue;
                }
                /*
                ball *b1 = (dynamic_cast<ball *>(objs[i]));
                ball *b2 = (dynamic_cast<ball *>(objs[j]));
                if(b1 == nullptr || b2 == nullptr) continue;
                glm::vec3 v1 = b1->getV();
                glm::vec3 v2 = b2->getV();
                glm::vec3 loc1 = b1->getLoc();
                glm::vec3 loc2 = b2->getLoc();
                float m1 = b1->getM();
                float m2 = b2->getM();
                glm::vec3 diff_loc = loc1 - loc2;
                //v1' = v1 - ((2 * m2 / (m1 + m2)) * (v1 - v2) * (loc1 - loc2) / |loc1 - loc2|^2)*(loc1 - loc2)
                glm::vec3 v1_after = v1 - ((2 * m2 / (m1 + m2)) * glm::dot(v1 - v2, loc1 - loc2) / glm::dot(loc1 - loc2, loc1 - loc2)) * (loc1 - loc2);
                glm::vec3 v2_after = v2 - ((2 * m1 / (m1 + m2)) * glm::dot(v2 - v1, loc2 - loc1) / glm::dot(loc2 - loc1, loc2 - loc1)) * (loc2 - loc1);
                b1->setV(v1_after);
                b2->setV(v2_after);

                glm::vec3 oloc1 = b1->getLoc();
                glm::vec3 oloc2 = b2->getLoc();
                glm::vec3 dirVec = oloc1 - oloc2;
                if(glm::length(dirVec) < b1->getR() + b2->getR()){
                    b1->setLoc(oloc1 + glm::normalize(dirVec) * (2 * b1->getR() - glm::length(dirVec)));
                    b2->setLoc(oloc2 - glm::normalize(dirVec) * (2 * b2->getR() - glm::length(dirVec)));
                }
                b1->setColor(glm::vec3(0, 1, 0));
                b2->setColor(glm::vec3(0, 1, 0));
                // */
            }
        }
    }
    // /*
    Kdtree::KdNodeVector nodes;
    for(int i = 6; i < objs.size(); i++){
        std::pair<int, std::vector<double>> p;
        p.second.resize(3);
        p.second[0] = objs[i]->getLoc().x;
        p.second[1] = objs[i]->getLoc().y;
        p.second[2] = objs[i]->getLoc().z;
        p.first = i;
        nodes.push_back(Kdtree::KdNode(p));
    }
    Kdtree::KdTree tree(&nodes);

    for(int i = 6; i < objs.size(); i++){
        std::pair<int, std::vector<double>> tp;
        tp.second.resize(3);
        tp.second[0] = objs[i]->getLoc().x;
        tp.second[1] = objs[i]->getLoc().y;
        tp.second[2] = objs[i]->getLoc().z;
        Kdtree::KdNodeVector result;
        tree.range_nearest_neighbors(tp, 0.6f, &result);
        ball *b1 = (dynamic_cast<ball *>(objs[i]));
        std::sort(result.begin(), result.end(), [](Kdtree::KdNode a, Kdtree::KdNode b){
            return a.point.first < b.point.first;
        });
        for(auto j : result){
            if(i <= j.point.first) continue;
            // std::cout << i << " " << j.point.first << std::endl;
            ball *b2 = (dynamic_cast<ball *>(objs[j.point.first]));
            if(b1 == nullptr || b2 == nullptr) continue;
            glm::vec3 v1 = b1->getV();
            glm::vec3 v2 = b2->getV();
            glm::vec3 loc1 = b1->getLoc();
            glm::vec3 loc2 = b2->getLoc();
            float m1 = b1->getM();
            float m2 = b2->getM();
            glm::vec3 diff_loc = loc1 - loc2;
            //v1' = v1 - ((2 * m2 / (m1 + m2)) * (v1 - v2) * (loc1 - loc2) / |loc1 - loc2|^2)*(loc1 - loc2)
            glm::vec3 v1_after = v1 - ((2 * m2 / (m1 + m2)) * glm::dot(v1 - v2, loc1 - loc2) / glm::dot(loc1 - loc2, loc1 - loc2)) * (loc1 - loc2);
            glm::vec3 v2_after = v2 - ((2 * m1 / (m1 + m2)) * glm::dot(v2 - v1, loc2 - loc1) / glm::dot(loc2 - loc1, loc2 - loc1)) * (loc2 - loc1);
            b1->setV(v1_after);
            b2->setV(v2_after);

            glm::vec3 oloc1 = b1->getLoc();
            glm::vec3 oloc2 = b2->getLoc();
            glm::vec3 dirVec = oloc1 - oloc2;
            if(glm::length(dirVec) < b1->getR() + b2->getR()){
                b1->setLoc(oloc1 + glm::normalize(dirVec) * (2 * b1->getR() - glm::length(dirVec)));
                b2->setLoc(oloc2 - glm::normalize(dirVec) * (2 * b2->getR() - glm::length(dirVec)));
            }
            b1->setColor(glm::vec3(0, 1, 0));
            b2->setColor(glm::vec3(0, 1, 0));
        }

    }
    // */
    for(auto &obj : objs){
        obj->update(dt / 2);
    }
    for(auto &obj : objs){
        ball *b = dynamic_cast<ball *>(obj);
        cube *floorObj = (dynamic_cast<cube *>(objs[0]));
        //彈性係數 0.8
        if(b != nullptr){
            b->setA(glm::vec3(0, -9.8, 0));
            // /*
            if(b->getLoc().y - b->getR() - floorObj->getW() / 2 < ESP){
                // F = -u * m * g
                glm::vec3 v = b->getV();
                glm::vec3 f = glm::vec3(-0.1 * 9.8 * b->getM() * glm::normalize(v).x, 0, -0.1 * 9.8 * b->getM() * glm::normalize(v).z);
                b->addA(f / b->getM());
                // continue;
            }
            // */
            if(b->getLoc().y - b->getR() - floorObj->getW() / 2 < ESP){
                b->setLoc(glm::vec3(b->getLoc().x, b->getR() + floorObj->getW() / 2, b->getLoc().z));
                b->setV(glm::vec3(b->getV().x, -0.8 * b->getV().y, b->getV().z));
                b->setColor(glm::vec3(0, 0, 1));
            }
        }
    }
    for(auto &obj : objs){
        obj->update(dt / 2);
    }
}
