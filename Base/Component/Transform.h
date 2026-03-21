#pragma once
#include "../ComponentBase.h"
#include "../ECS/Entity.h"

namespace GameBase::System
{
	class TransformCalculator;
}

namespace GameBase::Component
{

	/// <summary>
	/// コンポーネント : 座標系
	/// </summary>
	struct Transform : ComponentBase<Transform>
	{
		friend class GameBase::System::TransformCalculator;

		Transform();
		void OnLoad(const YAML::Node& _node, const SchemaLoadBundle& _bundle) override;
		void OnSave(YAML::Emitter& _emitter, const SchemaLoadBundle& _bundle) override;

		inline Vec3 Forward() const { return Vec3::Forward() * localRotationMatrix; }
		inline Vec3 Up() const { return Vec3::Up() * localRotationMatrix; }
		inline Vec3 WorldPosition() const { return { worldMatrix.r[3].m128_f32[Vec3::AT_X], worldMatrix.r[3].m128_f32[Vec3::AT_Y], worldMatrix.r[3].m128_f32[Vec3::AT_Z] }; }

		Vec3 position;
		Quaternion rotation;
		Vec3 scale;
		Entity parent;
	private:
		Mat4x4 localTranslateMatrix;
		Mat4x4 localRotationMatrix;
		Mat4x4 localScaleMatrix;
		Mat4x4 worldMatrix;
	};
}
