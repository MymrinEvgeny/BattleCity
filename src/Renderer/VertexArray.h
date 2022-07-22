#pragma once
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#include <glad/glad.h>

namespace Renderer {

	class VertexArray {
	public:
		VertexArray();
		VertexArray(const VertexArray&) = delete;
		VertexArray(VertexArray&& vertexArray) noexcept;
		~VertexArray();

		VertexArray& operator=(const VertexArray&) = delete;
		VertexArray& operator=(VertexArray&& vertexArray) noexcept;

		void addVertexBuffer(const VertexBuffer& vertexBuffer,
			const VertexBufferLayout& vertexBufferLayout);
		void bind() const;
		void unbind() const;
	private:
		GLuint id;
		size_t elementsCount = 0;
	};

}