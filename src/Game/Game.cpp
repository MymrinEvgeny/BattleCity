#include "Game.h"
#include "../Resources/ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/AnimatedSprite.h"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

Game::Game(const glm::ivec2& windowSize) {
    this->windowSize = windowSize;
	currentGameState = GameState::Active;
	keys.fill(false);
}
Game::~Game() {

}

void Game::render() {
    ResourceManager::getAnimatedSprite("NewAnimatedSprite")->render();
}
void Game::update(const uint64_t delta) {
    ResourceManager::getAnimatedSprite("NewAnimatedSprite")->update(delta);
}
void Game::setKey(const int& key, const int& action) {
	keys[key] = action;
}
bool Game::init() {
    auto pDefaultShaderProgram = ResourceManager::loadShaders(
        "DefaultShaderProgram", "res/shaders/vertex.glsl",
        "res/shaders/fragment.glsl");
    if (pDefaultShaderProgram == nullptr) {
        std::cerr << "Can't create shader program: "
            << "DefaultShaderProgram" << std::endl;
        return false;
    }
    auto pSpriteShaderProgram = ResourceManager::loadShaders(
        "SpriteShaderProgram", "res/shaders/vertexSprite.glsl",
        "res/shaders/fragmentSprite.glsl");
    if (pSpriteShaderProgram == nullptr) {
        std::cerr << "Can't create shader program: "
            << "SpriteShaderProgram" << std::endl;
        return false;
    }

    auto tex = ResourceManager::loadTexture("DefaultTexture",
        "res/textures/map_16x16.png");
    if (tex == nullptr) {
        std::cerr << "Can't create texture: "
            << "DefaultTexture" << std::endl;
        return false;
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
    auto pTextureAtlas = ResourceManager::loadTextureAtlas("DefaultTextureAtlas",
        "res/textures/map_16x16.png", subTextureNames, 16, 16);
    if (pTextureAtlas == nullptr) {
        std::cerr << "Can't create textureAtlas: "
            << "DefaultTextureAtlas" << std::endl;
        return false;
    }

    auto pSprite = ResourceManager::loadSprite("NewSprite", "DefaultTextureAtlas",
        "metalBlock", "SpriteShaderProgram", 100, 100);
    if (pSprite == nullptr) {
        std::cerr << "Can't create sprite: "
            << "NewSprite" << std::endl;
        return false;
    }

    auto pAnimatedSprite = ResourceManager::loadAnimatedSprite("NewAnimatedSprite",
        "DefaultTextureAtlas", "metalBlock", "SpriteShaderProgram", 100, 100);
    if (pAnimatedSprite == nullptr) {
        std::cerr << "Can't create animatedSprite: "
            << "NewAnimatedSprite" << std::endl;
        return false;
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

	return true;
}