#pragma once

#include <string>

namespace Assets
{
	const std::string BasePath = "Assets/";

	namespace Shaders
	{
		const std::string InnerPath = "Shaders/";

		const std::string ShapeName			= "shader_shape";
		const std::string ShapeVertexFile	= BasePath + InnerPath + "shape.vert";
		const std::string ShapeFragmentFile = BasePath + InnerPath + "shape.frag";
	}
}