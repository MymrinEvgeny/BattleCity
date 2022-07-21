#include "IndexBuffer.h"

#include <vcruntime.h>

namespace Renderer {

	IndexBuffer::IndexBuffer() : id(NULL) {}
	IndexBuffer::IndexBuffer(IndexBuffer&& indexBuffer) noexcept {
		id = indexBuffer.id;
		indexBuffer.id = NULL;
	}
	IndexBuffer::~IndexBuffer() {
		glDeleteBuffers(1, &id);
	}

	IndexBuffer& IndexBuffer::operator=(IndexBuffer&& indexBuffer) noexcept {
		id = indexBuffer.id;
		indexBuffer.id = NULL;
		return *this;
	}

	void IndexBuffer::init(const void* data, const unsigned int& size) {
		glGenBuffers(1, &id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}
	void IndexBuffer::bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	}
	void IndexBuffer::unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
	}

}