#pragma once

#include <map>
#include <vector>
#include <memory>
#include <string>
#include <glm/glm.hpp>


namespace pk
{
	class Shader;
	class Texture;
	struct Particle;
	struct Character;

	class Renderer
	{
	public:
		typedef std::shared_ptr<Shader> ShaderPtr;
		typedef std::shared_ptr<Texture> TexturePtr;
		typedef std::vector<std::shared_ptr<Particle>> ParticleList;
		typedef std::map<char, Character> CharacterMap;

		static Renderer& Get()
		{
			static Renderer Instance;
			return Instance;
		}

		void RenderSprite(const ShaderPtr& Shader, const TexturePtr& Texture, const glm::mat4& Model, const glm::vec3& Color);
		void RenderParticleVfx(const ParticleList& Particles, const ShaderPtr& Shader, const TexturePtr& Texture, float Scale);
		void RenderText(const std::string& Text, const ShaderPtr& Shader, const CharacterMap& Characters, const glm::vec2& Position, float Scale, const glm::vec4& Color);

	private:
		Renderer();
		void InitializeSpriteQuad();
		void InitializeTextQuad();

		unsigned int SpriteQuadId;
		unsigned int TextQuadId;
		unsigned int TextBufferId;
		unsigned int LastShaderId;
	};
}