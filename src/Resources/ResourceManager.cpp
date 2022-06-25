#include"ResourceManager.h"
#include"../Renderer/ShaderProgram.h"
#include"../Renderer/Texture2D.h"

#include<iostream>
#include<fstream>
#include<sstream>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

std::string ResourceManager::getFileString(const std::string& relativeFilePath) const {
	std::ifstream file(executablePath + '/' + relativeFilePath, std::ios::binary);
	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << relativeFilePath << std::endl;
		return std::string{};
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

ResourceManager::ResourceManager(const std::string& executablePath) {
	size_t found = executablePath.find_last_of("/\\");
	this->executablePath = executablePath.substr(0, found);
}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::loadShaders(
	const std::string& shaderName, const std::string& vertexShaderPath,
	const std::string& fragmentShaderPath) {

	std::string vertexShaderString = getFileString(vertexShaderPath);
	if (vertexShaderString.empty()) {
		std::cerr << "No vertex shader!" << std::endl;
		return nullptr;
	}

	std::string fragmentShaderString = getFileString(fragmentShaderPath);
	if (fragmentShaderString.empty()) {
		std::cerr << "No fragment shader!" << std::endl;
		return nullptr;
	}

	std::shared_ptr<Renderer::ShaderProgram>& newShaderProgram = shaderPrograms.emplace(
		shaderName, std::make_shared<Renderer::ShaderProgram>(
			vertexShaderString, fragmentShaderString)).first->second;
	if (!newShaderProgram->isCompiled()) {
		std::cerr << "Can't load shader program:\n"
			<< "Vertex: " << vertexShaderPath << '\n'
			<< "Fragment: " << fragmentShaderPath << std::endl;
		return nullptr;
	}
	return newShaderProgram;
}
std::shared_ptr<Renderer::ShaderProgram> ResourceManager::getShaderProgram(
	const std::string& shaderName) {

	ShaderProgramsMap::const_iterator it = shaderPrograms.find(shaderName);
	if (it == shaderPrograms.end()) {
		std::cerr << "Cant't find the shader program: " << shaderName << std::endl;
		return nullptr;
	}
	return it->second;
}
std::shared_ptr<Renderer::Texture2D> ResourceManager::loadTexture(
	const std::string& textureName, const std::string& texturePath) {
	
	int channels = NULL;
	int width = NULL;
	int height = NULL;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* pixels = stbi_load(
		std::string(executablePath + '/' + texturePath).c_str(), &width, &height,
		&channels, NULL);

	if (pixels == NULL) {
		std::cerr << "Can't load image: " << texturePath << std::endl;
		return nullptr;
	}

	std::shared_ptr<Renderer::Texture2D> newTexture = textures.emplace(textureName,
		std::make_shared<Renderer::Texture2D>(width, height, pixels, channels,
			GL_NEAREST, GL_CLAMP_TO_EDGE)).first->second;

	stbi_image_free(pixels);
	return newTexture;
}
std::shared_ptr<Renderer::Texture2D> ResourceManager::getTexture(const std::string& textureName) {
	TexturesMap::const_iterator it = textures.find(textureName);
	if (it == textures.end()) {
		std::cerr << "Cant't find the shader program: " << textureName << std::endl;
		return nullptr;
	}
	return it->second;
}