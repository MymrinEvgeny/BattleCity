#include "Renderer.h"

namespace RenderEngine {

	void Renderer::draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer,
		const ShaderProgram& shaderProgram) {

		shaderProgram.use();
		vertexArray.bind();
		indexBuffer.bind();

		glDrawElements(GL_TRIANGLES, indexBuffer.getCount(), GL_UNSIGNED_INT, nullptr);
	}
	void Renderer::setClearColor(float r, float g, float b, float a) {
		glClearColor(r, g, b, a);
	}
	void Renderer::clear() {
		glClear(GL_COLOR_BUFFER_BIT);
	}
	void Renderer::setViewport(GLint x, GLint y, GLsizei width, GLsizei height) {
		glViewport(x, y, width, height);
	}
	std::string Renderer::getRendererStr() {
		return reinterpret_cast<const char*>(glGetString(GL_RENDERER));
	}
	std::string Renderer::getVersionStr() {
		return reinterpret_cast<const char*>(glGetString(GL_VERSION));
	}
}