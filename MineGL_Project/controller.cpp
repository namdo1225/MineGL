#include "controller.h"

GLFWwindow* Controller::window;

int Controller::screenWidth;
int Controller::screenHeight;
int Controller::flags;

float Controller::lastX = 400.f;
float Controller::lastY = 600 / 2.0f;
bool Controller::firstMouse = true;

float Controller::deltaTime = 0.0f;
float Controller::lastFrame = 0.0f;

Camera Controller::camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
glm::vec3 Controller::lightPos = glm::vec3(0.f, 0.f, 0.f);

std::map<GLchar, Character> Controller::Characters;

Shader Controller::cubeShader;

std::vector<unsigned int> Controller::cubeVBOs;
std::vector<Drawable*> Controller::all;
std::vector<Text*> Controller::texts;

unsigned int Controller::cubeVAO;
unsigned int Controller::cubeVBO;
unsigned int Controller::cubeEBO;
unsigned int Controller::instanceCubeVBO;

std::vector<Texture2D> Controller::textures;

glm::vec3 Controller::playerPos;
glm::vec3 Controller::prevPlayerPos;

ChunkMap Controller::map;

glm::mat4 Controller::projection;
glm::mat4 Controller::view;

float Controller::vertices[] = {
    // Back face
    // Look at the pattern of x and y and compare them with the UV
    -0.3f, -0.3f, -0.3f,  0.0f, 0.0f, -0.36f,  1.0f, 1.0f, // 0
     0.3f,  0.3f, -0.3f,  0.0f, 0.0f, -0.36f,  0.0f, 0.0f, // 1
     0.3f, -0.3f, -0.3f,  0.0f, 0.0f, -0.36f,  0.0f, 1.0f, // 2
    -0.3f,  0.3f, -0.3f,  0.0f, 0.0f, -0.36f,  1.0f, 0.0f, // 3

    // Front face
    -0.3f, -0.3f,  0.3f,  0.0f, 0.0f,  0.36f,  0.0f, 1.0f, // 4
     0.3f, -0.3f,  0.3f,  0.0f, 0.0f,  0.36f,  1.0f, 1.0f, // 5
     0.3f,  0.3f,  0.3f,  0.0f, 0.0f,  0.36f,  1.0f, 0.0f, // 6
    -0.3f,  0.3f,  0.3f,  0.0f, 0.0f,  0.36f,  0.0f, 0.0f, // 7

    // Left face
    -0.3f,  0.3f,  0.3f,  0.36f, 0.0f,  0.0f,  1.0f, 0.0f, // 8
    -0.3f,  0.3f, -0.3f,  0.36f, 0.0f,  0.0f,  0.0f, 0.0f, // 9
    -0.3f, -0.3f, -0.3f,  0.36f, 0.0f,  0.0f,  0.0f, 1.0f, // 10
    -0.3f, -0.3f,  0.3f,  0.36f, 0.0f,  0.0f,  1.0f, 1.0f, // 11

     // Right face
     0.3f,  0.3f,  0.3f, -0.36f, 0.0f,  0.0f,  0.0f, 0.0f, // 12
     0.3f, -0.3f, -0.3f, -0.36f, 0.0f,  0.0f,  1.0f, 1.0f, // 13
     0.3f, -0.3f,  0.3f, -0.36f, 0.0f,  0.0f,  0.0f, 1.0f, // 14
     0.3f,  0.3f, -0.3f, -0.36f, 0.0f,  0.0f,  1.0f, 0.0f, // 15

    // Bottom face
     0.3f, -0.3f,  0.3f,  0.0f, 0.36f,  0.0f,  1.0f, 0.0f, // 16
    -0.3f, -0.3f, -0.3f,  0.0f, 0.36f,  0.0f,  0.0f, 1.0f, // 17
    -0.3f, -0.3f,  0.3f,  0.0f, 0.36f,  0.0f,  0.0f, 0.0f, // 18
     0.3f, -0.3f, -0.3f,  0.0f, 0.36f,  0.0f,  1.0f, 1.0f, // 19 

    // Top face
     0.3f,  0.3f,  0.3f,  0.0f, -0.36f, 0.0f,  1.0f, 1.0f, // 20
    -0.3f,  0.3f, -0.3f,  0.0f, -0.36f, 0.0f,  0.0f, 0.0f, // 21
    -0.3f,  0.3f,  0.3f,  0.0f, -0.36f, 0.0f,  0.0f, 1.0f, // 22
     0.3f,  0.3f, -0.3f,  0.0f, -0.36f, 0.0f,  1.0f, 0.0f  // 23
};

Player Controller::player;

