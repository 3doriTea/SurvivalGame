#pragma once
#include "../SystemBase.h"
#include <Component/Transform2D.h>


namespace GameBase::System
{
	/// <summary>
	/// システムインタフェース: 
	/// </summary>
	class ITransform2DCalculator : public ISystemInterfaceBase
	{
	public:
		ITransform2DCalculator() = default;
		virtual ~ITransform2DCalculator() = default;

		/// <summary>
		/// 親子関係を考慮した計算順番を参照する
		/// </summary>
		/// <returns>entities</returns>
		virtual const std::vector<Entity>& CalculationQueue() = 0;
	};

	/// <summary>
	/// システム: 
	/// </summary>
	class Transform2DCalculator : public SystemBase<Transform2DCalculator, ITransform2DCalculator>
	{
	public:
		Transform2DCalculator();
		~Transform2DCalculator();

		// ワールド実行情報を参照、このシステムを登録除外するかチェックする true を返すことで除外される
		//bool OnVerifyShouldSkip(const RunInfo& _info) override;

		// 他システムへの依存関係を登録する cppでincludeした他システムはここで登録
		void OnRegisterDependencies(FluentVectorAddOnly<SystemIndex>* _registry) override;

		/// <summary>
		/// 初期化処理
		/// </summary>
		void Initialize() override;

		/// <summary>
		/// 更新処理
		/// </summary>
		void Update(EntityRegistry&) override;

		/// <summary>
		/// 終了処理
		/// </summary>
		void Release() override;

		inline const std::vector<Entity>& CalculationQueue() override { return calculationQueue_; }

	private:
		void Calculate(View<Component::Transform2D>& _view, const Entity _entity, Component::Transform2D& _transform);

	private:
		std::unordered_set<Entity> checked_;
		std::vector<Entity> calculationQueue_;
	};
}
