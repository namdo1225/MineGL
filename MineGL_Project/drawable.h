#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "include.h"
#include "shader.h"
#include "stb_image.h"

class Drawable {
public:
	Shader drawShader;

	virtual void construct(float* vertices) = 0;
	virtual void construct(float* vertices, int* indices) = 0;
	virtual void deconstruct() = 0;

	virtual void draw() = 0;

	void changeProjection(const glm::mat4 mat);
	void changeView(const glm::mat4 mat);

	void setPosition(float x, float y, float z);
	void setScale(float x, float y, float z);
	void setRotation(float x, float y, float z, float r);

	void setTexture(const char* texturePath);

	void setColor(float r, float g, float b);

protected:
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);
	glm::vec3 color = glm::vec3(1.0f);

	unsigned int textureUnInt{ 0 };

	Drawable();

private:
};

#endif