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

class Controller {
public:
	Controller(const char* title, int w, int h);
	~Controller();

	void begin(const char* title, int w, int h);
	void loop();
	void end();

	static void setupOGL();

	static void createCube();

	static void drawCube();

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
	int flags;

	std::vector<Drawable*> all;
	std::vector<Text*> texts;
	static std::vector<unsigned int> cubeVBOs;

	static FT_Library ft;
	static FT_Face face;

	static std::vector<Texture2D> textures;
	static std::map<GLchar, Character> Characters;

	static Shader cubeShader;
	static Shader textShader;

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

	void createMenu();
};

#endif