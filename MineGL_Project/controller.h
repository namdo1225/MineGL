#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "include.h"
#include "drawable.h"
#include "text.h"
#include "characters.h"
#include "camera.h"
#include "stb_image.h"
#include "characters.h"
#include "debugging.h"
#include "chunk.h"
#include "chunkMap.h"
#include "player.h"
#include "greedyChunk.h"
#include "chunkMark.h"

class Controller {
public:
	Controller(const char* title, int w, int h);
	~Controller();

	static void begin(const char* title, int w, int h);
	static void loop();
	static void end();

protected:

private:
	static GLFWwindow* window;

	static Camera camera;

	static float lastX;
	static float lastY;
	static bool firstMouse;

	// timing
	static float deltaTime;
	static float lastFrame;

	// lighting
	static glm::vec3 lightPos;

	static int screenWidth, screenHeight;
	static int flags;

	static std::vector<Drawable*> all;
	static std::vector<Text*> texts;
	static std::vector<unsigned int> cubeVBOs;

	static std::vector<Texture2D> textures;
	static std::map<GLchar, Character> Characters;

	static Shader cubeShader;

	static unsigned int cubeVAO;
	static unsigned int cubeVBO;
	static unsigned int instanceCubeVBO;
	static unsigned int cubeEBO;

	static float vertices[];

	static ChunkMap map;

	static glm::vec3 prevPlayerPos;
	static glm::vec3 playerPos;

	static glm::mat4 projection;
	static glm::mat4 view;

	static Player player;

	static bool wireFrameMode;


	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void processInput(GLFWwindow* window);

	static float testNumber;

	static std::vector<std::vector<glm::mat4>> faces;
	static unsigned int indices[][6];


	static float recursiveTranslate(float scaleFactor);

	static void createCube();
	static void createMenu();
	static void createChunkMark();

	static void drawCube();
	static void drawMenu();
	static void drawChunkMark();

	static bool chunkMark;
	static ChunkMark mark;
};

#endif