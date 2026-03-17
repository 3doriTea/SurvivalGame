#pragma once

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
		virtual void OnGUI() = 0;
	};
}
