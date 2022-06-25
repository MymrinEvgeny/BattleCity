#pragma once

#include<glad/glad.h>
#include<string>

namespace Renderer {
	class Texture2D {
	private:

		GLuint ID;
		GLenum mode;
		unsigned int width;
		unsigned int height;

	public:
		Texture2D() = delete;
		Texture2D(const Texture2D&) = delete;
		Texture2D(Texture2D&& texture2D) noexcept;
		Texture2D(const GLuint width, const GLuint height, const unsigned char* data,
			const unsigned int channels, const GLenum filter, const GLenum wrapMode);
		~Texture2D();

		Texture2D& operator=(const Texture2D&) = delete;
		Texture2D& operator=(Texture2D&& texture2D) noexcept;

		void bind() const;
	};
}