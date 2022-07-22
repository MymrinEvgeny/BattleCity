#pragma once

#include <glad/glad.h>

#include <vector>

namespace RenderEngine {

	struct VertexBufferLayoutElement {
		GLint size;
		GLenum type;
		GLboolean normalized;
		GLint offset;
	};

	class VertexBufferLayout {
	public:
		VertexBufferLayout();
		~VertexBufferLayout() = default;

		void addElementLayoutFloat(const GLint& size, const GLboolean& normalized);
		void reserveElements(const size_t& count);
		GLsizei getStride() const;
		const std::vector<VertexBufferLayoutElement>& getVertexBufferLayoutElements()
			const;
	private:
		std::vector<VertexBufferLayoutElement> vertexBufferLayoutElements;
		GLsizei stride;
	};
}