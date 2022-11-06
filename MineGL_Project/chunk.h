#ifndef CHUNK_H
#define CHUNK_H

#include "include.h"
#include "drawable.h"
#include "text.h"
#include "characters.h"
#include "camera.h"
#include "stb_image.h"
#include "characters.h"

enum Block {
	AIR, // 0
	DIRT,
	GRASS,
	STONE
};

class Chunk {
private:
	std::vector<std::vector<glm::vec3>> faces;

	glm::vec3 worldCoord;

	std::vector<glm::vec3> translations;
	static unsigned int sizeX;
	static unsigned int sizeZ;
	static unsigned int sizeY;
	
	static float vertices[];

	static unsigned int indices[][6];
	unsigned int index{ 0 };

	static FastNoiseLite noise;

	int indexX, indexZ;

	bool checkEdge(unsigned int blockID, unsigned int faceIndex, unsigned int i, unsigned int j, unsigned int k,
					Block*** block0, Block*** block1, Block*** block2, Block*** block3);

public:
	bool edgeRegenerated{ false };
	Block*** block = new Block** [16];

	/*
		0 = north, z+
		1 = east,  x+
		2 = south, z-
		3 = west,  x-
	*/

	Chunk();

	Chunk(int x, int z);

	void clean();

	static void setNoise();

	void draw(std::vector<Texture2D> &textures);

	int getChunkInx(char index);

	void prepareRegenerationEdge(Block*** block0, Block*** block1, Block*** block2, Block*** block3);

};

#endif