#pragma once

#include<glad/glad.h>
#include<string>

namespace Renderer {
	class ShaderProgram {
	private:

		bool programIsCompiled = false;
		GLuint programID = NULL;

		bool createShader(const std::string& source,
			const GLenum shaderType,
			GLuint& shaderID);

	public:
		ShaderProgram() = delete;
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram(ShaderProgram&&) noexcept;
		ShaderProgram(const std::string& vertexShaderSource,
			const std::string& fragmentShaderSource);
		~ShaderProgram();

		ShaderProgram& operator=(const ShaderProgram&) = delete;
		ShaderProgram& operator=(ShaderProgram&&) noexcept;

		bool isCompiled() const {
			return programIsCompiled;
		}
		void use() const;
		void setInt(const std::string& name, const GLint value);
	};
}