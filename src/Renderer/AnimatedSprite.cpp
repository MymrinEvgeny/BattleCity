#include "AnimatedSprite.h"
#include "Texture2D.h"

#include <iostream>

namespace Renderer {

	AnimatedSprite::AnimatedSprite(const std::shared_ptr<Texture2D> pTexture,
		const std::string subTextureName,
		const std::shared_ptr<ShaderProgram> pShaderProgram,
		const glm::vec2& position,
		const glm::vec2& size,
		const float& rotation) : Sprite(pTexture, subTextureName,
			pShaderProgram, position, size, rotation) {

		itCurrentAnimationDurations = statesMap.end();
	}

	void AnimatedSprite::insertState(const std::string& state,
		const std::vector<std::pair<std::string, uint64_t>>& subTexturesDuration) {

		statesMap.emplace(state, subTexturesDuration);
	}
	void AnimatedSprite::setState(const std::string& newState) {
		
		auto it = statesMap.find(newState);
		if (it == statesMap.end()) {
			std::cout << "Can't find animation state: " << newState << std::endl;
			return;
		}

		if (it != itCurrentAnimationDurations) {
			currentAnimationTime = 0;
			currentFrame = 0;
			itCurrentAnimationDurations = it;
			dirty = true;
		}

	}
	void AnimatedSprite::update(const uint64_t& delta) {

		if (itCurrentAnimationDurations != statesMap.end()) {
			currentAnimationTime += delta;
			while (currentAnimationTime >=
				itCurrentAnimationDurations->second[currentFrame].second) {

				currentAnimationTime -=
					itCurrentAnimationDurations->second[currentFrame].second;
				++currentFrame;
				dirty = true;

				if (currentFrame == itCurrentAnimationDurations->second.size()) {
					currentFrame = 0;
				}
			}
		}
	}
	void AnimatedSprite::render() const {

		if (dirty) {
			auto subTexture = pTexture->getSubTexture(
				itCurrentAnimationDurations->second[currentFrame].first);

			const GLfloat texCoords[] = {
				subTexture.leftBottom.x, subTexture.leftBottom.y,
				subTexture.leftBottom.x, subTexture.rightTop.y,
				subTexture.rightTop.x, subTexture.rightTop.y,
				subTexture.rightTop.x, subTexture.leftBottom.y
			};

			textureCoordsBuffer.update(texCoords, sizeof(texCoords));
			glBindBuffer(GL_ARRAY_BUFFER, NULL);
			dirty = false;
		}
		Sprite::render();
	}
}