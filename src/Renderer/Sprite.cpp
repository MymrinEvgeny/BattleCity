#include"Sprite.h"

#include<glm/mat4x4.hpp>
#include<glm/gtc/matrix_transform.hpp>

#include<string>

#include"ShaderProgram.h"
#include"Texture2D.h"

namespace Renderer {

	Sprite::Sprite(const std::shared_ptr<Texture2D> pTexture,
		const std::string& subTextureName,
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

		vertexCoordsBuffer.init(vertexCoords, sizeof(vertexCoords));
		VertexBufferLayout vertexCoordsBufferLayout;
		vertexCoordsBufferLayout.addElementLayoutFloat(2, GL_FALSE);
		vertexArray.addVertexBuffer(vertexCoordsBuffer, vertexCoordsBufferLayout);

		textureCoordsBuffer.init(texCoords, sizeof(texCoords));
		VertexBufferLayout textureCoordsBufferLayout;
		textureCoordsBufferLayout.addElementLayoutFloat(2, GL_FALSE);
		vertexArray.addVertexBuffer(textureCoordsBuffer, textureCoordsBufferLayout);

		indexBuffer.init(indices, sizeof(indices));

		vertexCoordsBuffer.unbind(); // textureCoordsBuffer.unbind();
		vertexArray.unbind();
		indexBuffer.unbind();
	}
	Sprite::~Sprite() {}

	void Sprite::render() const {
		pShaderProgram->use();

		glm::mat4 model(1.0f);  
	
		model = glm::translate(model, glm::vec3(position, 0.0f));
		model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
		model = glm::scale(model, glm::vec3(size, 1.0f));

		vertexArray.bind();

		pShaderProgram->setMatrix4("modelMatrix", model);
		pTexture->bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		vertexArray.unbind();
	}
	void Sprite::setPosition(const glm::vec2& position) {
		this->position = position;
	}
	void Sprite::setSize(const glm::vec2& size) {
		this->size = size;
	}
	void Sprite::setRotation(const float& rotation) {
		this->rotation = rotation;
	}
}