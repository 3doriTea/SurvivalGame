#pragma once
#include <vector>

namespace GameBase
{
	/// <summary>
	/// つなげて追加できる vector ラッパ
	/// </summary>
	/// <typeparam name="T">要素の型</typeparam>
	template<typename T>
	class FluentVector
	{
	public:
		FluentVector() = default;
		virtual ~FluentVector() = default;

		/// <summary>
		/// 要素を追加する
		/// </summary>
		/// <param name="_value">追加する要素</param>
		/// <returns>このインスタンスのポインタ</returns>
		inline FluentVector* Add(T&& _value) { v_.emplace_back(_value); return this; }

		/// <summary>
		/// 要素をクリアする
		/// </summary>
		inline void clear() { v_.clear(); }
		inline auto begin() { return v_.begin(); }
		inline auto end() { return v_.end(); }
		inline auto begin() const { return v_.begin(); }
		inline auto end() const { return v_.end(); }
		inline size_t size() const { return v_.size(); }
		inline std::vector<T> Unwarp() { return std::move(v_); }

	private:
		std::vector<T> v_;
	};

	/// <summary>
	/// [追加専用] つなげて追加できる vector ラッパ
	/// </summary>
	/// <typeparam name="T">要素の型</typeparam>
	template<typename T>
	class FluentVectorAddOnly : public FluentVector<T>
	{
	public:
		FluentVectorAddOnly() = default;
		~FluentVectorAddOnly() = default;

		using FluentVector<T>::Add;

	private:  // privateに変更
		/*using FluentVector<T>::clear;
		using FluentVector<T>::begin;
		using FluentVector<T>::end;
		using FluentVector<T>::begin;
		using FluentVector<T>::end;*/

	};
}
