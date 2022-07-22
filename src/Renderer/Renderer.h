#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "ShaderProgram.h"

#include <string>

namespace RenderEngine {
	class Renderer {
	public:
		static void draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer,
			const ShaderProgram& shaderProgram);
		static void setClearColor(float r, float g, float b, float a);
		static void clear();
		static void setViewport(GLint x, GLint y, GLsizei width, GLsizei height);
		static std::string getRendererStr();
		static std::string getVersionStr();
	private:
	};
}