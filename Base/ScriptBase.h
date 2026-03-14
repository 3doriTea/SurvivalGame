#pragma once

namespace GameBase
{
	using ScriptIndex = size_t;

	class IScriptBase;
	template<typename T>
	class ScriptBase;

	namespace ScriptRegistry
	{
		/// <summary>
		/// スクリプトのインデクスカウンタ
		/// </summary>
		/// <returns></returns>
		ScriptIndex& IndexCounter();
		/// <summary>
		/// システムのインタフェース格納
		/// </summary>
		std::vector<std::weak_ptr<void>>& PScripts();
		/// <summary>
		/// コンストラクタ実行後に呼び出される処理
		/// </summary>
		std::queue<std::function<void()>>& RegisterQueue();

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

	/// <summary>
	/// スクリプトのベースクラス
	/// </summary>
	/// <typeparam name="T">派生クラス</typeparam>
	template<typename T>
	class ScriptBase : public IScriptBase
	{
	public:
		ScriptBase();
		inline virtual ~ScriptBase() = default;

		virtual void Start() override {}
		virtual void Update() override {}

	private:
		static inline std::shared_ptr<T> pInstance_{ std::make_shared<T>() };
	};
}

#include "ScriptBase.inl"
