#pragma once
#include "../../../ECS/Signature.h"

namespace GameBase
{
	/// <summary>
	/// コンポーネント選択
	/// </summary>
	class ComponentSelector
	{
		enum CreateObjectType : int
		{
			Empty,
			_3D,
			UI
		};
	
	public:
		ComponentSelector();
		~ComponentSelector() = default;
	
		/// <summary>
		/// 表示する
		/// </summary>
		void OnGUI();
	
		/// <summary>
		/// テンプレートのフラグ設定にする
		/// </summary>
		void ApplySetTemplateComponentFlags();
		/// <summary>
		/// 必要なフラグを追加する
		/// </summary>
		void ApplyAddRequiredComponentFlags();
	
		inline uint64_t GetComponentFlags() const { return componentFlags_.full; }
		inline void SetComponentFlags(const Signature _flags) { componentFlags_.full = _flags.to_ullong(); }
	
	private:
		int selected_;  // 選択中のテンプレート
		union
		{
			struct { uint32_t lower; uint32_t upper; };
			uint64_t full;
		} componentFlags_;  // コンポーネントのフラグ
	};
}
