#ifndef TEXT_H
#define TEXT_H

#include "include.h"
#include "shader.h"
#include "characters.h"
#include "resourceManager.h"

class Text {
public:
    Shader drawShader;

    Text();

    void construct(std::string text, float x, float y, float scale, glm::vec3 color, float resX, float resY);
    void deconstruct();
    void draw(std::map<GLchar, Character> &Characters);

    void changeView(const glm::mat4 mat);

private:
    unsigned int VBO, VAO;

    std::string textT;
    float xT, yT, scaleT; 

    glm::vec3 colorT;
    glm::mat4 projection;

    glm::mat4 view = glm::mat4(1.0f);

    std::string textB;
    float xB, yB, scaleB;

    glm::vec3 colorB;

};

#endif