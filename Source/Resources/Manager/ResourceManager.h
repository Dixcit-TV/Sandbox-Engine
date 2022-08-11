#pragma once
#include <string>
#include <unordered_map>
#include <iostream>

#include "Core/Base/Singleton.h"
#include "Core/Log/Logger.h"
#include "Resources/Loaders/ILoader.h"

namespace SDBX
{
	namespace Resource
	{
		class IResource;

		class ResourceManager final : public Singleton<ResourceManager>
		{
		public:
			~ResourceManager() override;
			ResourceManager(const ResourceManager& other) = delete;
			ResourceManager(ResourceManager&& other) = delete;
			ResourceManager& operator=(const ResourceManager& other) = delete;
			ResourceManager& operator=(ResourceManager&& other) = delete;

			void Init(const std::wstring& data);
			void RegisterLoader(ILoader* resourceLoader);

			template<typename ResourceType, typename... ArgType>
			ResourceType* LoadResource(const std::wstring&, ArgType&&...);

		private:
			friend class Singleton<ResourceManager>;
			ResourceManager() = default;
			std::wstring m_DataPath;
			std::unordered_map<std::string, ILoader*> m_Loaders;

			std::unordered_map<std::string, IResource*> m_pResource;
		};

		template<typename ResourceType, typename... ArgType>
		ResourceType* ResourceManager::LoadResource(const std::wstring& file, ArgType&&... args)
		{
			const std::string typeName{ typeid(ResourceType).name() };
			auto loaderIt{ m_Loaders.find(typeName) };

			if (loaderIt != std::end(m_Loaders))
			{
				auto* loader{ static_cast<IBaseLoader<ResourceType, ArgType...>*>(loaderIt->second) };
				const std::string resourceID{ loader->GenerateResourceID(file, std::forward<ArgType>(args)...) };

				auto resourceIt{ m_pResource.find(resourceID) };
				if (resourceIt != m_pResource.end())
					return static_cast<ResourceType*>(resourceIt->second);

				auto newResource{ loader->LoadContent(m_DataPath + file, std::forward<ArgType>(args)...) };
				m_pResource.emplace(resourceID, newResource);
				return newResource;
			}

			SDBX_W_LOG(WARNING_LOG, L"Could not load resource of type " + typeName + " , no compatible registered Loader found !")
			return nullptr;
		}
	}
}
