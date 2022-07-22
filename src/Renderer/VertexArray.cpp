#include "VertexArray.h"

#include <vcruntime.h>

namespace RenderEngine {

	VertexArray::VertexArray() {
		glGenVertexArrays(1, &id);
	}
	VertexArray::VertexArray(VertexArray&& vertexArray) noexcept {
		id = vertexArray.id;
		vertexArray.id = NULL;
	}
	VertexArray::~VertexArray() {
		glDeleteVertexArrays(1, &id);
	}

	VertexArray& VertexArray::operator=(VertexArray&& vertexArray) noexcept {
		id = vertexArray.id;
		vertexArray.id = NULL;
		return *this;
	}

	void VertexArray::addVertexBuffer(const VertexBuffer& vertexBuffer,
		const VertexBufferLayout& vertexBufferLayout) {

		bind();
		vertexBuffer.bind();
		const auto& vertexBufferLayoutElements =
			vertexBufferLayout.getVertexBufferLayoutElements();
		RenderEngine::VertexBufferLayoutElement el;
		GLuint currentAttribIndex;
		GLint offset = 0;
		for (size_t i = 0; i < vertexBufferLayoutElements.size(); i++) {
			el = vertexBufferLayoutElements[i];
			currentAttribIndex = elementsCount + i;
			glEnableVertexAttribArray(currentAttribIndex);
			glVertexAttribPointer(currentAttribIndex, el.size, el.type, el.normalized,
				vertexBufferLayout.getStride(), reinterpret_cast<void*>(offset));
			offset += el.offset;
		}
		elementsCount += vertexBufferLayoutElements.size();
	}
	void VertexArray::bind() const {
		glBindVertexArray(id);
	}
	void VertexArray::unbind() const {
		glBindVertexArray(NULL);
	}

}