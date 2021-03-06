#include"Texture2D.h"

namespace RenderEngine {
	Texture2D::Texture2D(Texture2D&& texture2D) noexcept {
		ID = texture2D.ID;
		mode = texture2D.mode;
		width = texture2D.width;
		height = texture2D.height;
		texture2D.ID = NULL;
		texture2D.mode = NULL;
		texture2D.width = NULL;
		texture2D.height = NULL;
	}
	Texture2D::Texture2D(const GLuint width, const GLuint height, const unsigned char* data,
		const unsigned int channels, const GLenum filter, const GLenum wrapMode) {

		this->width = width;
		this->height = height;

		switch (channels)
		{
		case 4:
			mode = GL_RGBA;
			break;
		case 3:
			mode = GL_RGB;
			break;
		default:
			mode = GL_RGBA;
			break;
		}

		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);
		glTexImage2D(GL_TEXTURE_2D, 0, mode, this->width, this->height, NULL, mode,
			GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, NULL);
	}
	Texture2D::~Texture2D() {
		glDeleteTextures(1, &ID);
	}

	Texture2D& Texture2D::operator=(Texture2D&& texture2D) noexcept {
		glDeleteTextures(1, &ID);
		ID = texture2D.ID;
		mode = texture2D.mode;
		width = texture2D.width;
		height = texture2D.height;
		texture2D.ID = NULL;
		texture2D.mode = NULL;
		texture2D.width = NULL;
		texture2D.height = NULL;
		return *this;
	}

	unsigned int Texture2D::getWidth() const {
		return width;
	}
	unsigned int Texture2D::getHeight() const {
		return height;
	}
	void Texture2D::bind() const {
		glBindTexture(GL_TEXTURE_2D, ID);
	}
	void Texture2D::addSubTexture(const std::string& name, const glm::vec2& leftBottom,
		const glm::vec2& rightTop) {

		subTextures.emplace(name, SubTexture2D(leftBottom, rightTop));
	}
	Texture2D::SubTexture2D Texture2D::getSubTexture(const std::string& name) const {

		auto it = subTextures.find(name);
		if (it == subTextures.end()) {
			std::cerr << "Cant't find the subTexture: " << name << ", default texture will be returned" << std::endl;
			return SubTexture2D();
		}
		return it->second;
	}
}