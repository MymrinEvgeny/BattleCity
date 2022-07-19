#pragma once

#include "Sprite.h"

#include <map>
#include <vector>

namespace Renderer {

	class AnimatedSprite : public Sprite {
	public:
		AnimatedSprite(const std::shared_ptr<Texture2D> pTexture,
			const std::string subTextureName,
			const std::shared_ptr<ShaderProgram> pShaderProgram,
			const glm::vec2& position = glm::vec2(0.0f),
			const glm::vec2& size = glm::vec2(1.0f),
			const float& rotation = 0.0f);

		void insertState(const std::string& state,
			const std::vector<std::pair<std::string, uint64_t>>& subTexturesDuration);
		void setState(const std::string& newState);
		void update(const uint64_t& delta);
		void render() const override;
	private:
		typedef std::map<std::string, std::vector<std::pair<std::string, uint64_t>>>
			StatesMap;

		StatesMap statesMap;
		StatesMap::const_iterator itCurrentAnimationDurations;
		size_t currentFrame = 0;
		uint64_t currentAnimationTime = 0;
		mutable bool dirty = false;
	};
}