#include"Sprite.h"

#include<glm/mat4x4.hpp>
#include<glm/gtc/matrix_transform.hpp>

#include<string>

#include"ShaderProgram.h"
#include"Texture2D.h"

namespace Renderer {

	Sprite::Sprite(const std::shared_ptr<Texture2D> pTexture,
		const std::string subTextureName,
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
			1.0f, 0.0f
		};

		auto subTexture = pTexture->getSubTexture(subTextureName);

		const GLfloat texCoords[] = {
			subTexture.leftBottom.x, subTexture.leftBottom.y,
			subTexture.leftBottom.x, subTexture.rightTop.y,
			subTexture.rightTop.x, subTexture.rightTop.y,
			subTexture.rightTop.x, subTexture.leftBottom.y
		};

		const GLuint indices[] = {
			0, 1, 2,
			0, 3, 2
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

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, NULL);
		glBindVertexArray(NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
	}
	Sprite::~Sprite() {
		glDeleteBuffers(1, &EBO);
		glDeleteBuffers(1, &textureCoordsVBO);
		glDeleteBuffers(1, &vertexCoordsVBO);
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
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

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