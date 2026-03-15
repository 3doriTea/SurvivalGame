#pragma once
#include <functional>
#include <vector>
#include <memory>
#include <Standalone/Utility/Assert.h>

namespace GameBase
{
	/// <summary>
	/// <para>Flyweightパターンでリソースをハンドルで扱うクラス</para>
	/// <para>0番はデフォルトリソース</para>
	/// </summary>
	template<typename T, std::unsigned_integral HandleT>
	class FlyweightFactory
	{
	public:
		FlyweightFactory();
		inline ~FlyweightFactory() = default;

		/// <summary>
		/// 追加する
		/// </summary>
		/// <typeparam name="...Args">可変長引数型</typeparam>
		/// <param name="..._args">コンストラクタ引数</param>
		/// <returns></returns>
		template<typename ...Args>
		requires std::constructible_from<T, Args...>
		HandleT Emplace(Args&&... _args);

		/// <summary>
		/// 取得または作成
		/// </summary>
		/// <param name="_matchFunc">一致判定処理bool(const T&)</param>
		/// <returns>ハンドル</returns>
		HandleT GetOrCreate(const std::function<bool(const T&)>& _matchFunc, const std::function<T()>& _createFunc);

		/// <summary>
		/// インデックス0 デフォルトのリソースを設定する
		/// </summary>
		/// <typeparam name="...Args">可変長引数型</typeparam>
		/// <param name="..._args">コンストラクタ引数</param>
		template<typename ...Args>
		requires std::constructible_from<T, Args...>
		void SetDefaultResource(Args&& ... _args);

		/// <summary>
		/// 要素に参照する
		/// </summary>
		/// <param name="_handle">ハンドル</param>
		/// <returns>参照ポインタ</returns>
		T& At(const HandleT _handle);

	private:
		std::vector<T> data_;    // ロードしたデータ
		HandleT handleCounter_;  // ハンドルカウンター
	};
}

#include "FlyweightFactory.inl"
