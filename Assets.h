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
		const std::string PlayerProjectilePool = BasePath + InnerPath + "player_projectile.txt";
	}

	namespace Shaders
	{
		const std::string InnerPath = "Shaders/";

		const std::string ShapeName			= "shader_shape";
		const std::string ShapeVertexFile	= BasePath + InnerPath + "shape.vert";
		const std::string ShapeFragmentFile = BasePath + InnerPath + "shape.frag";
	}
}