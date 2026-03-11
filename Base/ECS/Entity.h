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
		entity = static_cast<EntityVersion>(entity);
		return entity;
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
}
