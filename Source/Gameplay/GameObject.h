#pragma once
#include <vector>
#include <string>

#include "Gameplay/Components/Transform.h"

namespace SDBX
{
	//class Scene;
	class IComponent;

	class GameObject final
	{
		//friend class Scene;
	public:

		using Components = std::vector<IComponent*>;
		
		explicit GameObject(const Transform& transform = Transform(), const std::string& name = "GameObject", const std::string& tag = "")
			: m_ComponentPtrs{ }, m_Transform{ transform }, m_Name{ name }, m_Tag{ tag }/*, m_pParentScene{}*/, m_IsEnabled{ true } {}
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		template<typename ComponentType, typename... ARG_TYPE, typename = std::enable_if_t<std::is_base_of_v<IComponent, ComponentType>>>
		ComponentType* AddComponent(ARG_TYPE&&... arguments);

		template<typename ComponentType, typename = std::enable_if_t<std::is_base_of_v<IComponent, ComponentType>>>
		ComponentType* GetComponent() const;

		const Components& GetAllComponents() const { return m_ComponentPtrs; }
		Components& GetAllComponents() { return m_ComponentPtrs; }

		const Transform& GetTransform() const { return m_Transform; }
		Transform& GetTransform() { return m_Transform; }

		//std::shared_ptr<GameObject> GetGameObjectWithName(const std::string& name) const { return m_pParentScene.lock()->GetGameObjectWithName(name); }
		//std::vector<std::shared_ptr<GameObject>> GetAllGameObjectsWithName(const std::string& name) const { return m_pParentScene.lock()->GetAllGameObjectsWithName(name); }
		//std::shared_ptr<GameObject> GetGameObjectWithTag(const std::string& tag) const { return m_pParentScene.lock()->GetGameObjectWithTag(tag); }
		//std::vector<std::shared_ptr<GameObject>> GetAllGameObjectsWithTag(const std::string& tag) const { return m_pParentScene.lock()->GetAllGameObjectsWithTag(tag); }

		//std::shared_ptr<Scene> GetScene() const { return m_pParentScene.lock(); }

		const std::string& GetName() const { return m_Name; }
		const std::string& GetTag() const { return m_Tag; }
		void SetTag(const std::string& newTag) { m_Tag = newTag; }

		void SetEnable(bool enable) { m_IsEnabled = enable; }
		bool IsEnabled() const { return m_IsEnabled; }

		//void Delete() { m_pParentScene.lock()->Remove(shared_from_this()); }
	
	private:
		Components m_ComponentPtrs;
		Transform m_Transform;
		std::string m_Name;
		std::string m_Tag;
		//std::weak_ptr<Scene> m_pParentScene;
		bool m_IsEnabled;
	};

	template<typename ComponentType, typename... ARG_TYPE, typename>
	ComponentType* GameObject::AddComponent(ARG_TYPE&&... arguments)
	{
		ComponentType* newComp{ new ComponentType(std::forward<ARG_TYPE>(arguments)...) };
		newComp->SetParentGo(this);
		m_ComponentPtrs.push_back(newComp);
		return newComp;
	}

	template<typename ComponentType, typename>
	ComponentType* GameObject::GetComponent() const
	{
		auto it{ std::find_if(std::cbegin(m_ComponentPtrs), std::cend(m_ComponentPtrs), [](const Components::value_type& c) { return dynamic_cast<ComponentType>(c); }) };

		return it != std::cend(m_ComponentPtrs) ? static_cast<ComponentType>(*it) : nullptr;
	}
}
