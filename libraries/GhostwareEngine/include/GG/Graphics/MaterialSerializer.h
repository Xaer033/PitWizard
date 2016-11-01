// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include <GG/Core/ISerializer.h>
#include <GG/Core/Json.h>
#include "Material.h"
#include "RenderState.h"

namespace GG
{
	class MaterialSerializer //: public ISerializer<Material>
	{
	public:
		static int		VERSION;

		static bool	Serialize(const Material & material, std::string & jsonString);
		static bool	Deserialize(Material & material, const std::string & jsonString);

	};
}
