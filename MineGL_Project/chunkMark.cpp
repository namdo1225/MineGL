#include "chunkMark.h"

float ChunkMark::quadVertices[] = {
    // Back face
    -0.025f, -0.025f, -0.025f, // 0
     0.025f,  0.025f, -0.025f, // 1
     0.025f, -0.025f, -0.025f, // 2
    -0.025f,  0.025f, -0.025f, // 3

    // Front face
    -0.025f, -0.025f,  0.025f, // 4
     0.025f, -0.025f,  0.025f, // 5
     0.025f,  0.025f,  0.025f, // 6
    -0.025f,  0.025f,  0.025f, // 7

    // Left face
    -0.025f, -0.025f, -0.025f, // 8
     0.025f,  0.025f,  0.025f, // 9
    -0.025f, -0.025f,  0.025f, // 10

     // Right face
     0.025f,  0.025f,  0.025f, // 11
     0.025f, -0.025f, -0.025f, // 12
     0.025f, -0.025f,  0.025f, // 13     

    // Bottom face
     0.025f, -0.025f, -0.025f, // 14

    // Top face
    -0.025f,  0.025f,  0.025f, // 15  
};

unsigned int ChunkMark::indices[] = {
    0,  1,  2,  1,  0,  3,
    4,  5,  6,  6,  7,  4,
    8,  9, 10, 10,  4,  8,
   11, 12,  1, 12, 11, 13,
   10, 14,  5,  5,  4, 10,
    3, 11,  1, 11,  3, 15
};

unsigned int ChunkMark::lineVAO = 0;
unsigned int ChunkMark::lineVBO = 0;
unsigned int ChunkMark::lineInstVBO = 0;
unsigned int ChunkMark::lineEBO = 0;

int ChunkMark::playerX = -10;
int ChunkMark::playerZ = -10;

glm::vec2 ChunkMark::translations[];

Shader ChunkMark::drawShader;

glm::mat4 ChunkMark::projection;
glm::mat4 ChunkMark::view;

ChunkMark::ChunkMark() {}

void ChunkMark::draw(Player& player, Camera& camera) {
    int playerChunkX = player.getPlayerChunk('x'), playerChunkZ = player.getPlayerChunk('z');

 
    if (playerChunkX != playerX or playerChunkZ != playerZ) {
        playerX = playerChunkX; playerZ = playerChunkZ;
        int minChunkX{ playerChunkX - 5 }, minChunkZ{ playerChunkZ - 5 };
        int maxChunkX{ playerChunkX + 5 }, maxChunkZ{ playerChunkZ + 5 };

        unsigned int index = 0;
        for (int i{ minChunkZ }; i < maxChunkZ; i++) { // z
            for (int j{ minChunkX }; j < maxChunkX; j++) { // x
                translations[index++] = glm::vec2(j * 9.6f, i * 9.6f);
                //translations[index++] = glm::vec2(0, 0);
            }
        }
    }

    //translations[0] = glm::vec3(9.6f, 250.0f, 9.6);


    // store instance data in an array buffer
    // --------------------------------------

    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    //view = camera.GetViewMatrix();

    //drawShader.SetMatrix4("view", view);

    glBindVertexArray(lineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lineEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // also set instance data
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, lineInstVBO); // this attribute comes from a different vertex buffer
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glVertexAttribDivisor(1, 1); // tell OpenGL this is an instanced vertex attribute.

    glBindBuffer(GL_ARRAY_BUFFER, lineInstVBO); // this attribute comes from a different vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(translations), translations, GL_STATIC_DRAW);

    drawShader.Use();


    glBindVertexArray(lineVAO);


    glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, 100); // 100 rectangles of 36 vertices each

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void ChunkMark::construct() {
    drawShader = ResourceManager::LoadShader("shader/2.6_instancing_chunkmap_vec3_vs.txt", "shader/2.6_instancing_chunkmap_vec3_fs.txt", nullptr, "ChunkMarkShader");
    glGenBuffers(1, &lineInstVBO);
    glGenVertexArrays(1, &lineVAO);
    glGenBuffers(1, &lineVBO);
    glGenBuffers(1, &lineEBO);
}

void ChunkMark::setProjection(glm::mat4 project) {
    drawShader.Use();
    drawShader.SetMatrix4("projection", project);
}


void ChunkMark::setView(glm::mat4 view) {
    drawShader.Use();
    drawShader.SetMatrix4("view", view);
}