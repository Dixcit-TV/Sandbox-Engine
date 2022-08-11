#include "pch.h"
#include "ResourceManager.h"

SDBX::Resource::ResourceManager::~ResourceManager()
{
	for (auto& loaderPair : m_Loaders)
		delete loaderPair.second;
}

void SDBX::Resource::ResourceManager::Init(const std::wstring& dataPath)
{
	m_DataPath = dataPath;
}

void SDBX::Resource::ResourceManager::RegisterLoader(ILoader* resourceLoader)
{
	const std::string typeName{ resourceLoader->GetType().name() };

	[[maybe_unused]] bool added{ m_Loaders.try_emplace(typeName, resourceLoader).second };

	SDBX_ASSERT_AS_WARNING(added, "could not add loader for type " + typeName + ", it might already have been registered.");
}