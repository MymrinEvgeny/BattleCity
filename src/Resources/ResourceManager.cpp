#include"ResourceManager.h"
#include"../Renderer/ShaderProgram.h"

#include<iostream>
#include<fstream>
#include<sstream>

std::string ResourceManager::getFileString(const std::string& relativeFilePath) const {
	std::ifstream file(executablePath + "/" + relativeFilePath, std::ios::binary);
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