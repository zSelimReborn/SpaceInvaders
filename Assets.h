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
		const std::string SecretAlienFile = BasePath + InnerPath + "a_secret.txt";

		const std::string PlayerProjectilePool = BasePath + InnerPath + "player_projectile.txt";
		const std::string AlienProjectilePool = BasePath + InnerPath + "alien_projectile.txt";
	}

	namespace Shaders
	{
		const std::string InnerPath = "Shaders/";

		const std::string ShapeName			= "shader_shape";
		const std::string ShapeVertexFile	= BasePath + InnerPath + "shape.vert";
		const std::string ShapeFragmentFile = BasePath + InnerPath + "shape.frag";
	}
}