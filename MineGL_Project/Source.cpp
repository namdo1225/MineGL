#include "include.h"
#include "controller.h"

int main() {
    float vertices[] = {
        // Back face
        -0.3f, -0.3f, -0.3f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // 0
         0.3f,  0.3f, -0.3f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // 1
         0.3f, -0.3f, -0.3f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // 2
        -0.3f,  0.3f, -0.3f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, // 3

        // Front face
        -0.3f, -0.3f,  0.3f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // 4
         0.3f, -0.3f,  0.3f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // 5
         0.3f,  0.3f,  0.3f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // 6
        -0.3f,  0.3f,  0.3f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, // 7

        // Left face
        -0.3f,  0.3f,  0.3f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // 8
        -0.3f,  0.3f, -0.3f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // 9
        -0.3f, -0.3f, -0.3f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, // 10

         // Right face
         0.3f,  0.3f,  0.3f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // 11
         0.3f, -0.3f, -0.3f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, // 12
         0.3f, -0.3f,  0.3f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // 13     

        // Bottom face
         0.3f, -0.3f, -0.3f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // 14

        // Top face
        -0.3f,  0.3f,  0.3f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f  // 15  
    };

    std::vector<std::vector<unsigned int>> indices2 = {
    { 0,  1,  2,  1,  0,  3},
    {4,  5,  6,  6,  7,  4},
    { 8,  9, 10, 10,  4,  8},
    {11, 12,  1, 12, 11, 13},
    {10, 14,  5,  5,  4, 10},
    {3, 11,  1, 11, 3, 15}
    
    };

    Controller controller = Controller("MineGL", 800, 600);
    //controller.appendDrawable(new SimpleTri);
    //controller.constructDrawable(0, vertices);
    //controller.setTexture("container.jpg", 0);
    //controller.setPosition(0.0f, 0.0f, 0.0f, 0);

    controller.createCube();

    //controller.constructDrawable(0, vertices);
    //controller.setTexture("texture/container.jpg", 0);
    //controller.setRotation(0.0f, 0.0f, 1.0f, 90.f, 0);
    //controller.setPosition(0.0f, 0.0f, 0.0f, 0);
    //controller.setScale(2.0f, 2.0f, 2.0f, 0);
    //controller.setColor(1.0f, 1.0f, 1.0f, 0);

    //controller.appendText(new Text);

    //controller.constructText(0, "This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.3, 0.8f, 0.2f), 800.f, 600.f);

    controller.loop();
    controller.end();

}