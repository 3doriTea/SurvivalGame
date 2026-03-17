#include "pch.h"
#include "AssetsView.h"
#include "../../System/Editor/EditorBase.h"
#include <System/TextureRegistry.h>

GameBase::Editor::AssetsView::AssetsView()
{
	hImageUpDir_ = Get<System::TextureRegistry>().Load("./Assets/Images/Editor/UpDir.png");
}

void GameBase::Editor::AssetsView::OnGUI()
{
	ImGui::Begin("Assets");
	static float thumbnailSize{ 64.0f };
	static float padding{ 16.0f };

	ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 32.0f, 256.0f);
	ImGui::Separator();

	float cellSize{ thumbnailSize + padding };

	float panelWidth{ ImGui::GetContentRegionAvail().x };
	int cloumnCount{ static_cast<int>(panelWidth / cellSize) };
	if (cloumnCount < 1)
	{
		cloumnCount = 1;
	}

	static fs::path currentPath{ "./Assets" };

	if (ImGui::BeginTable("AssetTable", cloumnCount))
	{
		{  // 1階層戻るボタン
			ImGui::TableNextColumn();

			ImGui::PushID("__updirectory__");

			// 全体をひとつのグループとして、クリック判定などを取りやすくする
			ImGui::BeginGroup();

			// サムネイル表示（テクスチャIDがある場合は ImGui::Image）
			// ここではUnityっぽくSelectableをボタンのように重ねるテクニック
			ImVec2 size = { thumbnailSize, thumbnailSize };
			ImTextureID icon{};

			Get<System::TextureRegistry>().RefAt(
				hImageUpDir_,
				[&icon](const Texture& _texture)
				{
					icon = reinterpret_cast<ImTextureID>(_texture.pShaderResourceView.Get());
				});

			// 背景透明にする
			ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
			//ImGui::Button("##Icon", size);
			if (ImGui::ImageButton("##thumb", icon, size))
			{
				// クリック
			}
			ImGui::PopStyleColor();

			// テキストを中央揃えにする、または折り返して表示
			ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + thumbnailSize);
			ImGui::Text("../");
			ImGui::PopTextWrapPos();

			ImGui::EndGroup();

			// ダブルクリックでディレクトリに入る処理など
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				currentPath = currentPath.parent_path();
			}

			ImGui::PopID();
		}

		for (auto& entry : fs::directory_iterator{ currentPath })
		{
			ImGui::TableNextColumn();

			// アイテムごとにIDをユニークにする（ファイル名などを利用）
			ImGui::PushID(entry.path().string().c_str());

			// 全体をひとつのグループとして、クリック判定などを取りやすくする
			ImGui::BeginGroup();

			// サムネイル表示（テクスチャIDがある場合は ImGui::Image）
			// ここではUnityっぽくSelectableをボタンのように重ねるテクニック
			ImVec2 size = { thumbnailSize, thumbnailSize };
			ImGui::Button("##Icon", size);

			// テキストを中央揃えにする、または折り返して表示
			ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + thumbnailSize);
			ImGui::Text("%s", entry.path().filename().string().c_str());
			ImGui::PopTextWrapPos();

			ImGui::EndGroup();

			// ダブルクリックでディレクトリに入る処理など
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (entry.is_directory()) {
					currentPath = entry.path(); // パスを更新
				}
			}

			ImGui::PopID();
		}
		ImGui::EndTable();

		if (ImGui::BeginPopupContextWindow("AssetWindowContextMenu"))
		{
			if (ImGui::MenuItem("New Folder"))
			{
				// 新しいフォルダの名前を決定（例: NewFolder, NewFolder (1)...）
				fs::path newPath = currentPath / "New Folder";
				int suffix = 1;
				while (fs::exists(newPath))
				{
					newPath = currentPath / ("New Folder (" + std::to_string(suffix++) + ")");
				}

				// 実際にフォルダを作成
				std::error_code ec;
				if (fs::create_directory(newPath, ec)) {
					// 成功したら必要に応じてファイルリストを再スキャンするフラグを立てる
					// refreshFileList = true; 
				}
				else {
					// エラーハンドリング
				}
			}

			if (ImGui::MenuItem("Refresh")) {
				// リフレッシュ処理
			}

			ImGui::EndPopup();
		}
	}
	ImGui::End();
}
