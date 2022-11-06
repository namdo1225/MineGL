#include "chunk.h"

FastNoiseLite Chunk::noise;

unsigned int Chunk::indices[][6] = {
    {0,  1,  2,  1,  0,  3 },
    {4,  5,  6,  6,  7,  4},
    {15, 3,  0, 0, 10, 15},
    {11, 12,  1, 12, 11, 13},
    {8, 14,  5,  5,  4, 8},
    {3, 9,  1, 9, 3, 15}
};

unsigned int Chunk::sizeX{ 16 };
unsigned int Chunk::sizeZ{ 16 };
unsigned int Chunk::sizeY{ 256 };

Chunk::Chunk() {}

Chunk::Chunk(int x, int z) {
    indexX = x, indexZ = z;

    worldCoord = glm::vec3(x * 9.6f, 0.0f, z * 9.6f);

    float noiseData[16 * 16];
    
    for (int i = 0; i < sizeZ; i++) { // z
        float nZ = i + (16 * z);
        for (int j = 0; j < sizeX; j++) { // x
            float nX = j + (16 * x);

            noiseData[index++] = noise.GetNoise(nZ, nX);
        }
    }

    for (unsigned int i{ 0 }; i < sizeZ; i++)
        faces.push_back(std::vector<glm::vec3>());

    for (int i = 0; i < sizeZ; i++) {
        block[i] = new Block * [sizeX];
        for (int j = 0; j < sizeX; j++) {
            block[i][j] = new Block[sizeY];
        }
    }

    for (unsigned int i{ 0 }; i < sizeZ; i++) { //z
        for (unsigned int j{ 0 }; j < sizeX; j++) { //x
            for (unsigned int k{ 0 }; k < sizeY; k++) { //y
                block[i][j][k] = AIR;
            }
        }
    }

    for (unsigned int i = 0; i < sizeZ * sizeZ; i++) { // x and y
        index = 0;
        float height{ noiseData[i] * 150 };

        for (float j = 0; j < height; j += 0.60) { // y
            if (j + 0.6 >= height)
                block[i / sizeZ][i % sizeZ][index++] = GRASS;
            else if (j <= noiseData[i] * 100)
                block[i / sizeZ][i % sizeZ][index++] = STONE;
            else
                block[i / sizeZ][i % sizeZ][index++] = DIRT;

            glm::vec3 translation = worldCoord;

            translation.x += (i % sizeZ) * 0.6;
            translation.y += j;
            translation.z += (i / sizeZ) * 0.6;

            translations.push_back(translation);
        }
    }
}

void Chunk::setNoise() {
    //noise.SetSeed(1250);
    noise.SetFrequency(0.001);
    noise.SetFractalType(FastNoiseLite::FractalType::FractalType_Ridged);
    //noise.SetFractalGain(3.0);
    noise.SetFractalLacunarity(5.0f);
    
    noise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_Perlin);
    //noise.SetFractalPingPongStrength(10.0f);
    //noise.SetFractalOctaves(100);
}

void Chunk::draw(std::vector<Texture2D>& textures) {
    for (unsigned int i{ 0 }; i < faces.size(); i++) {
        if (faces[i].size() == 0)
            continue;

        if (i == 11) {
            textures[1].Bind();
        }
        else if (i >= 6 and i <= 9) {
            textures[2].Bind();
        }
        else if (i >= 12) {
            textures[3].Bind();
        }
        else {
            textures[0].Bind();
        }

        glBufferData(GL_ARRAY_BUFFER, faces[i].size() * sizeof(glm::vec3), &faces[i][0], GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), &indices[i % 6][0], GL_STATIC_DRAW);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, faces[i].size());
    }
}

void Chunk::clean() {
    for (int i = 0; i < sizeZ; i++)
    {
        for (int j = 0; j < sizeX; j++) {
            delete[] block[i][j];
        }
        delete[] block[i];
    }

    delete[] block;
}

int Chunk::getChunkInx(char index) {
    if (index == 'z')
        return indexZ;
    return indexX;
}

bool Chunk::checkEdge(unsigned int blockID, unsigned int faceIndex, unsigned int i, unsigned int j, unsigned int k,
    Block*** block0, Block*** block1, Block*** block2, Block*** block3) {
    /*
        0 = north, z+
        1 = east,  x+
        2 = south, z-
        3 = west,  x-
    */

    if (block[i][j][k] == blockID) {
        // back, south
        if ((i == 0 and block2[15][j][k] == 0) or (i != 0 and block[i - 1][j][k] == 0))
            faces[faceIndex].push_back(translations[index]);

        // front, north
        if ((i == sizeZ - 1 and block0[0][j][k] == 0) or (i < sizeZ - 1 and block[i + 1][j][k] == 0))
            faces[faceIndex + 1].push_back(translations[index]);

        // left, west
        if ((j == 0 and block3[i][15][k] == 0) or (j != 0 and block[i][j - 1][k] == 0))
            faces[faceIndex + 2].push_back(translations[index]);

        // right, east
        if ((j == sizeZ - 1 and block1[i][0][k] == 0) or (j < sizeZ - 1 and block[i][j + 1][k] == 0))
            faces[faceIndex + 3].push_back(translations[index]);


        // bottom
        if (k != 0 and block[i][j][k - 1] == 0)
            faces[faceIndex + 4].push_back(translations[index]);

        // top
        if (k < 254 and block[i][j][k + 1] == 0)
            faces[faceIndex + 5].push_back(translations[index]);

        index++;
        return true;

    }

    return false;
}

void Chunk::prepareRegenerationEdge(Block*** block0, Block*** block1, Block*** block2, Block*** block3) {
    index = 0;

    for (unsigned int i{ 0 }; i < sizeZ; i++) { //z
        for (unsigned int j{ 0 }; j < sizeX; j++) { //x
            for (unsigned int k{ 0 }; k < sizeY; k++) { //y
                for (unsigned int m{ 1 }; m < 4; m++) {
                    if (checkEdge(m, 0 + (6 * (m - 1)), i, j, k, block0, block1, block2, block3))
                        break;
                }
            }
        }
    }

    edgeRegenerated = true;
}