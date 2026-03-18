#pragma once
#include "../../ECS/EntityRegistry.h"

namespace GameBase::Editor
{
	class SelectedEvent;
	
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
		/// <returns>選択がされた true / false</returns>
		virtual bool OnGUI(EntityRegistry&) = 0;

		/// <summary>
		/// なにかしら項目選択時のイベント
		/// </summary>
		/// <param name="_event">選択イベント</param>
		inline virtual void OnSelected(SelectedEvent& _event) {}
	};
}