bool Controller::wireFrameMode{ false }, Controller::cullMode{ true }, Controller::chunkMark{ false },
Controller::hideCommands{ false };

float Controller::testNumber = 0.0f;

ChunkMark Controller::mark;

Controller::Controller(const char* title, int w, int h) {
    begin(title, w, h);
    createCube();
}

Controller::~Controller() {}

void Controller::begin(const char* title, int w, int h) {
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    screenWidth = w, screenHeight = h;

    window = glfwCreateWindow(w, h, title, NULL, NULL);

    // glfw window creation
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_DEBUG_OUTPUT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    createChunkMark();
    createMenu();

    cubeShader = ResourceManager::LoadShader("shader/2.5_instancing_mat4_vs.txt", "shader/2.5_instancing_mat4_fs.txt", nullptr, "cubeShader");
    //this->textShader = ResourceManager::LoadShader("shader/1.2_text_vs.txt", "shader/1.2_text_fs.txt", nullptr, "textShader");
    textures.push_back(ResourceManager::LoadTexture("texture/dirt_resize.jpg", true, "dirt"));
    textures.push_back(ResourceManager::LoadTexture("texture/grass_resize.jpg", true, "grass"));
    textures.push_back(ResourceManager::LoadTexture("texture/grass_dirt_resize.jpg", true, "grassDirt"));
    textures.push_back(ResourceManager::LoadTexture("texture/stone.jpg", true, "stone"));
    
    cubeShader.SetInteger("textureMain", 0, true);

}

void Controller::loop() {
    unsigned int counter{ 0 };
    double prevTime = 0.0;
    double currentTime = 0.0;
    double timeDiff;

    projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
    cubeShader.SetMatrix4("projection", projection);
    mark.setProjection(projection);

    while (!glfwWindowShouldClose(window))
    {
        currentTime = glfwGetTime();
        timeDiff = currentTime - prevTime;
        counter++;

        if (timeDiff >= 1.0 / 30.0) {
            std::string FPS = std::to_string((1.0 / timeDiff) * counter);
            std::string ms = std::to_string((timeDiff / counter) * 1000);
            std::string title = "MineGL: " + FPS + "FPS / " + ms + "ms";
            glfwSetWindowTitle(window, title.c_str());

            prevTime = currentTime;
            counter = 0;
        }

        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.4f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        drawCube();
        drawChunkMark();
        drawMenu();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Controller::end() {
    for (unsigned int i{ 0 }; i < all.size(); i++)
        all[i]->deconstruct();

    for (unsigned int i{ 0 }; i < texts.size(); i++)
        texts[i]->deconstruct();

    all.erase(all.begin(), all.end());
    texts.erase(texts.begin(), texts.end());

    map.cleanChunks();

    glfwTerminate();
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void Controller::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
void Controller::mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (!glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        camera.ProcessMouseMovement(xoffset, yoffset);

        lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
    }
    else if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Controller::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    if (!glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
        return;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        if (xpos > 1150 && xpos < 1200 && ypos > 0 && ypos < 100) {
            hideCommands = !hideCommands;
        }

        if (hideCommands)
            return;

        if (xpos > 0 && xpos < 100 && ypos > 0 && ypos < 100)
            camera.teleport(0.f, 125.f, 0.f);
        else if (xpos > 120 && xpos < 180 && ypos > 0 && ypos < 100) {
            chunkMark = !chunkMark          ;
        }
        else if (xpos > 200 and xpos < 320 and ypos > 0 and ypos < 100) {
            if (!wireFrameMode) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                wireFrameMode = true;
            }
            else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                wireFrameMode = false;
            }
        }
        else if (xpos > 340 && xpos < 380 && ypos > 0 && ypos < 100) {
            if (!cullMode) {
                glEnable(GL_CULL_FACE);
                cullMode = true;
            }
            else {
                glDisable(GL_CULL_FACE);
                cullMode = false;
            }
        }
    }

}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void Controller::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
    cubeShader.SetMatrix4("projection", projection);
    mark.setProjection(projection);

    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void Controller::processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float speed = 1.0f;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
        speed = 10.0f;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS or
        glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS or
        glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS or
        glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            player.updatePlayerPos(camera.ProcessKeyboard(FORWARD, deltaTime * speed));
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            player.updatePlayerPos(camera.ProcessKeyboard(BACKWARD, deltaTime * speed));
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            player.updatePlayerPos(camera.ProcessKeyboard(LEFT, deltaTime * speed));
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            player.updatePlayerPos(camera.ProcessKeyboard(RIGHT, deltaTime * speed));

        texts[6]->changeText(std::to_string(player.getPlayerChunk('x')) + "/" + std::to_string(player.getPlayerChunk('z')));

        texts[7]->changeText(std::to_string(abs(player.getPlayerPos().x - (player.getPlayerChunk('x') * 9.6))) + "/"
            + std::to_string(abs(player.getPlayerPos().z / 9.6 - (player.getPlayerChunk('z') * 9.6))));

        texts[8]->changeText(std::to_string(player.getPlayerPos().x) + "/"
            + std::to_string(player.getPlayerPos().y) + "/"
            + std::to_string(player.getPlayerPos().z));
    }
}

