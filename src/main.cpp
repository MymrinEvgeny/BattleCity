#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <chrono>

#include "Renderer/ShaderProgram.h"
#include "Resources/ResourceManager.h"
#include "Renderer/Texture2D.h"
#include "Renderer/Sprite.h"
#include "Renderer/AnimatedSprite.h"

GLfloat points[] = {
    0.0f, 50.0f, 0.0f,
    50.0f, -50.0f, 0.0f,
    -50.0f, -50.0f, 0.0f
};

GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

GLfloat texCoord[] = {
    0.5f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f
};

glm::ivec2 windowSize(640, 480);

bool isEagle = false;

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height) {
    windowSize.x = width;
    windowSize.y = height;
    glViewport(0, 0, width, height);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
        isEagle = !isEagle;
    }
}

int main(int argc, char* argv[]) {

    /* Initialize the library */
    if (!glfwInit()) {
        std::cout << "glfwInit failed!" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* pWindow = glfwCreateWindow(windowSize.x, windowSize.y,
        "BattleCity", nullptr, nullptr);
    if (!pWindow) {
        std::cout << "glfwCreateWindow failed!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
    glfwSetKeyCallback(pWindow, glfwKeyCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(pWindow);
	
	if(!gladLoadGL()) {
		std::cout << "Can't load GLAD!" << std::endl;
		return -1;
	}
	
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    glClearColor(0, 0, 0.3f, 1);

    {
        ResourceManager resourceManager(argv[0]);

        auto pDefaultShaderProgram = resourceManager.loadShaders(
            "DefaultShaderProgram", "res/shaders/vertex.glsl",
            "res/shaders/fragment.glsl");
        if (pDefaultShaderProgram == nullptr) {
            std::cerr << "Can't create shader program: "
                << "DefaultShaderProgram" << std::endl;
            return -1;
        }
        auto pSpriteShaderProgram = resourceManager.loadShaders(
            "SpriteShaderProgram", "res/shaders/vertexSprite.glsl",
            "res/shaders/fragmentSprite.glsl");
        if (pSpriteShaderProgram == nullptr) {
            std::cerr << "Can't create shader program: "
                << "SpriteShaderProgram" << std::endl;
            return -1;
        }

        auto tex = resourceManager.loadTexture("DefaultTexture",
            "res/textures/map_16x16.png");
        if (tex == nullptr) {
            std::cerr << "Can't create texture: "
                << "DefaultTexture" << std::endl;
            return -1;
        }

        std::vector<std::string> subTextureNames = {
            "block",
            "topBlock",
            "bottomBlock",
            "leftBlock",
            "rightBlock",
            "topLeftBlock",
            "topRightBlock",
            "bottomLeftBlock",
            "bottomRightBlock",

            "metalBlock",
            "topMetalBlock",
            "bottomMetalBlock",
            "leftMetalBlock",
            "rightMetalBlock",
            "topLeftMetalBlock",
            "topRightMetalBlock",
            "bottomLeftMetalBlock",
            "bottomRightMetalBlock",

            "water1",
            "water2",
            "water3",
            "bush",
            "ice",
            "wall",

            "eagle",
            "deadEagle",
            "void",
            "respawn1",
            "respawn2",
            "respawn3",
            "respawn4" 
        };
        auto pTextureAtlas = resourceManager.loadTextureAtlas("DefaultTextureAtlas",
            "res/textures/map_16x16.png", subTextureNames, 16, 16);
        if (pTextureAtlas == nullptr) {
            std::cerr << "Can't create textureAtlas: "
                << "DefaultTextureAtlas" << std::endl;
            return -1;
        }

        auto pSprite = resourceManager.loadSprite("NewSprite", "DefaultTextureAtlas",
            "metalBlock", "SpriteShaderProgram", 100, 100);
        if (pSprite == nullptr) {
            std::cerr << "Can't create sprite: "
                << "NewSprite" << std::endl;
            return -1;
        }

        auto pAnimatedSprite = resourceManager.loadAnimatedSprite("NewAnimatedSprite",
            "DefaultTextureAtlas", "metalBlock", "SpriteShaderProgram", 100, 100);
        if (pAnimatedSprite == nullptr) {
            std::cerr << "Can't create animatedSprite: "
                << "NewAnimatedSprite" << std::endl;
            return -1;
        }

        std::vector<std::pair<std::string, uint64_t>> waterState;
        waterState.emplace_back(
            std::make_pair<std::string, uint64_t>("water1", 1000000000));
        waterState.emplace_back(
            std::make_pair<std::string, uint64_t>("water2", 1000000000));
        waterState.emplace_back(
            std::make_pair<std::string, uint64_t>("water3", 1000000000));

        std::vector<std::pair<std::string, uint64_t>> eagleState;
        eagleState.emplace_back(
            std::make_pair<std::string, uint64_t>("eagle", 1000000000));
        eagleState.emplace_back(
            std::make_pair<std::string, uint64_t>("deadEagle", 1000000000));

        pAnimatedSprite->insertState("waterState", waterState);
        pAnimatedSprite->insertState("eagleState", eagleState);

        pAnimatedSprite->setState("waterState");

        GLuint pointsVBO = NULL;
        glGenBuffers(1, &pointsVBO);
        glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

        GLuint colorsVBO = NULL;
        glGenBuffers(1, &colorsVBO);
        glBindBuffer(GL_ARRAY_BUFFER, colorsVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

        GLuint texCoordVBO = NULL;
        glGenBuffers(1, &texCoordVBO);
        glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(texCoord), texCoord, GL_STATIC_DRAW);

        GLuint VAO = NULL;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorsVBO);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        glm::mat4 modelMatrix1(1.0f);
        modelMatrix1 = glm::translate(modelMatrix1, glm::vec3(100.0f, 100.0f, 0.0f));

        glm::mat4 modelMatrix2(1.0f);
        modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(540.0f, 100.0f, 0.0f));

        glm::mat4 projectionMatrix = glm::ortho(
            0.0f, static_cast<float>(windowSize.x),
            0.0f, static_cast<float>(windowSize.y),
            0.0f, 1.0f);

        pDefaultShaderProgram->use();
        pDefaultShaderProgram->setInt("tex", 0);
        pDefaultShaderProgram->setMatrix4("projectionMatrix", projectionMatrix);

        pSpriteShaderProgram->use();
        pSpriteShaderProgram->setInt("tex", 0);
        pSpriteShaderProgram->setMatrix4("projectionMatrix", projectionMatrix);

        pSprite->setPosition(glm::vec2(200.0f, 200.0f));
        pAnimatedSprite->setPosition(glm::vec2(300.0f, 300.0f));

        auto lastTime = std::chrono::high_resolution_clock::now();

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(pWindow)) {

            if (isEagle) pAnimatedSprite->setState("eagleState");
            else  pAnimatedSprite->setState("waterState");

            auto currentTime = std::chrono::high_resolution_clock::now();
            uint64_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(
                currentTime - lastTime).count();
            lastTime = currentTime;
            pAnimatedSprite->update(duration);

            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            pDefaultShaderProgram->use();
            glBindVertexArray(VAO);
            tex->bind();

            pDefaultShaderProgram->setMatrix4("modelMatrix", modelMatrix1);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            pDefaultShaderProgram->setMatrix4("modelMatrix", modelMatrix2);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            pSprite->render();

            pAnimatedSprite->render();

            /* Swap front and back buffers */
            glfwSwapBuffers(pWindow);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}