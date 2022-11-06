#ifndef PLAYER_H
#define PLAYER_H

#include "include.h"

class Player {
private:
	glm::vec3 playerPos = glm::vec3(0.f, 0.f, 0.f);
	int playerChunkX{ 0 }, playerChunkZ{ 0 };
	unsigned int viewDistance{ 10 };

public:
	int getPlayerChunk(char c);

	unsigned int getViewDistance();

	glm::vec3 getPlayerPos();

	void updatePlayerPos(glm::vec3 pos);

	Player();
};

#endif