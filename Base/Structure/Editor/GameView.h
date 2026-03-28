#pragma once
#include "IEditorView.h"

namespace GameBase::Editor
{
	/// <summary>
	/// エディタ部品 : ゲームビュー
	/// </summary>
	class GameView : public IEditorView
	{
	public:
		GameView();
		inline ~GameView() = default;

		/// <summary>
		/// ImGUI描画タイミング
		/// </summary>
		/// <param name=""></param>
		/// <returns>選択されたか</returns>
		bool OnGUI(EntityRegistry&) override;


	};
}
