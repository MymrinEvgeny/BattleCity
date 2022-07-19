#pragma once

#include<string>
#include<memory>
#include<map>
#include<vector>

namespace Renderer {
	class ShaderProgram;
	class Texture2D;
	class Sprite;
	class AnimatedSprite;
}

class ResourceManager {
private:

	typedef std::map<const std::string, std::shared_ptr<Renderer::ShaderProgram>> 
		ShaderProgramsMap;
	typedef std::map<const std::string, std::shared_ptr<Renderer::Texture2D>>
		TexturesMap;
	typedef std::map<const std::string, std::shared_ptr<Renderer::Sprite>>
		SpritesMap;
	typedef std::map<const std::string, std::shared_ptr<Renderer::AnimatedSprite>>
		AnimatedSpritesMap;

	std::string executablePath;
	ShaderProgramsMap shaderPrograms;
	TexturesMap textures;
	SpritesMap sprites;
	AnimatedSpritesMap animatedSprites;

	std::string getFileString(const std::string& relativeFilePath) const;

public:
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&) = delete;
	ResourceManager(const std::string& executablePath);
	~ResourceManager() = default;

	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;

	std::shared_ptr<Renderer::ShaderProgram> loadShaders(
		const std::string& shaderProgramName, const std::string& vertexShaderPath,
		const std::string& fragmentShaderPath);
	std::shared_ptr<Renderer::ShaderProgram> getShaderProgram(
		const std::string& shaderProgramName);
	std::shared_ptr<Renderer::Texture2D> loadTexture(const std::string& textureName,
		const std::string& texturePath);
	std::shared_ptr<Renderer::Texture2D> getTexture(const std::string& textureName);
	std::shared_ptr<Renderer::Sprite> loadSprite(const std::string& spriteName,
		const std::string& textureName, const std::string& subTextureName,
		const std::string& shaderProgramName, const unsigned int spriteWidth,
		const unsigned int spriteHeight);
	std::shared_ptr<Renderer::Sprite> getSprite(const std::string& spriteName);
	std::shared_ptr<Renderer::Texture2D> loadTextureAtlas(
		const std::string& textureName, const std::string& texturePath,
		const std::vector<std::string>& subTextureNames,
		const unsigned int& subTextureWidth, const unsigned int& subTextureHeight);
	std::shared_ptr<Renderer::AnimatedSprite> loadAnimatedSprite(
		const std::string& animatedSpriteName, const std::string& textureName,
		const std::string& subTextureName, const std::string& shaderProgramName,
		const unsigned int spriteWidth, const unsigned int spriteHeight);
	std::shared_ptr<Renderer::AnimatedSprite> getAnimatedSprite(
		const std::string& animatedSpriteName);
};