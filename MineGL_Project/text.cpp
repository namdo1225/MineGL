#include "text.h"

Text::Text() {}

void Text::construct(std::string text, float x, float y, float scale, glm::vec3 color, float resX, float resY) {
    drawShader = ResourceManager::LoadShader("shader/1.2_text_vs.txt", "shader/1.2_text_fs.txt", nullptr, "TextShader");
    projection = glm::ortho(0.0f, resX, 0.0f, resY);
    drawShader.Use();

    textT = text;
    xT = x;
    yT = y;
    scaleT = scale;
    colorT = color;

    textB = text;
    xB = x;
    yB = y;
    scaleB = scale;
    colorB = color;

    glUniformMatrix4fv(glGetUniformLocation(drawShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void Text::deconstruct() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Text::draw(std::map<GLchar, Character> &Characters) {
    drawShader.Use();

    textT = textB;
    xT = xB;
    yT = yB;
    scaleT = scaleB;
    colorT = colorB;

    glUniform3f(glGetUniformLocation(drawShader.ID, "textColor"), colorT.x, colorT.y, colorT.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
    
    // iterate through all characters
    std::string::const_iterator c;
    for (c = textT.begin(); c != textT.end(); c++)
    {
        Character ch = Characters[*c];

        float xpos = xT + ch.Bearing.x * scaleT;
        float ypos = yT - (ch.Size.y - ch.Bearing.y) * scaleT;

        float w = ch.Size.x * scaleT;
        float h = ch.Size.y * scaleT;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        xT += (ch.Advance >> 6) * scaleT; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Text::changeView(const glm::mat4 mat) {
    view = mat;
    drawShader.SetMatrix4("view", view, true);
}