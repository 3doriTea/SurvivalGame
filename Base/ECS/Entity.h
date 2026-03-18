#pragma once

namespace GameBase
{
	using EntityIndex = uint32_t;
	using EntityVersion = uint32_t;
	using Entity = uint64_t;

	static inline Entity MakeEntity(const EntityIndex _index, const EntityVersion _version)
	{
		Entity created{};
		created = _version;
		created <<= sizeof(EntityIndex) * 8;
		created |= _index;
		return created;
	}

	/// <summary>
	/// エンティティからバージョンを取得する
	/// </summary>
	/// <param name="_entity">エンティティ</param>
	/// <returns>エンティティの所属するバージョン</returns>
	static inline EntityVersion GetVersion(const Entity _entity)
	{
		Entity entity{ _entity };
		entity >>= sizeof(EntityIndex) * 8;
		return static_cast<EntityVersion>(entity);
	}

	/// <summary>
	/// エンティティからインデクスを取得する
	/// </summary>
	/// <param name="_entity">エンティティ</param>
	/// <returns></returns>
	static inline EntityIndex GetIndex(const Entity _entity)
	{
		return static_cast<EntityIndex>(_entity);
	}

	static inline constexpr Entity INVALID_ENTITY{ 0 };
}

namespace YAML
{
	/*template<>
	struct convert<GameBase::EntityIndex>
	{
		inline static Node encode(const GameBase::EntityIndex& _rhs)
		{
			Node node{ static_cast<int>(_rhs) };
			return node;
		}

		inline static bool decode(const Node& _node, GameBase::EntityIndex& _rhs)
		{
			_rhs = static_cast<GameBase::EntityIndex>(_node.as<int>());
			return true;
		}
	};*/
}
