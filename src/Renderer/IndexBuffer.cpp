#include "IndexBuffer.h"

#include <vcruntime.h>

namespace RenderEngine {

	IndexBuffer::IndexBuffer() : id(NULL), count(0) {}
	IndexBuffer::IndexBuffer(IndexBuffer&& indexBuffer) noexcept {
		id = indexBuffer.id;
		count = indexBuffer.count;
		indexBuffer.id = NULL;
		indexBuffer.count = 0;
	}
	IndexBuffer::~IndexBuffer() {
		glDeleteBuffers(1, &id);
	}

	IndexBuffer& IndexBuffer::operator=(IndexBuffer&& indexBuffer) noexcept {
		id = indexBuffer.id;
		count = indexBuffer.count;
		indexBuffer.id = NULL;
		indexBuffer.count = 0;
		return *this;
	}

	void IndexBuffer::init(const void* data, const unsigned int& size) {
		glGenBuffers(1, &id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		count = size / sizeof(GLfloat);
	}
	void IndexBuffer::bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	}
	void IndexBuffer::unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
	}
	GLsizei IndexBuffer::getCount() const {
		return count;
	}
}