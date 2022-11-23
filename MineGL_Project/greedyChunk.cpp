#include "greedyChunk.h"

FastNoiseLite GreedyChunk::noise;

unsigned int GreedyChunk::indices[][6] = {
    {0,  1,  2,  1,  0,  3 },
    {4,  5,  6,  6,  7,  4 },
    {8,  9, 10, 10, 11,  8 },
    {12, 13, 15, 13, 12, 14},
    {17, 19, 16, 16, 18, 17},
    {21, 20, 23, 20, 21, 22}
};

unsigned int GreedyChunk::sizeX{ 16 };
unsigned int GreedyChunk::sizeZ{ 16 };
unsigned int GreedyChunk::sizeY{ 256 };

GreedyChunk::GreedyChunk() {}

GreedyChunk::GreedyChunk(int x, int z) {
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
        faces.push_back(std::vector<glm::mat4>());

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

void GreedyChunk::setNoise() {
    std::mt19937 mt{ static_cast<unsigned int>(
        std::chrono::steady_clock::now().time_since_epoch().count()
        ) };

    noise.SetSeed(mt());
    noise.SetFrequency(0.001);
    noise.SetFractalType(FastNoiseLite::FractalType::FractalType_Ridged);
    //noise.SetFractalGain(3.0);
    noise.SetFractalLacunarity(4.25f);

    noise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_Perlin);
    //noise.SetFractalPingPongStrength(10.0f);
    //noise.SetFractalOctaves(100);
}

void GreedyChunk::draw(std::vector<Texture2D>& textures) {
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

        glBufferData(GL_ARRAY_BUFFER, faces[i].size() * sizeof(glm::mat4), &faces[i][0], GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), &indices[i % 6][0], GL_STATIC_DRAW);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, faces[i].size());
    }
}

void GreedyChunk::clean() {
    for (int i = 0; i < sizeZ; i++)
    {
        for (int j = 0; j < sizeX; j++) {
            delete[] block[i][j];
        }
        delete[] block[i];
    }

    delete[] block;
}

int GreedyChunk::getChunkInx(char index) {
    if (index == 'z')
        return indexZ;
    return indexX;
}

bool GreedyChunk::checkEdge(unsigned int blockID, unsigned int faceIndex, unsigned int i, unsigned int j, unsigned int k,
    Block*** block0, Block*** block1, Block*** block2, Block*** block3) {
    /*
        0 = north, z+
        1 = east,  x+
        2 = south, z-
        3 = west,  x-
    */

    if (block[i][j][k] == blockID) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, translations[index]);

        // back, south
        if ((i == 0 and block2[15][j][k] == 0) or (i != 0 and block[i - 1][j][k] == 0))
            faces[faceIndex].push_back(model);

        // front, north
        if ((i == sizeZ - 1 and block0[0][j][k] == 0) or (i < sizeZ - 1 and block[i + 1][j][k] == 0))
            faces[faceIndex + 1].push_back(model);

        // left, west
        if ((j == 0 and block3[i][15][k] == 0) or (j != 0 and block[i][j - 1][k] == 0))
            faces[faceIndex + 2].push_back(model);

        // right, east
        if ((j == sizeZ - 1 and block1[i][0][k] == 0) or (j < sizeZ - 1 and block[i][j + 1][k] == 0))
            faces[faceIndex + 3].push_back(model);


        // bottom
        if (k != 0 and block[i][j][k - 1] == 0)
            faces[faceIndex + 4].push_back(model);

        // top
        if (k < 254 and block[i][j][k + 1] == 0)
            faces[faceIndex + 5].push_back(model);

        index++;
        return true;

    }

    return false;
}

void GreedyChunk::prepareRegenerationEdge(Block*** block0, Block*** block1, Block*** block2, Block*** block3) {
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

    greedyAlgorithm();
    edgeRegenerated = true;
}

void GreedyChunk::greedyAlgorithm() {
    faces[11].clear();
    std::vector<CoorPack> coords;
    for (unsigned int k{ 0 }; k < sizeY; k++) { //y
        glm::mat4 model = glm::mat4(1.0f);
        glm::vec3 translate = glm::vec3(1.0f);
        bool first = true;
        int scaleZ = 1;
        int scaleY = 1;

        for (unsigned int j{ 0 }; j < sizeX; j++) { //x
 

            for (unsigned int i{ 0 }; i < sizeZ; i++) { //z
                if (block[i][j][k] == GRASS) {

                    // if this is the first z or only z checked
                    if (first) {
                        translate = glm::vec3((j * 0.6) + (indexX * 9.6f), k * 0.6, (i * 0.6) + (indexZ * 9.6f));
                        first = false;

                        if (i + 1 == sizeZ) {
                            coords.push_back(CoorPack(j, k, i, 0, 0, scaleZ));

                            model = glm::translate(model, translate);
                            faces[11].push_back(model);

                            model = glm::mat4(1.0f);
                            translate = glm::vec3(1.0f);
                            first = true;
                            scaleZ = 1;
                        }
                    }
                    else if (i + 1 == sizeZ) {
                        scaleZ += 1.0f;
                        model = glm::translate(model, translate);
                        model = glm::scale(model, glm::vec3(1.0f, 1.0f, scaleZ));
                        model = glm::translate(model, glm::vec3(0.f, 0.f, translateFactor[scaleZ]));
                        faces[11].push_back(model);

                        coords.push_back(CoorPack(j, k, i, 0, 0, scaleZ));


                        model = glm::mat4(1.0f);
                        translate = glm::vec3(1.0f);
                        first = true;
                        scaleZ = 1;
                    } else
                        scaleZ += 1;
                }

                // if the current block is not grass and the previous block is grass
                else if (block[i][j][k] != GRASS and !first) {
                    model = glm::translate(model, translate);
                    model = glm::scale(model, glm::vec3(1.0f, 1.0f, scaleZ));
                    model = glm::translate(model, glm::vec3(0.f, 0.f, translateFactor[scaleZ]));
                    faces[11].push_back(model);

                    coords.push_back(CoorPack(j, k, i, 0, 0, scaleZ));


                    model = glm::mat4(1.0f);
                    translate = glm::vec3(1.0f);
                    first = true;
                    scaleZ = 1;
                }
            }
        }
    }
}

void GreedyChunk::test_moveChunkFace(float pixel) {
    for (unsigned int i{ 0 }; i < faces[11].size(); i++) {
        faces[11][i] = glm::translate(faces[11][i], glm::vec3(0, 0, pixel));
    }
}