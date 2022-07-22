#pragma once

#include<glad/glad.h>
#include<glm/vec2.hpp>

#include<iostream>
#include<string>
#include<map>

namespace RenderEngine {
	class Texture2D {
	public:

		class SubTexture2D {
		public:

			glm::vec2 leftBottom;
			glm::vec2 rightTop;

			SubTexture2D() : leftBottom(0.0f), rightTop(1.0f) {}
			SubTexture2D(const glm::vec2& leftBottom, const glm::vec2& rightTop) :
				leftBottom(leftBottom), rightTop(rightTop) {}
		};

		Texture2D() = delete;
		Texture2D(const Texture2D&) = delete;
		Texture2D(Texture2D&& texture2D) noexcept;
		Texture2D(const GLuint width, const GLuint height, const unsigned char* data,
			const unsigned int channels, const GLenum filter, const GLenum wrapMode);
		~Texture2D();

		Texture2D& operator=(const Texture2D&) = delete;
		Texture2D& operator=(Texture2D&& texture2D) noexcept;

		unsigned int getWidth() const;
		unsigned int getHeight() const;
		void bind() const;
		void addSubTexture(const std::string& name, const glm::vec2& leftBottom,
			const glm::vec2& rightTop);
		SubTexture2D getSubTexture(const std::string& name) const;

	private:

		GLuint ID;
		GLenum mode;
		unsigned int width;
		unsigned int height;

		std::map<std::string, SubTexture2D> subTextures;
	};
}