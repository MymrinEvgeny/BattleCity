#pragma once

#include <glm/vec2.hpp>

#include<array>

class Game {
public:
	Game(const glm::ivec2& windowSize);
	~Game();

	void render();
	void update(const uint64_t delta);
	void setKey(const int& key, const int& action);
	bool init();
private:
	enum class GameState {
		Active,
		Pause
	};

	static const int KEYS_COUNT = 349;

	glm::ivec2 windowSize;
	GameState currentGameState;
	std::array<bool, KEYS_COUNT> keys;
};