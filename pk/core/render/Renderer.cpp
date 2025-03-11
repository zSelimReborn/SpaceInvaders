#include "Renderer.h"

#include <glad/glad.h>

#include "../vfx/Emitter.h"
#include "../asset/Font.h"
#include "../asset/Shader.h"
#include "../asset/Texture.h"

using namespace pk;

Renderer::Renderer()
	: SpriteQuadId(-1), TextQuadId(-1), TextBufferId(-1), LastShaderId(-1)
{
	InitializeSpriteQuad();
	InitializeTextQuad();
}

void Renderer::InitializeSpriteQuad()
{
	float VertexData[] = {
	 0.5f,  0.5f, 1.f, 1.f,
	 0.5f, -0.5f, 1.f, 0.f,
	-0.5f, -0.5f, 0.f, 0.f,
	-0.5f,  0.5f, 0.f, 1.f
	};

	unsigned int Indices[] = {  
		0, 1, 3,
		1, 2, 3 
	};

	unsigned int VAO = 0;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	SpriteQuadId = VAO;
}

void Renderer::InitializeTextQuad()
{
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	TextQuadId = VAO;
	TextBufferId = VBO;
}

void Renderer::RenderSprite(const ShaderPtr& Shader, const TexturePtr& Texture, const glm::mat4& Model, const glm::vec3& Color)
{
	if (Shader == nullptr)
	{
		return;
	}

	glBindVertexArray(SpriteQuadId);

	if (LastShaderId != Shader->GetShaderId())
	{
		LastShaderId = Shader->GetShaderId();
		Shader->Use();
	}

	Shader->SetFloat("spriteColor", Color);
	Shader->SetMatrix("model", Model);

	if (Texture != nullptr)
	{
		glActiveTexture(GL_TEXTURE0);
		Texture->Bind();
	}

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	if (Texture != nullptr)
	{
		Texture->UnBind();
	}
}

void Renderer::RenderParticleVfx(const ParticleList& Particles, 
                                 const ShaderPtr& Shader, const TexturePtr& Texture, float Scale)
{
	if (Shader == nullptr)
	{
		return;
	}

	if (LastShaderId != Shader->GetShaderId())
	{
		LastShaderId = Shader->GetShaderId();
		Shader->Use();
	}

	glBindVertexArray(SpriteQuadId);

	if (Texture != nullptr)
	{
		glActiveTexture(GL_TEXTURE0);
		Texture->Bind();
	}

	for (const Particle::SharedPtr& CurrentParticle : Particles)
	{
		if (CurrentParticle->Life <= 0.f)
		{
			continue;
		}

		float CurrentScale = Scale;
		if (CurrentParticle->OverrideScale >= 0.f)
		{
			CurrentScale = CurrentParticle->OverrideScale;
		}

		Shader->SetFloat("position", CurrentParticle->Position);
		Shader->SetFloat("color", CurrentParticle->Color);
		Shader->SetFloat("scale", CurrentScale);

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	if (Texture != nullptr)
	{
		Texture->UnBind();
	}

	glBindVertexArray(0);
}

void Renderer::RenderText(const std::string& Text, const ShaderPtr& Shader, const CharacterMap& Characters, 
	const glm::vec2& Position, float Scale, const glm::vec4& Color)
{
	if (Shader == nullptr)
	{
		return;
	}

	if (LastShaderId != Shader->GetShaderId())
	{
		LastShaderId = Shader->GetShaderId();
		Shader->Use();
	}

	Shader->SetColor("textColor", Color);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(TextQuadId);

	// iterate through all characters
	float x = Position.x;
	float y = Position.y;

	Character MaxChar = Characters.at('H');

	for (const char c : Text)
	{
		Character Glyph = Characters.at(c);

		float xpos = x + Glyph.Bearing.x * Scale;
		float ypos = y + (MaxChar.Bearing.y - Glyph.Bearing.y) * Scale;

		float w = Glyph.Size.x * Scale;
		float h = Glyph.Size.y * Scale;
		// update VBO for each character
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 0.0f },

			{ xpos,     ypos + h,   0.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 0.0f }
		};
		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, Glyph.TextureID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, TextBufferId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);

		x += (Glyph.Advance >> 6) * Scale;
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