void Controller::createCube() {
    glGenBuffers(1, &instanceCubeVBO);
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

    map.setUp();
}

float Controller::recursiveTranslate(float scaleFactor) {
    if (scaleFactor == 1.0f)
        return 0.0f;
    else if (scaleFactor == 2.0f)
        return 0.15f;

    return 0.05 / scaleFactor + recursiveTranslate(scaleFactor);
}

unsigned int Controller::indices[][6] = {
    {0,  1,  2,  1,  0,  3 },
    {4,  5,  6,  6,  7,  4 },
    {8,  9, 10, 10, 11,  8 },
    {12, 15, 13, 13, 14, 12},
    {17, 19, 16, 16, 18, 17},
    {21, 20, 23, 20, 21, 22}
};

std::vector<std::vector<glm::mat4>> Controller::faces;

void Controller::drawCube() {
    view = camera.GetViewMatrix();

    // pass transformation matrices to the shader
    mark.setView(view);
    cubeShader.Use();
    cubeShader.SetMatrix4("view", view);

    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);

    glBindBuffer(GL_ARRAY_BUFFER, instanceCubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

    // number of floats per line * float size * # of square 's edges * # of cube's sides 
    glBufferData(GL_ARRAY_BUFFER, 8 * 4 * 6 * 6, vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // mat4 instance attributes
    glBindBuffer(GL_ARRAY_BUFFER, instanceCubeVBO);

    std::size_t vec4Size = sizeof(glm::vec4);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    map.draw(player, textures);

    glBindVertexArray(0);
}

void Controller::createMenu() {
    texts.push_back(new Text());
    texts[0]->setupOGL();
    texts[0]->loadShader();

    texts[0]->construct("Reset Pos", 0, 780, 0.5, glm::vec4(1, 1, 1, 0.5), 1200, 800);

    texts.push_back(new Text());
    texts[1]->construct("Chunk", 120, 780, 0.5, glm::vec4(1, 1, 1, 0.5), 1200, 800);

    texts.push_back(new Text());
    texts[2]->construct("Wireframe", 200, 780, 0.5, glm::vec4(1, 1, 1, 0.5), 1200, 800);

    texts.push_back(new Text());
    texts[3]->construct("Chunk X/Z:", 0, 20, 0.5, glm::vec4(1, 1, 1, 0.5), 1200, 800);

    texts.push_back(new Text());
    texts[4]->construct("Relative X/Z:", 0, 40, 0.5, glm::vec4(1, 1, 1, 0.5), 1200, 800);

    texts.push_back(new Text());
    texts[5]->construct("Absolute X/Y/Z:", 0, 60, 0.5, glm::vec4(1, 1, 1, 0.5), 1200, 800);

    texts.push_back(new Text());
    texts[6]->construct("", 180, 20, 0.5, glm::vec4(1, 1, 1, 0.5), 1200, 800);

    texts.push_back(new Text());
    texts[7]->construct("", 180, 40, 0.5, glm::vec4(1, 1, 1, 0.5), 1200, 800);

    texts.push_back(new Text());
    texts[8]->construct("", 180, 60, 0.5, glm::vec4(1, 1, 1, 0.5), 1200, 800);

    texts.push_back(new Text());
    texts[9]->construct("Cull", 320, 780, 0.5, glm::vec4(1, 1, 1, 0.5), 1200, 800);

    texts.push_back(new Text());
    texts[10]->construct("X", 1180, 780, 0.5, glm::vec4(1, 1, 1, 0.5), 1200, 800);
}

void Controller::drawMenu() {
    texts[10]->draw();

    if (hideCommands)
        return;

    texts[0]->draw();
    texts[1]->draw();
    texts[2]->draw();

    texts[3]->draw();
    texts[4]->draw();
    texts[5]->draw();

    texts[6]->draw();
    texts[7]->draw();
    texts[8]->draw();

    texts[9]->draw();
}

void Controller::createChunkMark() {
    mark = ChunkMark();
    mark.construct();
}

void Controller::drawChunkMark() {
    if (!chunkMark)
        return;

    mark.draw(player, camera);
}