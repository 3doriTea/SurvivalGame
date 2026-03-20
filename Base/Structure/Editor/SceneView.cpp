#include "SceneView.h"
#include <System/ViewportSwitcher.h>


GameBase::Editor::SceneView::SceneView()
{}

bool GameBase::Editor::SceneView::OnGUI(EntityRegistry&)
{
	ImGui::Begin("シーン編集");


	ImTextureRef pTexture{};
	Get<System::ViewportSwitcher>().RefAt(
		ViewportMode::Gamer,
		[&pTexture](const RenderSurface& _surface)
		{
			GB_ASSERT(_surface.pShaderResourceView.Get());
			pTexture = _surface.pShaderResourceView.Get();
		});

	if (pTexture._TexID) {
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		// サイズが 0 だと何も映らないので確認
		ImGui::Text("Panel Size: %.1f, %.1f", viewportPanelSize.x, viewportPanelSize.y);

		// テクスチャを描画
		ImGui::Image(pTexture, viewportPanelSize, ImVec2(0, 0), ImVec2(1, 1));
	}
	else {
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "SRV is NULL!");
	}

	
	ImVec2 viewportPanelSize{ ImGui::GetContentRegionAvail() };
	ImGui::Image(pTexture, viewportPanelSize, ImVec2{ 0, 0 }, ImVec2{ 1, 1 });

	ImGui::End();

	return false;
}
