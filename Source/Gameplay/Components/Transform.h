#pragma once
#include "Core/Maths/Vec.h"

namespace SDBX
{
	class Transform final
	{
	public:
		explicit Transform() : Transform(Vector::Vec3f{}) {}
		explicit Transform(const Vector::Vec3f& position, const Vector::Vec3f& scale = Vector::Vec3f{1.f}, const Vector::Vec3f& rotation = Vector::Vec3f{})
			: m_Position(position), m_Scale(scale), m_Rotation(rotation) {}

		const Vector::Vec3f& GetPosition() const { return m_Position; }
		void SetPosition(float x, float y, float z) { m_Position = Vector::Vec3f{ x, y, z }; }
		void SetPosition(const Vector::Vec3f& position) { SetPosition(position.x, position.y, position.z); };

		const Vector::Vec3f& GetScale() const { return m_Scale; }
		void SetScale(float x, float y, float z) { m_Scale = Vector::Vec3f{ x, y, z }; }
		void SetScale(const Vector::Vec3f& scale) { SetScale(scale.x, scale.y, scale.z); };

		const Vector::Vec3f& GetRotation() const { return m_Rotation; }
		void SetRotation(float x, float y, float z) { m_Rotation = Vector::Vec3f{ x, y, z }; }
		void SetRotation(const Vector::Vec3f& rotation) { SetRotation(rotation.x, rotation.y, rotation.z); };
	
	private:
		Vector::Vec3f m_Position;
		Vector::Vec3f m_Scale;
		Vector::Vec3f m_Rotation;
	};
}
