#pragma once
#include "../../ECS/EntityRegistry.h"

namespace GameBase::Editor
{
	/// <summary>
	/// エディタ上の部品インタフェース
	/// </summary>
	class IEditorView
	{
	public:
		inline IEditorView() = default;
		inline virtual ~IEditorView() = default;

		/// <summary>
		/// ImGUI描画タイミング
		/// </summary>
		virtual void OnGUI(EntityRegistry&) = 0;
	};
}
