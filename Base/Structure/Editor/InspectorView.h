#pragma once
#include "IEditorView.h"

namespace GameBase::Editor
{
	class InspectorView : public IEditorView
	{
		enum struct Mode
		{
			ShowComponents,
		};

	public:
		InspectorView();
		inline ~InspectorView() = default;

		/// <summary>
		/// ImGUI描画タイミング
		/// </summary>
		/// <param name=""></param>
		bool OnGUI(EntityRegistry&) override;

	private:
		
	};
}
