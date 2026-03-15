#include "FlyweightFactory.h"
template<typename T, std::unsigned_integral HandleT>
inline GameBase::FlyweightFactory<T, HandleT>::FlyweightFactory() :
	data_{},
	handleCounter_{ 1 }
{
}

template<typename T, std::unsigned_integral HandleT>
inline HandleT GameBase::FlyweightFactory<T, HandleT>::GetOrCreate(
	const std::function<bool(const T&)>& _matchFunc,
	const std::function<T()>& _createFunc)
{
	for (auto itr = data_.begin(); itr != data_.end(); itr++)
	{
		if (_matchFunc(*itr))
		{
			// マッチしたならハンドル(index)を返す
			return itr - data_.begin();
		}
	}

	// マッチしなかったら新規作成
	data_.emplace_back(_createFunc());
	HandleT handle{ handleCounter_ };

	handleCounter_++;
	GB_ASSERT(data_.size() == handleCounter_
		&& "まだデフォルトリソースの設定がされていない");
	return handleCounter_;
}

template<typename T, std::unsigned_integral HandleT>
inline T& GameBase::FlyweightFactory<T, HandleT>::At(const HandleT _handle)
{
	return data_[_handle];
}

template<typename T, std::unsigned_integral HandleT>
template<typename ...Args>
requires std::constructible_from<T, Args...>
inline HandleT GameBase::FlyweightFactory<T, HandleT>::Emplace(Args&& ..._args)
{
	data_.emplace_back(std::forward<Args>(_args)...);

	return handleCounter_++;
}

template<typename T, std::unsigned_integral HandleT>
template<typename ...Args>
requires std::constructible_from<T, Args...>
inline void GameBase::FlyweightFactory<T, HandleT>::SetDefaultResource(Args&& ..._args)
{
	data_.emplace_back(std::forward<Args>(_args)...);
}
