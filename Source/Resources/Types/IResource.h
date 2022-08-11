#pragma once
#include <string>

namespace SDBX
{
	namespace Resource
	{
		class IResource
		{
		public:
			virtual ~IResource() = default;
			IResource(const IResource&) = delete;
			IResource(IResource&&) noexcept = delete;
			IResource& operator=(const IResource&) = delete;
			IResource& operator=(IResource&&) noexcept = delete;

		protected:
			explicit IResource() = default;
		};
	}
}

