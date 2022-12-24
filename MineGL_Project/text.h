#ifndef TEXT_H
#define TEXT_H

#include "include.h"
#include "shader.h"
#include "characters.h"
#include "resourceManager.h"

class Text {
public:
    static Shader drawShader;

    Text();

    void construct(std::string text, float x, float y, float scale, glm::vec4 color, float resX, float resY);
    static void deconstruct();
    void draw();

    void changeView(const glm::mat4 mat);

    static void loadShader();

    static void setupOGL();

    void changeText(std::string text);

private:
    static unsigned int VBO, VAO;
    static std::map<GLchar, Character> Characters;
    static FT_Library ft;
    static FT_Face face;

    std::string textT;
    float xT, yT, scaleT;
    float xB;

    glm::vec4 colorT;
    glm::mat4 projection;

    glm::mat4 view = glm::mat4(1.0f);
};

#endif