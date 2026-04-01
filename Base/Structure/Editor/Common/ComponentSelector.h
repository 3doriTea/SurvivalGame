#pragma once


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

	void ApplyRequiredCreateObjectComponentFlags();

	inline uint64_t GetComponentFlags() const { return componentFlags_.full; }

private:
	int selected_;  // 選択中のテンプレート
	union
	{
		struct { uint32_t lower; uint32_t upper; };
		uint64_t full;
	} componentFlags_;  // コンポーネントのフラグ
};
