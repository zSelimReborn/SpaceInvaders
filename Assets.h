#pragma once

#include <string>

namespace Assets
{
	const std::string BasePath = "Assets/";

	namespace Config
	{
		const std::string InnerPath = "Config/";

		const std::string WindowFile = BasePath + InnerPath + "window.txt";
		const std::string GameFile = BasePath + InnerPath + "game.txt";
		const std::string PlayerFile = BasePath + InnerPath + "player.txt";

		const std::string AlienGroupFile = BasePath + InnerPath + "alien_group.txt";
		const std::string SquidFile = BasePath + InnerPath + "squid.txt";
		const std::string CrabFile = BasePath + InnerPath + "crab.txt";
		const std::string OctopusFile = BasePath + InnerPath + "octopus.txt";
		const std::string BonusAlienFile = BasePath + InnerPath + "bonus.txt";
		const std::string SecretAlienFile = BasePath + InnerPath + "a_secret.txt";
		const std::string BunkerFile = BasePath + InnerPath + "bunker.txt";

		const std::string PlayerProjectilePool = BasePath + InnerPath + "player_projectile.txt";
		const std::string AlienProjectilePool = BasePath + InnerPath + "alien_projectile.txt";
	}

	namespace Shaders
	{
		const std::string InnerPath = "Shaders/";

		const std::string ShapeName			= "shader_shape";
		const std::string ShapeVertexFile	= BasePath + InnerPath + "shape.vert";
		const std::string ShapeFragmentFile = BasePath + InnerPath + "shape.frag";

		const std::string TextName = "shader_text";
		const std::string TextVertexFile = BasePath + InnerPath + "text.vert";
		const std::string TextFragmentFile = BasePath + InnerPath + "text.frag";

		const std::string ParticleShapeName = "shader_particle_shape";
		const std::string ParticleTextureName = "shader_particle_shape";
		const std::string ParticleVertexFile = BasePath + InnerPath + "particle.vert";

		const std::string ParticleShapeFragmentFile = BasePath + InnerPath + "particle_shape.frag";
		const std::string ParticleTextureFragmentFile = BasePath + InnerPath + "particle_texture.frag";
	}

	namespace Fonts
	{
		const std::string InnerPath = "Fonts/";

		const std::string TextFontName = "JerseyText";
		const std::string TextFontPath = BasePath + InnerPath + "Jersey10-Regular.ttf";

		const std::string HeadingFontName = "JerseyHeading";
		const std::string HeadingFontPath = BasePath + InnerPath + "Jersey10-Regular.ttf";
	}
}