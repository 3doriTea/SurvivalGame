#pragma once
#include "Entity.h"
#include "IComponentPool.h"


namespace GameBase
{
	template<typename T>
	class ComponentPool : public IComponentPool
	{
	public:
		ComponentPool(const uint32_t _capacity = 1024);
		~ComponentPool() = default;

		IComponentBase& Assign(const EntityIndex _index) override;

		void Assign(EntityIndex _entityIndex, T _component);

		/// <summary>
		/// エンティティがコンポーネントを持っているか
		/// </summary>
		/// <param name="_entityIndex">エンティティのインデクス</param>
		/// <returns>持っている true / false</returns>
		bool Has(const EntityIndex _entityIndex) const;

		/// <summary>
		/// コンポーネントを取得する
		/// </summary>
		/// <param name="_entityIndex">エンティティのインデクス</param>
		/// <returns>コンポーネントの参照ポインタ</returns>
		T& Get(const EntityIndex _entityIndex);

		/// <summary>
		/// コンポーネントをインタフェースで取得する
		/// </summary>
		/// <param name="_index">インデクス</param>
		/// <returns>コンポーネントインタフェースの参照ポインタ</returns>
		IComponentBase& GetI(const EntityIndex _index) override;

		/// <summary>
		/// コンポーネントを削除する
		/// </summary>
		/// <param name="_entityIndex">エンティティのインデクス</param>
		void Remove(const EntityIndex _entityIndex) override;

		auto begin() { return denseData_.begin(); }
		auto end() { return denseData_.end(); }

	private:
		std::vector<T> denseData_;            // データが隙間なく並ぶ
		std::vector<uint32_t> denseToEntity;  // インデックス逆引き
		std::vector<int32_t> sparseIndices_;  // 隙間だらけ
	};
}

#include "ComponentPool.inl"
