#pragma once

#include <glad/glad.h>

namespace RenderEngine {

	class VertexBuffer {
	public:
		VertexBuffer();
		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer(VertexBuffer&& vertexBuffer) noexcept;
		~VertexBuffer();

		VertexBuffer& operator=(const VertexBuffer&) = delete;
		VertexBuffer& operator=(VertexBuffer&& vertexBuffer) noexcept;

		void init(const void* data, const unsigned int& size);
		void update(const void* data, const unsigned int& size) const;
		void bind() const;
		void unbind() const;
	private:
		GLuint id;
	};
}