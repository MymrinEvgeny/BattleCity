#include "Tank.h"
#include "../Renderer/AnimatedSprite.h"

Tank::Tank(std::shared_ptr<RenderEngine::AnimatedSprite> pAnimatedSprite, const float& speed,
	const glm::vec2& position) : orientation(Orientation::Top),
	pAnimatedSprite(pAnimatedSprite), move(false), speed(speed), position(position),
	moveOffset(glm::vec2(0.0f, 1.0f)) {

	pAnimatedSprite->setPosition(position);
}

void Tank::render() const {
	pAnimatedSprite->render();
}
void Tank::update(const uint64_t delta) {
	if (move) {
		position += delta * speed * moveOffset;
		pAnimatedSprite->setPosition(position);
		pAnimatedSprite->update(delta);

	}
}
void Tank::setOrientation(Orientation orientation) {
	if (this->orientation == orientation) return;
	this->orientation = orientation;
	switch (orientation)
	{
	case Tank::Orientation::Top:
		pAnimatedSprite->setState("yellowTank1Top");
		moveOffset.x = 0.0f;
		moveOffset.y = 1.0f;
		break;
	case Tank::Orientation::Bottom:
		pAnimatedSprite->setState("yellowTank1Bottom");
		moveOffset.x = 0.0f;
		moveOffset.y = -1.0f;
		break;
	case Tank::Orientation::Left:
		pAnimatedSprite->setState("yellowTank1Left");
		moveOffset.x = -1.0f;
		moveOffset.y = 0.0f;
		break;
	case Tank::Orientation::Right:
		pAnimatedSprite->setState("yellowTank1Right");
		moveOffset.x = 1.0f;
		moveOffset.y = 0.0f;
		break;
	default:
		break;
	}
}
void Tank::setMove(const bool& move) {
	this->move = move;
}