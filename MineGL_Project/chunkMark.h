#ifndef CHUNK_MARK_H
#define CHUNK_MARK_H

#include "include.h"
#include "player.h"
#include "shader.h"
#include "resourceManager.h"
#include "camera.h"

class ChunkMark {
public:
	ChunkMark();

	static void draw(Player& player, Camera& camera);
	static void clean();

	static void construct();

	static void setProjection(glm::mat4 project);
	static void setView(glm::mat4 view);

private:
	static unsigned int lineVAO;
	static unsigned int lineVBO;
	static unsigned int lineInstVBO;
	static unsigned int lineEBO;

	static glm::vec2 translations[100];

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
	static float quadVertices[];
	static float cubeVertices[];

	static Shader drawShader;

	static unsigned int indices[];

	static int playerX;
	static int playerZ;

	static glm::mat4 projection;
	static glm::mat4 view;
};
#endif