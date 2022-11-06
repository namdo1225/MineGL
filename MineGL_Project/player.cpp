#include "player.h"

Player::Player() {}

int Player::getPlayerChunk(char c) {
	if (c == 'x')
		return playerChunkX;
	return playerChunkZ;
}


unsigned int Player::getViewDistance() {
	return viewDistance;
}

glm::vec3 Player::getPlayerPos() {
	return playerPos;
}

void Player::updatePlayerPos(glm::vec3 pos) {
	playerPos = pos;

	playerChunkX = static_cast<int>(pos.x / 9.6);
	playerChunkZ = static_cast<int>(pos.z / 9.6);
}