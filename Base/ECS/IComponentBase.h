#pragma once

namespace GameBase
{
	/// <summary>
	/// コンポーネントのベースインタフェース
	/// </summary>
	struct IComponentBase
	{
		inline IComponentBase() = default;
		inline virtual ~IComponentBase() = default;

		virtual void OnLoad(const YAML::Node& _node) = 0;
		virtual void OnSave(YAML::Emitter& _emitter) = 0;
	};
}
