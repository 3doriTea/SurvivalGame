#include "ComponentPool.h"
#include "../ComponentRegistry.h"


template<typename T>
inline GameBase::ComponentPool<T>::ComponentPool(const uint32_t _capacity)
{
	sparseIndices_.resize(_capacity, -1);
	auto reg = []()
	{
		GameBase::ComponentRegistry::Add<T>(GameBase::ComponentBase<T>::pInstance_, typeid(T).name());
	};

	GameBase::ComponentRegistry::RegisterQueue().push(reg);
}

template<typename T>
inline GameBase::IComponentBase& GameBase::ComponentPool<T>::Assign(const EntityIndex _index)
{
	Assign(_index, T{});
	return Get(_index);
}

template<typename T>
inline void GameBase::ComponentPool<T>::Assign(EntityIndex _entityIndex, T _component)
{
	if (Has(_entityIndex))
	{
		return;
	}

	sparseIndices_[_entityIndex] = static_cast<int32_t>(denseData_.size());
	denseToEntity.push_back(_entityIndex);
	denseData_.push_back(_component);
}

template<typename T>
inline bool GameBase::ComponentPool<T>::Has(const EntityIndex _entityIndex) const
{
	return _entityIndex < sparseIndices_.size()
		&& sparseIndices_[_entityIndex] != -1;
}

template<typename T>
inline T& GameBase::ComponentPool<T>::Get(const EntityIndex _entityIndex)
{
	return denseData_[sparseIndices_[_entityIndex]];
}

template<typename T>
inline GameBase::IComponentBase& GameBase::ComponentPool<T>::GetI(const EntityIndex _index)
{
	return Get(_index);
}

template<typename T>
inline void GameBase::ComponentPool<T>::Remove(const EntityIndex _entityIndex)
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
