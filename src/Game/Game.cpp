#include "Game.h"
#include "../Resources/ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/AnimatedSprite.h"
#include "Tank.h"

#include <GLFW/glfw3.h>
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
    if (pTank != nullptr) {
        pTank->render();
    }
}
void Game::update(const uint64_t delta) {
    ResourceManager::getAnimatedSprite("NewAnimatedSprite")->update(delta);

    if (pTank != nullptr) {
        if (keys[GLFW_KEY_W]) {
            pTank->setOrientation(Tank::Orientation::Top);
            pTank->setMove(true);
        }
        else if (keys[GLFW_KEY_A]) {
            pTank->setOrientation(Tank::Orientation::Left);
            pTank->setMove(true);
        }
        else if (keys[GLFW_KEY_S]) {
            pTank->setOrientation(Tank::Orientation::Bottom);
            pTank->setMove(true);
        }
        else if (keys[GLFW_KEY_D]) {
            pTank->setOrientation(Tank::Orientation::Right);
            pTank->setMove(true);
        }
        else pTank->setMove(false);
        pTank->update(delta);
    }
}
void Game::setKey(const int& key, const int& action) {
	keys[key] = action;
}
bool Game::init() {

    ResourceManager::loadJSONResources("res/resources.json");

    // SpriteShaderProgram
    auto pSpriteShaderProgram =
        ResourceManager::getShaderProgram("spriteShaderProgram");
    if (pSpriteShaderProgram == nullptr) {
        std::cerr << "Can't create shader program: "
            << "spriteShaderProgram" << std::endl;
        return false;
    }

    // MapTextureAtlas
    auto pMapTextureAtlas = ResourceManager::getTexture("mapTextureAtlas");
    if (pMapTextureAtlas == nullptr) {
        std::cerr << "Can't create textureAtlas: "
            << "mapTextureAtlas" << std::endl;
        return false;
    }

    // TanksTextureAtlas
    auto pTanksTextureAtlas = ResourceManager::getTexture("tanksTextureAtlas");
    if (pTanksTextureAtlas == nullptr) {
        std::cerr << "Can't create textureAtlas: "
            << "tanksTextureAtlas" << std::endl;
        return false;
    }

    // TanksAnimatedSprite
    auto pTanksAnimatedSprite = ResourceManager::getAnimatedSprite("tankAnimatedSprite");
    if (pTanksAnimatedSprite == nullptr) {
        std::cerr << "Can't create animatedSprite: "
            << "TanksAnimatedSprite" << std::endl;
        return false;
    }

    auto pSprite = ResourceManager::loadSprite("NewSprite", "mapTextureAtlas",
        "metalBlock", "spriteShaderProgram", 100, 100);
    if (pSprite == nullptr) {
        std::cerr << "Can't create sprite: "
            << "NewSprite" << std::endl;
        return false;
    }

    auto pAnimatedSprite = ResourceManager::loadAnimatedSprite("NewAnimatedSprite",
        "mapTextureAtlas", "metalBlock", "spriteShaderProgram", 100, 100);
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

    glm::mat4 projectionMatrix = glm::ortho(
        0.0f, static_cast<float>(windowSize.x),
        0.0f, static_cast<float>(windowSize.y),
        0.0f, 1.0f);

    pSpriteShaderProgram->use();
    pSpriteShaderProgram->setInt("tex", 0);
    pSpriteShaderProgram->setMatrix4("projectionMatrix", projectionMatrix);

    pSprite->setPosition(glm::vec2(200.0f, 200.0f));
    pAnimatedSprite->setPosition(glm::vec2(300.0f, 300.0f));

    pTanksAnimatedSprite->setState("yellowTank1Top");

    pTank = std::make_unique<Tank>(pTanksAnimatedSprite, 0.0000001, glm::vec2(200.0f, 200.0f));

	return true;
}