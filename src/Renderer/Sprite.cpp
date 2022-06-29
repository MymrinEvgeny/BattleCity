#include"Sprite.h"

#include<glm/mat4x4.hpp>
#include<glm/gtc/matrix_transform.hpp>

#include"ShaderProgram.h"
#include"Texture2D.h"

namespace Renderer {

	Sprite::Sprite(const std::shared_ptr<Texture2D> pTexture,
		const std::shared_ptr<ShaderProgram> pShaderProgram,
		const glm::vec2& position,
		const glm::vec2& size,
		const float& rotation) : 
		pTexture(pTexture), pShaderProgram(pShaderProgram),
		position(position), size(size), rotation(rotation){

		const GLfloat vertexCoords[] = {
			0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,

			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f
		};

		const GLfloat texCoords[] = {

			0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,

			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f
		};

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &vertexCoordsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, vertexCoordsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexCoords), vertexCoords,
			GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

		glGenBuffers(1, &textureCoordsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, textureCoordsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords,
			GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

		glBindBuffer(GL_ARRAY_BUFFER, NULL);
		glBindVertexArray(NULL);
	}
	Sprite::~Sprite() {
		glDeleteBuffers(1, &vertexCoordsVBO);
		glDeleteBuffers(1, &textureCoordsVBO);
		glDeleteVertexArrays(1, &VAO);
	}

	void Sprite::render() const {
		pShaderProgram->use();

		glm::mat4 model(1.0f);  
	
		model = glm::translate(model, glm::vec3(position, 0.0f));
		model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
		model = glm::scale(model, glm::vec3(size, 1.0f));

		glBindVertexArray(VAO);

		pShaderProgram->setMatrix4("modelMatrix", model);
		pTexture->bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(NULL);
	}
	void Sprite::setPosition(const glm::vec2& position) {
		this->position = position;
	}
	void Sprite::setSize(const glm::vec2& size) {
		this->size = size;
	}
	void Sprite::setRotation(const float rotation) {
		this->rotation = rotation;
	}
}