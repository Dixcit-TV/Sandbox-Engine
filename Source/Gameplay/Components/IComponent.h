#pragma once
#include "Gameplay/GameObject.h"

namespace SDBX {
	class IComponent
	{
	public:
		virtual ~IComponent() = default;

		virtual void Update() {}
		virtual void Render() const {}

		GameObject* GetGameObject() const { return m_pGameObject; }

	protected:		
		explicit IComponent() = default;
		IComponent(const IComponent& rhs) = default;
		IComponent(IComponent&& rhs) noexcept = default;
		IComponent& operator=(const IComponent& rhs) = default;
		IComponent& operator=(IComponent&& rhs) noexcept = default;

		GameObject* m_pGameObject;

		//Transform& GetTransform() { return m_pGameObject->GetTransform(); }
		//const Transform& GetTransform() const { return m_pGameObject->GetTransform(); }

		template<typename ComponentType, typename = std::enable_if_t<std::is_base_of_v<IComponent, ComponentType>>>
		ComponentType* GetComponent() const { return m_pGameObject->GetComponent<ComponentType>(); }

	private:
		friend class GameObject;
		
		void SetParentGo(GameObject* pParent) { m_pGameObject = pParent; }
	};
}

