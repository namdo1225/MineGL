#include "controller.h"

GLFWwindow* Controller::window;

int Controller::screenWidth;
int Controller::screenHeight;

float Controller::lastX = 400.f;
float Controller::lastY = 600 / 2.0f;
bool Controller::firstMouse = true;

float Controller::deltaTime = 0.0f;
float Controller::lastFrame = 0.0f;

Camera Controller::camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
glm::vec3 Controller::lightPos = glm::vec3(0.f, 0.f, 0.f);

FT_Library Controller::ft;
FT_Face Controller::face;

std::map<GLchar, Character> Controller::Characters;

Shader Controller::cubeShader;

Shader Controller::textShader;

std::vector<unsigned int> Controller::cubeVBOs;

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

bool Controller::wireFrameMode{ false };

float Controller::testNumber = 0.0f;

Controller::Controller(const char* title, int w, int h) {
    begin(title, w, h);
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

    this->setupOGL();

    this->cubeShader = ResourceManager::LoadShader("shader/2.5_instancing_mat4_vs.txt", "shader/2.5_instancing_mat4_fs.txt", nullptr, "cubeShader");
    this->textShader = ResourceManager::LoadShader("shader/1.2_text_vs.txt", "shader/1.2_text_fs.txt", nullptr, "textShader");
    this->textures.push_back(ResourceManager::LoadTexture("texture/dirt_resize.jpg", true, "dirt"));
    this->textures.push_back(ResourceManager::LoadTexture("texture/grass_resize.jpg", true, "grass"));
    this->textures.push_back(ResourceManager::LoadTexture("texture/grass_dirt_resize.jpg", true, "grassDirt"));
    this->textures.push_back(ResourceManager::LoadTexture("texture/stone.jpg", true, "stone"));
    
    this->cubeShader.SetInteger("textureMain", 0, true);

}

void Controller::loop() {
    unsigned int counter{ 0 };
    double prevTime = 0.0;
    double currentTime = 0.0;
    double timeDiff;

    projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
    cubeShader.SetMatrix4("projection", projection);

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
        
        draw();


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

void Controller::setupOGL() {
    if (FT_Init_FreeType(&ft)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

    if (FT_New_Face(ft, "font/arial.ttf", 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }
    else {
        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 48);

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 128; c++)
        {
            // Load character glyph 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
            };
            Characters.insert(std::pair<char, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
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

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void Controller::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
    cubeShader.SetMatrix4("projection", projection);

    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void Controller::processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS and !wireFrameMode) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        wireFrameMode = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS and wireFrameMode) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        wireFrameMode = false;
    }



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

        //std::cout << player.getPlayerChunk('x') << " " << player.getPlayerChunk('z') << "\n";
        std::cout << testNumber << "\n";
    }

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        faces[0][1] = glm::translate(faces[0][1], glm::vec3(.0f, .0, .001f));
        faces[1][1] = glm::translate(faces[1][1], glm::vec3(.0f, .0, .001f));
        faces[2][1] = glm::translate(faces[2][1], glm::vec3(.0f, .0, .001f));
        faces[3][1] = glm::translate(faces[3][1], glm::vec3(.0f, .0, .001f));
        faces[4][1] = glm::translate(faces[4][1], glm::vec3(.0f, .0, .001f));
        faces[5][1] = glm::translate(faces[5][1], glm::vec3(.0f, .0, .001f));

        testNumber += .001f;
    }

    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
        faces[0][1] = glm::translate(faces[0][1], glm::vec3(.0f, .0, -.001f));
        faces[1][1] = glm::translate(faces[1][1], glm::vec3(.0f, .0, -.001f));
        faces[2][1] = glm::translate(faces[2][1], glm::vec3(.0f, .0, -.001f));
        faces[3][1] = glm::translate(faces[3][1], glm::vec3(.0f, .0, -.001f));
        faces[4][1] = glm::translate(faces[4][1], glm::vec3(.0f, .0, -.001f));
        faces[5][1] = glm::translate(faces[5][1], glm::vec3(.0f, .0, -.001f));
        
        testNumber -= 0.001f;
    }

    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
        faces[0][1] = glm::translate(faces[0][1], glm::vec3(.0f, .0, .15f));
        faces[1][1] = glm::translate(faces[1][1], glm::vec3(.0f, .0, .15f));
        faces[2][1] = glm::translate(faces[2][1], glm::vec3(.0f, .0, .15f));
        faces[3][1] = glm::translate(faces[3][1], glm::vec3(.0f, .0, .15f));
        faces[4][1] = glm::translate(faces[4][1], glm::vec3(.0f, .0, .15f));
        faces[5][1] = glm::translate(faces[5][1], glm::vec3(.0f, .0, .15f));
        
        testNumber += 0.15f;
    }

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
        faces[0][1] = glm::translate(faces[0][1], glm::vec3(.0f, .0, -.15f));
        faces[1][1] = glm::translate(faces[1][1], glm::vec3(.0f, .0, -.15f));
        faces[2][1] = glm::translate(faces[2][1], glm::vec3(.0f, .0, -.15f));
        faces[3][1] = glm::translate(faces[3][1], glm::vec3(.0f, .0, -.15f));
        faces[4][1] = glm::translate(faces[4][1], glm::vec3(.0f, .0, -.15f));
        faces[5][1] = glm::translate(faces[5][1], glm::vec3(.0f, .0, -.15f));
        
        testNumber -= 0.15f;
    }


}

void Controller::createCube() {
    glGenBuffers(1, &instanceCubeVBO);
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

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

void Controller::draw() {
    view = camera.GetViewMatrix();

    // pass transformation matrices to the shader
    cubeShader.Use();
    cubeShader.SetMatrix4("view", view);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 2.0f));
    cubeShader.SetMatrix4("model", model);


    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    map.draw(player, textures);

    /*
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
    }*/

    glBindVertexArray(0);
}