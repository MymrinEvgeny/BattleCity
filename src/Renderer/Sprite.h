#pragma once

#include<glad/glad.h>
#include<glm/vec2.hpp>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include<memory>
#include<string>

namespace RenderEngine {

	class Texture2D;
	class ShaderProgram;

	class Sprite {
	public:
		Sprite() = delete;
		Sprite(const Sprite&) = delete;
		Sprite(const std::shared_ptr<Texture2D> pTexture,
			const std::string& subTextureName,
			const std::shared_ptr<ShaderProgram> pShaderProgram,
			const glm::vec2& position = glm::vec2(0.0f),
			const glm::vec2& size = glm::vec2(1.0f),
			const float& rotation = 0.0f);
		~Sprite();

		Sprite& operator=(const Sprite&) = delete;

		virtual void render() const;
		void setPosition(const glm::vec2& position);
		void setSize(const glm::vec2& size);
		void setRotation(const float& rotation);
	protected:
		std::shared_ptr<Texture2D> pTexture;
		std::shared_ptr<ShaderProgram> pShaderProgram;
		glm::vec2 position;
		glm::vec2 size;
		float rotation;

		VertexArray vertexArray;
		VertexBuffer vertexCoordsBuffer;
		VertexBuffer textureCoordsBuffer;
		IndexBuffer indexBuffer;
	};
}