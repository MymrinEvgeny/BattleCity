#pragma once

#include <glm/vec2.hpp>

#include <memory>

namespace RenderEngine {
	class AnimatedSprite;
}

class Tank {
public:
	enum class Orientation {
		Top,
		Bottom,
		Left,
		Right
	};

	Tank(std::shared_ptr<RenderEngine::AnimatedSprite> pAnimatedSprite, const float& speed,
		const glm::vec2& position);

	void render() const;
	void update(const uint64_t delta);
	void setOrientation(Orientation orientation);
	void setMove(const bool& move);
private:
	Orientation orientation;
	std::shared_ptr<RenderEngine::AnimatedSprite> pAnimatedSprite;
	bool move;
	float speed;
	glm::vec2 position;
	glm::vec2 moveOffset;
};