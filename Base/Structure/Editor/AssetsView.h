#pragma once
#include "IEditorView.h"

namespace GameBase::Editor
{
	/// <summary>
	/// エディタ部品 : アセットビュー
	/// </summary>
	class AssetsView : public IEditorView
	{
	public:
		AssetsView();
		inline ~AssetsView() = default;

		/// <summary>
		/// ImGUI描画タイミング
		/// </summary>
		void OnGUI() override;

	private:
		TextureHandle hImageUpDir_;  // 上階層へのアイコン
	};
}
