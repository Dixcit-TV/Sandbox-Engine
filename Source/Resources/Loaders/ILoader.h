#pragma once
#include <string>

namespace SDBX
{
	namespace Resource
	{
		class ILoader
		{
		public:
			ILoader(const ILoader&) = delete;
			ILoader(ILoader&&) noexcept = delete;
			ILoader& operator=(const ILoader&) = delete;
			ILoader& operator=(ILoader&&) noexcept = delete;
			virtual ~ILoader() = default;

			virtual const type_info& GetType() const = 0;

		protected:
			explicit ILoader() = default;
		};

		template<typename ResourceType, typename... ArgType>
		class IBaseLoader : public ILoader
		{
		public:
			IBaseLoader(const IBaseLoader&) = delete;
			IBaseLoader(IBaseLoader&&) noexcept = delete;
			IBaseLoader& operator=(const IBaseLoader&) = delete;
			IBaseLoader& operator=(IBaseLoader&&) noexcept = delete;
			virtual ~IBaseLoader() = default;

			virtual const type_info& GetType() const override { return typeid(ResourceType); }

			virtual std::string GenerateResourceID(const std::wstring& path, ArgType&&...) const { return path; }
			virtual ResourceType* LoadContent(const std::wstring& path, ArgType&&...) const = 0;

		protected:
			explicit IBaseLoader() = default;
		};
	}
}

