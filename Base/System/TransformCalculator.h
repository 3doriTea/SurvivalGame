#pragma once
#include "../SystemBase.h"
#include <Component/Transform.h>


namespace GameBase::System
{
	/// <summary>
	/// システムインタフェース: 
	/// </summary>
	class ITransformCalculator : public ISystemInterfaceBase
	{
	public:
		ITransformCalculator() = default;
		virtual ~ITransformCalculator() = default;

		/// <summary>
		/// 親子関係を考慮した計算順番を参照する
		/// </summary>
		/// <returns>entities</returns>
		virtual const std::vector<Entity>& CalculationQueue() = 0;
	};

	/// <summary>
	/// システム: 
	/// </summary>
	class TransformCalculator : public SystemBase<TransformCalculator, ITransformCalculator>
	{
	public:
		TransformCalculator();
		~TransformCalculator();

		// 利用する参照があるときに使用
		//void OnRegisterDependencies(FluentVectorAddOnly<SystemIndex>* _registry) override;

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
		void Calculate(View<Component::Transform>& _view, const Entity _entity, Component::Transform& _transform);

	private:
		std::unordered_set<Entity> checked_;
		std::vector<Entity> calculationQueue_;
	};
}
