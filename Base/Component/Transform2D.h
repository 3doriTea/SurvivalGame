#pragma once
#include "../ComponentBase.h"


namespace GameBase::System
{
	class Transform2DCalculator;
}

namespace GameBase::Component
{
	/// <summary>
	/// コンポーネント : 
	/// </summary>
	struct Transform2D : ComponentBase<Transform2D>
	{
		friend class GameBase::System::Transform2DCalculator;

		Transform2D();
		void OnLoad(const YAML::Node& _node, const SchemaLoadBundle& _bundle) override;
		void OnSave(YAML::Emitter& _emitter, const SchemaLoadBundle& _bundle) override;

		/// <summary>
		/// ワールド行列を取得する
		/// </summary>
		/// <returns>ワールド行列</returns>
		inline Mat4x4 GetWorldMatrix() const { return worldMatrix; }

		Entity parent;     // 親
		Vec2 position;     // ローカル座標
		// 0.0 .. TAU
		float angle;       // xyに垂直な軸での回転
		Vec2 scale;        // xy平面への大きさ
		// 0.0 .. 1.0
		Vec2 anchorPoint;  // 画面全体のどこを基準に座標を決めるか
		// 0.0 .. 1.0
		Vec2 pivotPoint;   // どのローカル座標を基準に座標を決めるか

	private:
		Mat4x4 localMatrix;  // 自分自身の変換
		Mat4x4 worldMatrix;  // ワールド変換
		Mat4x4 toChildMatrix;  // 親からの変換
	};
}
