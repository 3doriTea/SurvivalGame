#pragma once
#include "ECS/Entity.h"
#include "ECS/EntityRegistry.h"
#include "SystemBase.h"


namespace GameBase
{
	using ScriptIndex = size_t;

	class IScriptBase;
	template<typename T, typename DataT>
	class ScriptBase;
	struct IComponentBase;
	class ISystemBase;

	class EntityRegistry;

	namespace ScriptRegistry
	{
		/// <summary>
		/// スクリプトのインデクスカウンタ
		/// </summary>
		/// <returns></returns>
		inline ScriptIndex& IndexCounter()
		{
			static GameBase::ScriptIndex indexCounter_;  // 型Idを増やすやつ
		}
		/// <summary>
		/// システムのインタフェース格納
		/// </summary>
		inline std::vector<std::weak_ptr<void>>& PScripts()
		{
			static std::vector<std::weak_ptr<void>> pSystems_;
		}
		/// <summary>
		/// コンストラクタ実行後に呼び出される処理
		/// </summary>
		inline std::queue<std::function<void()>>& RegisterQueue()
		{
			static std::queue<std::function<void()>> registerQueue_;
		}

		/// <summary>
		/// システムインデクスを取得する
		/// </summary>
		/// <typeparam name="T">システムの型</typeparam>
		/// <returns>インデクス</returns>
		template<typename T>
		ScriptIndex GetScriptIndex();

		/// <summary>
		/// スクリプトを追加する
		/// </summary>
		/// <typeparam name="T">システムの型</typeparam>
		/// <param name="_p"></param>
		/// <returns></returns>
		template<typename T>
		size_t Add(std::weak_ptr<IScriptBase> _p);
	}

	namespace Script
	{
		/// <summary>
		/// スクリプトの参照をするゲッタ
		/// </summary>
		/// <typeparam name="T">スクリプト型</typeparam>
		/// <returns></returns>
		template<typename T>
		T& Get();
	}

	class IScriptBase
	{
	public:
		inline IScriptBase() = default;
		inline virtual ~IScriptBase() = default;

		virtual void Start() = 0;
		virtual void Update() = 0;
	};
#define GB_LOG_COMPILE(msg) __pragma(message(__FILE__ "(" _CRT_STRINGIZE(__LINE__) ") : [Compile Log] " msg))
	/// <summary>
	/// スクリプトのベースクラス
	/// </summary>
	/// <typeparam name="T">派生構造体</typeparam>
	/// <typeparam name="DataT">データ構造体</typeparam>
	template<typename T, typename DataT>
	class ScriptBase : public IScriptBase, protected DataT
	{
		/* TODO static_assert(sizeof(T) == sizeof(DataT) + 8,
			"スクリプトにメンバ変数を書くことはできません。"
			"データ構造体に分けてください。");*/
	public:
		ScriptBase();
		inline virtual ~ScriptBase() = default;

		virtual void Start() override {}
		virtual void Update() override {}

	protected:
		template<typename ComponentT>
		ComponentT& Get() requires std::derived_from<ComponentT, IComponentBase>
		{
			GB_ASSERT(pRegistry_ && "レジストリが未指定");
			if (pRegistry_)
			{
				return pRegistry_->GetComponent<ComponentT>(self_);
			}
			else
			{
				return EntityRegistry::GetComponentNull<ComponentT>();
			}
		}

		template<typename SystemT>
		inline SystemT& Get() requires std::derived_from<SystemT, ISystemBase>
		{
			return GameBase::System::Get<SystemT>();
		}

	private:
		/// <summary>
		/// 内部で開始時に呼ばれる処理
		/// </summary>
		/// <param name="_pRegistry">レジストリのポインタ</param>
		/// <param name="_self">自分自身のエンティティ</param>
		void InnerStart(EntityRegistry* _pRegistry, const Entity _self);
		/// <summary>
		/// 内部の更新時に呼ばれる処理
		/// </summary>
		/// <param name="_pRegistry">レジストリのポインタ</param>
		/// <param name="_self">自分自身のエンティティ</param>
		void InnerUpdate(EntityRegistry* _pRegistry, const Entity _self);

	private:
		Entity self_;
		EntityRegistry* pRegistry_;
		
	private:
		static inline std::shared_ptr<T> pInstance_{ std::make_shared<T>() };
	};
}

#include "ScriptBase.inl"
