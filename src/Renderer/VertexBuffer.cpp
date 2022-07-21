#include "VertexBuffer.h"

#include <vcruntime.h>

namespace Renderer {

	VertexBuffer::VertexBuffer() : id(NULL) {}
	VertexBuffer::VertexBuffer(VertexBuffer&& vertexBuffer) noexcept {
		id = vertexBuffer.id;
		vertexBuffer.id = NULL;
	}
	VertexBuffer::~VertexBuffer() {
		glDeleteBuffers(1, &id);
	}

	VertexBuffer& VertexBuffer::operator=(VertexBuffer&& vertexBuffer) noexcept {
		id = vertexBuffer.id;
		vertexBuffer.id = NULL;
		return *this;
	}

	void VertexBuffer::init(const void* data, const unsigned int& size) {
		glGenBuffers(1, &id);
		glBindBuffer(GL_ARRAY_BUFFER, id);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}
	void VertexBuffer::update(const void* data, const unsigned int& size) const {
		glBindBuffer(GL_ARRAY_BUFFER, id);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}
	void VertexBuffer::bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, id);
	}
	void VertexBuffer::unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, NULL);
	}

}