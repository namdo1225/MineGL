#ifndef CHUNK_MAP_H
#define CHUNK_MAP_H

#include "chunk.h"
#include "greedyChunk.h"
#include "player.h"
#include "include.h"

struct Coordinate {
	int x, z;

	Coordinate(int aX, int aZ);

	bool operator<(const Coordinate& coord) const noexcept;
};

class ChunkMap {
public:
	ChunkMap();

	void cleanChunks();

	void draw(Player &player, std::vector<Texture2D>& textures);

	void add(int x, int z);

	void setUp();

	void test_moveFace(float pixel);

private:
	std::map<Coordinate, GreedyChunk> map;

	void loadAdjacentChunks(int x, int z);

};

#endif