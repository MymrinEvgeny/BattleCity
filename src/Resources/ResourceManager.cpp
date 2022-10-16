#include "ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/AnimatedSprite.h"

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

#include <iostream>
#include <fstream>
#include <sstream>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

std::string ResourceManager::executablePath;
ResourceManager::ShaderProgramsMap ResourceManager::shaderPrograms;
ResourceManager::TexturesMap ResourceManager::textures;
ResourceManager::SpritesMap ResourceManager::sprites;
ResourceManager::AnimatedSpritesMap ResourceManager::animatedSprites;

std::string ResourceManager::getFileString(const std::string& relativeFilePath) {
	std::ifstream file(executablePath + '/' + relativeFilePath, std::ios::binary);
	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << relativeFilePath << std::endl;
		return std::string{};
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

void ResourceManager::setExecutablePath(const std::string& executablePath) {
	size_t found = executablePath.find_last_of("/\\");
	ResourceManager::executablePath = executablePath.substr(0, found);
}
void ResourceManager::unloadAllResources() {
	shaderPrograms.clear();
	textures.clear();
	sprites.clear();
	animatedSprites.clear();
}
std::shared_ptr<RenderEngine::ShaderProgram> ResourceManager::loadShaders(
	const std::string& shaderProgramName, const std::string& vertexShaderPath,
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

	std::shared_ptr<RenderEngine::ShaderProgram>& newShaderProgram = shaderPrograms.emplace(
		shaderProgramName, std::make_shared<RenderEngine::ShaderProgram>(
			vertexShaderString, fragmentShaderString)).first->second;
	if (!newShaderProgram->isCompiled()) {
		std::cerr << "Can't load shader program:\n"
			<< "Vertex: " << vertexShaderPath << '\n'
			<< "Fragment: " << fragmentShaderPath << std::endl;
		return nullptr;
	}
	return newShaderProgram;
}
std::shared_ptr<RenderEngine::ShaderProgram> ResourceManager::getShaderProgram(
	const std::string& shaderProgramName) {

	ShaderProgramsMap::const_iterator it = shaderPrograms.find(shaderProgramName);
	if (it == shaderPrograms.end()) {
		std::cerr << "Cant't find the shader program: " 
			<< shaderProgramName << std::endl;
		return nullptr;
	}
	return it->second;
}
std::shared_ptr<RenderEngine::Texture2D> ResourceManager::loadTexture(
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

	std::shared_ptr<RenderEngine::Texture2D> newTexture = textures.emplace(textureName,
		std::make_shared<RenderEngine::Texture2D>(width, height, pixels, channels,
			GL_NEAREST, GL_CLAMP_TO_EDGE)).first->second;

	stbi_image_free(pixels);
	return newTexture;
}
std::shared_ptr<RenderEngine::Texture2D> ResourceManager::getTexture(
	const std::string& textureName) {

	TexturesMap::const_iterator it = textures.find(textureName);
	if (it == textures.end()) {
		std::cerr << "Cant't find the texture: " << textureName << std::endl;
		return nullptr;
	}
	return it->second;
}
std::shared_ptr<RenderEngine::Sprite> ResourceManager::loadSprite(
	const std::string& spriteName, const std::string& textureName,
	const std::string& subTextureName, const std::string& shaderProgramName,
	const unsigned int spriteWidth, const unsigned int spriteHeight) {

	auto pTexture = getTexture(textureName);
	if (pTexture == nullptr) {
		std::cerr << "Cant't find the texture: " << textureName << std::endl;
		return nullptr;
	}

	auto pShaderProgram = getShaderProgram(shaderProgramName);
	if (pShaderProgram == nullptr) {
		std::cerr << "Cant't find the shader program: " 
			<< shaderProgramName << std::endl;
		return nullptr;
	}

	std::shared_ptr<RenderEngine::Sprite> newSprite = sprites.emplace(spriteName,
		std::make_shared<RenderEngine::Sprite>(pTexture, subTextureName, pShaderProgram,
			glm::vec2(0.0f), glm::vec2(spriteWidth, spriteHeight))).first->second;

	return	newSprite;
}
std::shared_ptr<RenderEngine::Sprite> ResourceManager::getSprite(
	const std::string& spriteName) {

	SpritesMap::const_iterator it = sprites.find(spriteName);
	if (it == sprites.end()) {
		std::cerr << "Cant't find the sprite: " << spriteName << std::endl;
		return nullptr;
	}
	return it->second;
}
std::shared_ptr<RenderEngine::Texture2D> ResourceManager::loadTextureAtlas(
	const std::string& textureName, const std::string& texturePath,
	const std::vector<std::string>& subTextureNames, const unsigned int& subTextureWidth,
	const unsigned int& subTextureHeight) {

	auto pTexture = loadTexture(textureName, texturePath);
	if (pTexture == nullptr) {
		return nullptr;
	}
	
	const unsigned int textureWidth = pTexture->getWidth();
	const unsigned int textureHeight = pTexture->getHeight();

	unsigned int currentTextureOffsetX = 0;
	unsigned int currentTextureOffsetY = textureHeight;

	for (const auto& currentSubTextureName : subTextureNames) {
		glm::vec2 leftBottom(
			static_cast<float>(currentTextureOffsetX) / textureWidth,
			static_cast<float>(currentTextureOffsetY - subTextureHeight) / textureHeight);
		glm::vec2 rightTop(
			static_cast<float>(currentTextureOffsetX + subTextureWidth) / textureWidth,
			static_cast<float>(currentTextureOffsetY) / textureHeight);
		pTexture->addSubTexture(currentSubTextureName, leftBottom, rightTop);

		currentTextureOffsetX += subTextureWidth;
		if (currentTextureOffsetX >= textureWidth) {
			currentTextureOffsetX = 0;
			currentTextureOffsetY -= subTextureHeight;
		}
	}

	return pTexture;
}
std::shared_ptr<RenderEngine::AnimatedSprite> ResourceManager::loadAnimatedSprite(
	const std::string& animatedSpriteName, const std::string& textureName,
	const std::string& subTextureName, const std::string& shaderProgramName,
	const unsigned int spriteWidth, const unsigned int spriteHeight) {

	auto pTexture = getTexture(textureName);
	if (pTexture == nullptr) {
		std::cerr << "Cant't find the texture: " << textureName << std::endl;
		return nullptr;
	}

	auto pShaderProgram = getShaderProgram(shaderProgramName);
	if (pShaderProgram == nullptr) {
		std::cerr << "Cant't find the shader program: "
			<< shaderProgramName << std::endl;
		return nullptr;
	}

	std::shared_ptr<RenderEngine::AnimatedSprite> newAnimatedSprite =
		animatedSprites.emplace(animatedSpriteName,
			std::make_shared<RenderEngine::AnimatedSprite>(pTexture, subTextureName,
				pShaderProgram, glm::vec2(0.0f), glm::vec2(spriteWidth, spriteHeight))
		).first->second;

	return newAnimatedSprite;
}
std::shared_ptr<RenderEngine::AnimatedSprite> ResourceManager::getAnimatedSprite(
	const std::string& animatedSpriteName) {

	AnimatedSpritesMap::const_iterator it = animatedSprites.find(animatedSpriteName);
	if (it == animatedSprites.end()) {
		std::cerr << "Cant't find the animatedSprite: " 
			<< animatedSpriteName << std::endl;
		return nullptr;
	}
	return it->second;
}
bool ResourceManager::loadJSONResources(const std::string& JSONPath) {
	const std::string JSONString = getFileString(JSONPath);
	if (JSONString.empty()) {
		std::cerr << "No JSON resources file!" << std::endl;
		return false;
	}

	rapidjson::Document document;
	rapidjson::ParseResult parseResult = document.Parse(JSONString.c_str());
	if (!parseResult) {
		std::cerr << "JSON parse error: "
			<< GetParseError_En(parseResult.Code())
			<< " (" << parseResult.Offset() << ")" << std::endl;
		std::cerr << "In JSON file: " << JSONPath << std::endl;
		return false;
	}

	// ShaderPrograms
	auto shaderProgramsIt = document.FindMember("shaderPrograms");
	if (shaderProgramsIt == document.MemberEnd()) {
		return false;
	}

	for (const auto& shaderProgram : shaderProgramsIt->value.GetArray()) {
		const std::string name = shaderProgram["name"].GetString();
		const std::string filePathV = shaderProgram["filePathV"].GetString();
		const std::string filePathF = shaderProgram["filePathF"].GetString();

		if (loadShaders(name, filePathV, filePathF) == nullptr) {
			std::cerr << "Can't create shader program: " << name << std::endl;
			return false;
		}
	}

	// TextureAtlases
	auto textureAtlasesIt = document.FindMember("textureAtlases");
	if (textureAtlasesIt == document.MemberEnd()) {
		return false;
	}

	for (const auto& textureAtlas : textureAtlasesIt->value.GetArray()) {
		const std::string name = textureAtlas["name"].GetString();
		const std::string filePath = textureAtlas["filePath"].GetString();
		const unsigned int width = textureAtlas["width"].GetInt();
		const unsigned int height = textureAtlas["height"].GetInt();
		const unsigned int subTextureWidth = 
			textureAtlas["subTextureWidth"].GetInt();
		const unsigned int subTextureHeight = 
			textureAtlas["subTextureHeight"].GetInt();
		const auto subTextureNamesArray =
			textureAtlas["subTextureNames"].GetArray();
		std::vector<std::string> subTextureNames;
		subTextureNames.reserve(subTextureNamesArray.Size());
		for (const auto& subTextureName : subTextureNamesArray) {
			subTextureNames.emplace_back(subTextureName.GetString());
		}

		if (loadTextureAtlas(name, filePath, subTextureNames,
			subTextureWidth, subTextureHeight) == nullptr) {
			std::cerr << "Can't create textureAtlas: " << name << std::endl;
			return false;
		}
	}

	// AnimatedSprites
	auto animatedSpritesIt = document.FindMember("animatedSprites");
	if (animatedSpritesIt == document.MemberEnd()) {
		return false;
	}

	for (const auto& animatedSprite : animatedSpritesIt->value.GetArray()) {
		const std::string name = animatedSprite["name"].GetString();
		const std::string textureAtlas = animatedSprite["textureAtlas"].GetString();
		const std::string defaultTexture = animatedSprite["defaultTexture"].GetString();
		const std::string shaderProgram = animatedSprite["shaderProgram"].GetString();
		const unsigned int width = animatedSprite["width"].GetInt();
		const unsigned int height = animatedSprite["height"].GetInt();
		const auto statesArray = animatedSprite["states"].GetArray();

		auto pAnimatedSprite = loadAnimatedSprite(name, textureAtlas,
			defaultTexture, shaderProgram, width, height);
		if (pAnimatedSprite == nullptr) {
			std::cerr << "Can't create animatedSprite: " << name << std::endl;
			return false;
		}

		for (const auto& state : statesArray) {
			const std::string stateName = state["stateName"].GetString();
			const auto& framesArray = state["frames"].GetArray();
			std::vector<std::pair<std::string, uint64_t>> frames;
			frames.reserve(framesArray.Size());
			for (const auto& frame : framesArray) {
				const std::string subTexture = frame["subTexture"].GetString();
				const uint64_t duration = frame["duration"].GetUint64();
				frames.emplace_back(subTexture, duration);
			}
			pAnimatedSprite->insertState(stateName, frames);
		}
	}

	return true;
}