#pragma once

#include<string>
#include<memory>
#include<map>

namespace Renderer {
	class ShaderProgram;
}

class ResourceManager {
private:

	typedef std::map<const std::string, std::shared_ptr<Renderer::ShaderProgram>> 
		ShaderProgramsMap;

	std::string executablePath;
	ShaderProgramsMap shaderPrograms;

	std::string getFileString(const std::string& relativeFilePath) const;

public:
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&) = delete;
	ResourceManager(const std::string& executablePath);
	~ResourceManager() = default;

	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;

	std::shared_ptr<Renderer::ShaderProgram> loadShaders(const std::string& shaderName,
		const std::string& vertexShaderPath,const std::string& fragmentShaderPath);
	std::shared_ptr<Renderer::ShaderProgram> getShaderProgram(const std::string& shaderName);
	void loadTexture(const std::string& textureName, const std::string& texturePath);
};