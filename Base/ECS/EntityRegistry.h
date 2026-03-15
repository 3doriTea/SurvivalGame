#pragma once
#include "Entity.h"
#include "View.h"
#include "Signature.h"
#include "IComponentBase.h"


namespace GameBase
{
	class EntityRegistry
	{
		template<typename ...ComponentsT>
		friend class View;

	public:
		EntityRegistry(const EntityVersion _version);
		~EntityRegistry() = default;

		/// <summary>
		/// エンティティを作成する
		/// </summary>
		/// <returns>作成されたエンティティ</returns>
		Entity CreateEntity();

		template<typename T>
		T& GetComponent(const Entity _entity)
		{
			return GetComponent<T>(GetIndex(_entity));
		}

		template<typename T>
		T& GetComponent(const EntityIndex _entityIndex)
		{
			size_t index{ ComponentRegistry::GetComponentIndex<T>() };
			return Get<T>().Get(index);
		}

		/// <summary>
		/// エンティティにコンポーネントを追加する
		/// </summary>
		/// <param name="_entity">エンティティ</param>
		/// <param name="_type"></param>
		/// <returns>コンポーネントの参照ポインタ</returns>
		IComponentBase& AddComponent(const Entity _entity, const ComponentIndex _type);
		/// <summary>
		/// エンティティからコンポーネントを削除する
		/// </summary>
		/// <param name="_entity">エンティティ</param>
		/// <param name="_type"></param>
		void RemoveComponent(const Entity _entity, const ComponentIndex _type);

		/// <summary>
		/// エンティティからコンポーネントを取得する
		/// </summary>
		/// <param name="_entity">エンティティ</param>
		/// <param name="_entity">コンポーネントインデクス</param>
		/// <returns>コンポーネントの参照ポインタ</returns>
		IComponentBase& GetComponent(const Entity _entity, const ComponentIndex _type);

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
		/// コンポーネントマスクに一致するか
		/// </summary>
		/// <param name="_entity">エンティティ</param>
		/// <param name="_mask">コンポーネントマスク</param>
		/// <returns>一致する true / false</returns>
		inline bool MatchComponents(const EntityIndex _entityIndex, const Signature& _mask) const
		{
			return (entitySignatures_[_entityIndex] & _mask)
				== _mask;
		}

		/// <summary>
		/// 総エンティティ数を取得する
		/// </summary>
		/// <returns>総エンティティ数</returns>
		inline size_t TotalEntitiesCount() const { return entityCounter_; }

		template<typename ...Args>
		inline View<Args...> GetView()
		{
			return View<Args...>{ *this };
		}

	private:
		const EntityVersion VERSION_;  // このレジストリのエンティティバージョン
		std::vector<Signature> entitySignatures_;  // 各エンティティのコンポーネントフラグ
		EntityIndex entityCounter_;  // エンティティIdのカウンタ
	};
}
