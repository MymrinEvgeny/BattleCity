#include "VertexBufferLayout.h"

namespace Renderer {

	VertexBufferLayout::VertexBufferLayout() : stride(0) {}

	void VertexBufferLayout::addElementLayoutFloat(const GLint& size,
		const GLboolean& normalized) {

		vertexBufferLayoutElements.push_back({ size, GL_FLOAT, normalized,
			static_cast<GLint>(size * sizeof(GLfloat)) });
		stride += vertexBufferLayoutElements.back().offset;
	}
	void VertexBufferLayout::reserveElements(const size_t& count) {
		vertexBufferLayoutElements.reserve(count);
	}
	GLsizei VertexBufferLayout::getStride() const {
		return stride;
	}
	const std::vector<VertexBufferLayoutElement>&
		VertexBufferLayout::getVertexBufferLayoutElements() const {
		return vertexBufferLayoutElements;
	}

}