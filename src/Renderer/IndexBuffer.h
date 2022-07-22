#pragma once

#include <glad/glad.h>

namespace RenderEngine {

	class IndexBuffer {
	public:
		IndexBuffer();
		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer(IndexBuffer&& indexBuffer) noexcept;
		~IndexBuffer();

		IndexBuffer& operator=(const IndexBuffer&) = delete;
		IndexBuffer& operator=(IndexBuffer&& indexBuffer) noexcept;

		void init(const void* data, const unsigned int& size);
		void bind() const;
		void unbind() const;
		GLsizei getCount() const;
	private:
		GLuint id;
		GLsizei count;
	};
}