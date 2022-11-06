#ifndef GREEDY_CHUNK_H
#define GREEDY_CHUNK_H

#include "include.h"
#include "drawable.h"
#include "text.h"
#include "characters.h"
#include "camera.h"
#include "stb_image.h"
#include "characters.h"
#include "chunk.h"

class GreedyChunk {
private:
	std::vector<std::vector<glm::mat4>> faces;

	glm::vec3 worldCoord;

	std::vector<glm::vec3> translations;
	static unsigned int sizeX;
	static unsigned int sizeZ;
	static unsigned int sizeY;

	static float vertices[];

	static unsigned int indices[][6];
	unsigned int index{ 0 };
	float translateFactor[17] = { 0.0f, 0.0f, .15f, .20f, .225f, .24005, 0.249875, 0.256875, 0.2621575, 0.26661875, 0.27061875, 0.27261875,
	0.27468875, 0.27641875, 0.27841875, 0.27941875, 0.28041875 };

	static FastNoiseLite noise;

	int indexX, indexZ;

	bool checkEdge(unsigned int blockID, unsigned int faceIndex, unsigned int i, unsigned int j, unsigned int k,
		Block*** block0, Block*** block1, Block*** block2, Block*** block3);

public:
	bool edgeRegenerated{ false };
	Block*** block = new Block * *[16];

	/*
		0 = north, z+
		1 = east,  x+
		2 = south, z-
		3 = west,  x-
	*/

	GreedyChunk();

	GreedyChunk(int x, int z);

	void clean();

	static void setNoise();

	void draw(std::vector<Texture2D>& textures);

	int getChunkInx(char index);

	void prepareRegenerationEdge(Block*** block0, Block*** block1, Block*** block2, Block*** block3);


	// test
	void greedyAlgorithm();

	// test
	void test_moveChunkFace(float pixel);
};

#endif