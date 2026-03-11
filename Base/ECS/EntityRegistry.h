#pragma once
#include "Entity.h"


namespace GameBase
{
	enum ComponentType : uint32_t
	{
		COMPONENT_TYPE_MAX = 64,
	};

	class EntityRegistry
	{
		using Signature = std::bitset<COMPONENT_TYPE_MAX>;

	public:
		EntityRegistry(const EntityVersion _version);
		~EntityRegistry() = default;

		/// <summary>
		/// エンティティを作成する
		/// </summary>
		/// <returns>作成されたエンティティ</returns>
		Entity CreateEntity();

		/// <summary>
		/// エンティティにコンポーネントを追加する
		/// </summary>
		/// <param name="_entity">エンティティ</param>
		/// <param name="_type"></param>
		void AddComponent(const Entity _entity, ComponentType _type);
		/// <summary>
		/// エンティティからコンポーネントを削除する
		/// </summary>
		/// <param name="_entity">エンティティ</param>
		/// <param name="_type"></param>
		void RemoveComponent(const Entity _entity, ComponentType _type);

		/// <summary>
		/// コンポーネントマスクに一致するか
		/// </summary>
		/// <param name="_entity">エンティティ</param>
		/// <param name="_mask">コンポーネントマスク</param>
		/// <returns>一致する true / false</returns>
		inline bool MatchComponents(const Entity _entity, const Signature& _mask) const
		{
			return (entitySignatures_[static_cast<EntityIndex>(_entity)] & _mask)
				== _mask;
		}

		/// <summary>
		/// 総エンティティ数を取得する
		/// </summary>
		/// <returns>総エンティティ数</returns>
		inline size_t TotalEntitiesCount() const { return entityCounter_; }

	private:
		const EntityVersion VERSION_;  // このレジストリのエンティティバージョン
		std::vector<Signature> entitySignatures_;  // 各エンティティのコンポーネントフラグ
		EntityIndex entityCounter_;  // エンティティIdのカウンタ
	};
}
