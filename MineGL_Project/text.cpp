#include "text.h"

FT_Library Text::ft;
FT_Face Text::face;

std::map<GLchar, Character> Text::Characters;

unsigned int Text::VBO = 0;
unsigned int Text::VAO = 0;
Shader Text::drawShader;

Text::Text() {}

void Text::construct(std::string text, float x, float y, float scale, glm::vec4 color, float resX, float resY) {
    projection = glm::ortho(0.0f, resX, 0.0f, resY);
    drawShader.Use();

    textT = text;
    xT = x;
    yT = y;
    scaleT = scale;
    colorT = color;

    xB = x;

    glUniformMatrix4fv(glGetUniformLocation(drawShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

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

void Text::draw() {
    drawShader.Use();
    xT = xB;

    glUniform4f(glGetUniformLocation(drawShader.ID, "textColor"), colorT.x, colorT.y, colorT.z, colorT.a);
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

void Text::loadShader() {
    drawShader = ResourceManager::LoadShader("shader/1.2_text_vs.txt", "shader/1.2_text_fs.txt", nullptr, "TextShader");

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
}

void Text::setupOGL() {
    if (FT_Init_FreeType(&ft)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

    if (FT_New_Face(ft, "font/arial.ttf", 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }
    else {
        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 48);

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 128; c++)
        {
            // Load character glyph 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
            };
            Characters.insert(std::pair<char, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void Text::changeText(std::string text) {
    textT = text;
}