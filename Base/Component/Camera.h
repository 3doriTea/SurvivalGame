#pragma once
#include "../ComponentBase.h"


namespace GameBase::Component
{
	/// <summary>
	/// コンポーネント : カメラ
	/// </summary>
	struct Camera : ComponentBase<Camera>
	{
		Camera();
		void OnLoad(const YAML::Node& _node) override;
		void OnSave(YAML::Emitter& _emitter) override;

		float nearZ;        // ファークリップ平面の距離
		float farZ;         // ニヤークリップ平面の距離
		float fovRadian;    // 視野角
	private:
		float aspectRatio;        // アスペクト比
		Mat4x4 viewMatrix;        // ビュー行列
		Mat4x4 projectionMatrix;  // プロジェクション行列
	};
}
