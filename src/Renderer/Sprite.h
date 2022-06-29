#pragma once

#include<glad/glad.h>
#include<glm/vec2.hpp>

#include<memory>

namespace Renderer {

	class Texture2D;
	class ShaderProgram;

	class Sprite {
	private:

		std::shared_ptr<Texture2D> pTexture;
		std::shared_ptr<ShaderProgram> pShaderProgram;
		glm::vec2 position;
		glm::vec2 size;
		float rotation;

		GLuint VAO;
		GLuint vertexCoordsVBO;
		GLuint textureCoordsVBO;

	public:
		Sprite() = delete;
		Sprite(const Sprite&) = delete;
		Sprite(const std::shared_ptr<Texture2D> pTexture,
			const std::shared_ptr<ShaderProgram> pShaderProgram,
			const glm::vec2& position = glm::vec2(0.0f),
			const glm::vec2& size = glm::vec2(1.0f),
			const float& rotation = 0.0f);
		~Sprite();

		Sprite& operator=(const Sprite&) = delete;

		void render() const;
		void setPosition(const glm::vec2& position);
		void setSize(const glm::vec2& size);
		void setRotation(const float rotation);
	};
}