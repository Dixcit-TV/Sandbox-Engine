#pragma once
#include <string>

namespace SDBX
{
	namespace Renderer
	{
		struct DrawCommand
		{
			std::string vShader;
			std::string hShader;
			std::string dShader;
			std::string gShader;
			std::string pShader;
		};

		struct DispatchCommand
		{
			std::string cShader;
			uint32_t dipatchSize[3];
		};
	}
}
