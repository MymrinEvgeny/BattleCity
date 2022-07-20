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

	static std::string executablePath;
	static ShaderProgramsMap shaderPrograms;
	static TexturesMap textures;
	static SpritesMap sprites;
	static AnimatedSpritesMap animatedSprites;

	static std::string getFileString(const std::string& relativeFilePath);

public:
	ResourceManager() = delete;
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&) = delete;
	~ResourceManager() = delete;

	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;

	static void setExecutablePath(const std::string& executablePath);
	static void unloadAllResources();
	static std::shared_ptr<Renderer::ShaderProgram> loadShaders(
		const std::string& shaderProgramName, const std::string& vertexShaderPath,
		const std::string& fragmentShaderPath);
	static std::shared_ptr<Renderer::ShaderProgram> getShaderProgram(
		const std::string& shaderProgramName);
	static std::shared_ptr<Renderer::Texture2D> loadTexture(const std::string& textureName,
		const std::string& texturePath);
	static std::shared_ptr<Renderer::Texture2D> getTexture(const std::string& textureName);
	static std::shared_ptr<Renderer::Sprite> loadSprite(const std::string& spriteName,
		const std::string& textureName, const std::string& subTextureName,
		const std::string& shaderProgramName, const unsigned int spriteWidth,
		const unsigned int spriteHeight);
	static std::shared_ptr<Renderer::Sprite> getSprite(const std::string& spriteName);
	static std::shared_ptr<Renderer::Texture2D> loadTextureAtlas(
		const std::string& textureName, const std::string& texturePath,
		const std::vector<std::string>& subTextureNames,
		const unsigned int& subTextureWidth, const unsigned int& subTextureHeight);
	static std::shared_ptr<Renderer::AnimatedSprite> loadAnimatedSprite(
		const std::string& animatedSpriteName, const std::string& textureName,
		const std::string& subTextureName, const std::string& shaderProgramName,
		const unsigned int spriteWidth, const unsigned int spriteHeight);
	static std::shared_ptr<Renderer::AnimatedSprite> getAnimatedSprite(
		const std::string& animatedSpriteName);
};