#include "GameView.h"
#include <System/ViewportSwitcher.h>


GameBase::Editor::GameView::GameView()
{}

bool GameBase::Editor::GameView::OnGUI(EntityRegistry&)
{
	ImGui::Begin("ゲームプレイ画面", nullptr, ImGuiWindowFlags_NoScrollbar);


	ImTextureRef pTexture{};
	Get<System::ViewportSwitcher>().RefAt(
		ViewportMode::Gamer,
		[&pTexture](const RenderSurface& _surface)
		{
			GB_ASSERT(_surface.pShaderResourceView.Get());
			pTexture = _surface.pShaderResourceView.Get();
		});

	if (pTexture._TexID)
	{
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

		// テクスチャを描画
		ImGui::Image(pTexture, viewportPanelSize, ImVec2(0, 0), ImVec2(1, 1));
	}
	else
	{
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "SRV is NULL!");
	}

	ImGui::End();

	return false;
}
