#include "chunkMap.h"

Coordinate::Coordinate(int aX, int aZ) {
    x = aX;
    z = aZ;
}

bool Coordinate::operator<(const Coordinate& coord) const noexcept {
    if (x < coord.x) return true;
    if (x > coord.x) return false;

    if (z < coord.z) return true;
    if (z > coord.z) return false;

    return false;
}

ChunkMap::ChunkMap() {}

void ChunkMap::cleanChunks() {
    std::map<Coordinate, GreedyChunk>::iterator i;

    for (i = map.begin(); i != map.end(); i++) {
        i->second.clean();
    }
}

/*

beginning: load the chunk and the surrouding chunks. Regenerate all chunks maybe
with the exception of the edge chunk. Make sure to mark which chunks are regenerated for edges.

Then, whenever player moves maybe by a chunk, generate a chunk every direction? or just one direction?
Check the edge case again to regenerate for edges.

*/

void ChunkMap::loadAdjacentChunks(int x, int z) {
    if (!map.count(Coordinate(x + 1, z))) {
        add(x + 1, z);
    }

    if (!map.count(Coordinate(x, z + 1))) {
        add(x, z + 1);
    }

    if (!map.count(Coordinate(x - 1, z))) {
        add(x - 1, z);
    }

    if (!map.count(Coordinate(x, z - 1))) {
        add(x, z - 1);
    }
}

void ChunkMap::draw(Player& player, std::vector<Texture2D>& textures) {
    int playerChunkX = player.getPlayerChunk('x'), playerChunkZ = player.getPlayerChunk('z');

    int minChunkX{ playerChunkX - 10 }, minChunkZ{ playerChunkZ - 10 };
    int maxChunkX{ playerChunkX + 10 }, maxChunkZ{ playerChunkZ + 10 };

    for (int i{ minChunkZ }; i < maxChunkZ; i++) { // z
        for (int j{ minChunkX }; j < maxChunkX; j++) { // x
            add(j, i);

            loadAdjacentChunks(j, i);

            if (map[Coordinate(j, i)].edgeRegenerated == false) {
                map[Coordinate(j, i)].prepareRegenerationEdge(map[Coordinate(j, i + 1)].block,
                    map[Coordinate(j + 1, i)].block,
                    map[Coordinate(j, i - 1)].block,
                    map[Coordinate(j - 1, i)].block);
            }

        }
    }

    for (int i{ minChunkZ }; i < maxChunkZ; i++) { // z
        for (int j{ minChunkX }; j < maxChunkX; j++) { // x
            map[Coordinate(j, i)].draw(textures);
        }
    }
}

void ChunkMap::add(int x, int z) {
    if (!map.count(Coordinate(x, z)))
        map[Coordinate(x, z)] = GreedyChunk(x, z);
}

void ChunkMap::setUp() {
    GreedyChunk::setNoise();

    /// <summary>
    /// 
    /// FIX IT SO IT GENERATES 10 x 10 CHUNK
    /// </summary>
    for (int i{ -10 }; i < 10; i++) // x
        for (int j{ -10 }; j < 10; j++) { // z 
            add(i, j);
            loadAdjacentChunks(i, j);
            map[Coordinate(i, j)].prepareRegenerationEdge(map[Coordinate(i, j + 1)].block,
                                                          map[Coordinate(i + 1, j)].block,
                                                          map[Coordinate(i, j - 1)].block,
                                                          map[Coordinate(i - 1, j)].block);
        }

}

void ChunkMap::test_moveFace(float pixel) {
    map[Coordinate(0, 0)].test_moveChunkFace(pixel);
}