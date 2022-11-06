#include "drawable.h"

Drawable::Drawable() {}

void Drawable::changeProjection(const glm::mat4 mat) {
	projection = mat;
	drawShader.SetMatrix4("projection", projection, true);
}

void Drawable::changeView(const glm::mat4 mat) {
	view = mat;
	drawShader.SetMatrix4("view", view, true);
}

void Drawable::setPosition(float x, float y, float z) {
	model = glm::translate(model, glm::vec3(x, y, z));
	drawShader.SetMatrix4("model", model, true);
}

void Drawable::setScale(float x, float y, float z) {
	model = glm::scale(model, glm::vec3(x, y, z));
	drawShader.SetMatrix4("model", model, true);
}

void Drawable::setRotation(float x, float y, float z, float r) {
	model = glm::rotate(model, glm::radians(r), glm::vec3(x, y, z));
	drawShader.SetMatrix4("model", model, true);
}

void Drawable::setTexture(const char* texturePath) {
    glActiveTexture(GL_TEXTURE1);
    glGenTextures(1, &textureUnInt);
    glBindTexture(GL_TEXTURE_2D, textureUnInt); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);
}

void Drawable::setColor(float r, float g, float b) {
    color = glm::vec3(r, g, b);
    drawShader.SetVector3f("extraColor", color, true);
}