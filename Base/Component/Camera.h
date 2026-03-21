#pragma once
#include "../ComponentBase.h"


namespace GameBase::System
{
	class MainCamera;
}

namespace GameBase::Component
{
	/// <summary>
	/// コンポーネント : カメラ
	/// </summary>
	struct Camera : ComponentBase<Camera>
	{
		friend GameBase::System::MainCamera;

		Camera();
		void OnLoad(const YAML::Node& _node, const SchemaLoadBundle& _bundle) override;
		void OnSave(YAML::Emitter& _emitter, const SchemaLoadBundle& _bundle) override;

		float nearZ;        // ファークリップ平面の距離
		float farZ;         // ニヤークリップ平面の距離
		float fovRadian;    // 視野角
	private:
		float aspectRatio;        // アスペクト比
		Mat4x4 viewMatrix;        // ビュー行列
		Mat4x4 projectionMatrix;  // プロジェクション行列
	};
}
