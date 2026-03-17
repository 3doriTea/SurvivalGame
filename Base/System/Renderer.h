#pragma once
#include "../SystemBase.h"
#include "Structure/RenderItem.h"


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

		/// <summary>
		/// 描画する前の処理を登録する
		/// </summary>
		/// <param name="_callback">コールバック関数 void(EventSubject<>&)</param>
		virtual void OnBegin(const std::function<void(EventSubject<>&)>& _callback) = 0;
		/// <summary>
		/// 描画時の処理を登録する
		/// </summary>
		/// <param name="_callback">コールバック関数 void(EventSubject<>&)</param>
		virtual void OnRender(const std::function<void(EventSubject<>&)>& _callback) = 0;
		/// <summary>
		/// 描画した後の処理を登録する
		/// </summary>
		/// <param name="_callback">コールバック関数 void(EventSubject<>&)</param>
		virtual void OnEnd(const std::function<void(EventSubject<>&)>& _callback) = 0;
		/// <summary>
		/// 描画キューに追加する
		/// </summary>
		/// <param name="_renderItem">描画アイテム</param>
		virtual void Enqueue(RenderItem&& _renderItem) = 0;
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

		void OnBegin(const std::function<void(EventSubject<>&)>& _callback) override;
		void OnRender(const std::function<void(EventSubject<>&)>& _callback) override;
		void OnEnd(const std::function<void(EventSubject<>&)>& _callback) override;
		void Enqueue(RenderItem&& _renderItem) override;

	private:
		EventSubject<> beginEvent_;
		EventSubject<> endEvent_;
		EventSubject<> renderEvent_;
		std::vector<RenderItem> renderQueue_;  // 描画キュー
	};
}
