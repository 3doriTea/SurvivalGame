#pragma once
#include "Entity.h"
#include "IComponentPool.h"

namespace GameBase
{
	template<typename T>
	class ComponentPool : public IComponentPool
	{
	public:
		ComponentPool(const uint32_t _capacity = 1024)
		{
			sparseIndices_.resize(_capacity, -1);
		}
		~ComponentPool() = default;

		void Assign(EntityIndex _entityIndex, T _component)
		{
			if (Has(_entityIndex))
			{
				return;
			}

			sparseIndices_[_entityIndex] = static_cast<int32_t>(denseData_.size());
			denseToEntity.push_back(_entityIndex);
			denseData_.push_back(_component);
		}

		/// <summary>
		/// エンティティがコンポーネントを持っているか
		/// </summary>
		/// <param name="_entityIndex">エンティティのインデクス</param>
		/// <returns>持っている true / false</returns>
		bool Has(const EntityIndex _entityIndex) const
		{
			return _entityIndex < sparseIndices_.size()
				&& sparseIndices_[_entityIndex] != -1;
		}

		/// <summary>
		/// コンポーネントを取得する
		/// </summary>
		/// <param name="_entityIndex">エンティティのインデクス</param>
		/// <returns>コンポーネントの参照ポインタ</returns>
		T& Get(const EntityIndex _entityIndex)
		{
			return denseData_[sparseIndices_[_entityIndex]];
		}

		/// <summary>
		/// コンポーネントを削除する
		/// </summary>
		/// <param name="_entityIndex">エンティティのインデクス</param>
		void Remove(const EntityIndex _entityIndex)
		{
			if (Has(_entityIndex) == false)
			{
				return;  // すでに持っていないなら無視
			}

			int32_t indexToRemove{ sparseIndices_[_entityIndex] };
			int32_t lastIndex{ static_cast<int32_t>(denseData_.size()) - 1 };

			// 末端要素を削除する要素へ移動(上書きする)
			denseData_[indexToRemove] = denseData_[lastIndex];
			denseToEntity[indexToRemove] = denseToEntity[lastIndex];

			// 移動した要素のSparseIndexを更新
			uint32_t movedEntityIndex{ denseToEntity[indexToRemove] };
			sparseIndices_[movedEntityIndex] = indexToRemove;

			// 末端を消す
			denseData_.pop_back();
			denseToEntity.pop_back();
			sparseIndices_[_entityIndex] = -1;
		}

		auto begin() { return denseData_.begin(); }
		auto end() { return denseData_.end(); }

	private:
		std::vector<T> denseData_;            // データが隙間なく並ぶ
		std::vector<uint32_t> denseToEntity;  // インデックス逆引き
		std::vector<int32_t> sparseIndices_;  // 隙間だらけ
	};
}
