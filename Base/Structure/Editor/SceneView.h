#pragma once
#include "IEditorView.h"

namespace GameBase::Editor
{
	/// <summary>
	/// エディタ部品 : シーンビュー
	/// </summary>
	class SceneView : public IEditorView
	{
	public:
		SceneView();
		inline ~SceneView() = default;

		/// <summary>
		/// ImGUI描画タイミング
		/// </summary>
		/// <param name=""></param>
		/// <returns>選択されたか</returns>
		bool OnGUI(EntityRegistry&) override;


	};
}
