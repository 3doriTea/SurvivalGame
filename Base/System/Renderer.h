#pragma once
#include "../SystemBase.h"


namespace GameBase::System
{
	/// <summary>
	/// システムインタフェース: 描画
	/// </summary>
	class IRenderer : public ISystemInterfaceBase
	{
	public:
		IRenderer() = default;
		virtual ~IRenderer() = default;

		virtual void OnBegin(const std::function<void(Event<>&)> _callback) = 0;
		virtual void OnEnd(const std::function<void(Event<>&)> _callback) = 0;
	};

	/// <summary>
	/// システム: 描画
	/// </summary>
	class Renderer : public SystemBase<Renderer, IRenderer>
	{
	public:
		Renderer();
		~Renderer();

		// 利用する参照があるときに使用
		void OnRegisterDependencies(FluentVectorAddOnly<SystemIndex>* _registry) override;

		/// <summary>
		/// 初期化処理
		/// </summary>
		void Initialize() override;

		/// <summary>
		/// 更新処理
		/// </summary>
		void Update() override;

		/// <summary>
		/// 終了処理
		/// </summary>
		void Release() override;

		void OnBegin(const std::function<void(Event<>&)> _callback) override;
		void OnEnd(const std::function<void(Event<>&)> _callback) override;

	private:
		Event<> beginEvent_;
		Event<> endEvent_;

	};
}
