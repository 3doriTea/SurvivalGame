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
		virtual void OnBegin(const std::function<void(EventSubject<EntityRegistry&>&)>& _callback) = 0;
		/// <summary>
		/// 描画時の処理を登録する
		/// </summary>
		/// <param name="_callback">コールバック関数 void(EventSubject<>&)</param>
		virtual void OnRender(const std::function<void(EventSubject<EntityRegistry&>&)>& _callback) = 0;
		/// <summary>
		/// ゲーム画面描画後の処理を登録する
		/// </summary>
		/// <param name="_callback">コールバック関数 void(EventSubject<>&)</param>
		virtual void OnReleasedTarget(const std::function<void(EventSubject<EntityRegistry&>&)>& _callback) = 0;
		/// <summary>
		/// 遅らせた描画時の処理を登録する
		/// </summary>
		/// <param name="_callback">コールバック関数 void(EventSubject<>&)</param>
		virtual void OnRenderLate(const std::function<void(EventSubject<EntityRegistry&>&)>& _callback) = 0;
		/// <summary>
		/// 描画した後の処理を登録する
		/// </summary>
		/// <param name="_callback">コールバック関数 void(EventSubject<>&)</param>
		virtual void OnEnd(const std::function<void(EventSubject<EntityRegistry&>&)>& _callback) = 0;
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
		void Update(EntityRegistry&) override;

		/// <summary>
		/// 終了処理
		/// </summary>
		void Release() override;

		void OnBegin(const std::function<void(EventSubject<EntityRegistry&>&)>& _callback) override;
		void OnRender(const std::function<void(EventSubject<EntityRegistry&>&)>& _callback) override;
		void OnReleasedTarget(const std::function<void(EventSubject<EntityRegistry&>&)>& _callback) override;
		void OnRenderLate(const std::function<void(EventSubject<EntityRegistry&>&)>& _callback) override;
		void OnEnd(const std::function<void(EventSubject<EntityRegistry&>&)>& _callback) override;
		void Enqueue(RenderItem&& _renderItem) override;

	private:
		EventSubject<EntityRegistry&> beginEvent_;
		EventSubject<EntityRegistry&> renderEvent_;
		EventSubject<EntityRegistry&> releasedTargetEvent_;
		EventSubject<EntityRegistry&> renderLateEvent_;
		EventSubject<EntityRegistry&> endEvent_;
		std::vector<RenderItem> renderQueue_;  // 描画キュー
	};
}
