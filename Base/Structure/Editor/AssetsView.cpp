#include "pch.h"
#include "AssetsView.h"
#include "../../System/Editor/EditorBase.h"
#include <System/TextureRegistry.h>
#include <System/ModelRegistry.h>

#include <ShlObj.h>
#pragma comment(lib, "shell32.lib")

namespace
{
	using namespace GameBase;
	using AssetType = GameBase::Editor::AssetsView::AssetTypes;
	using EntityRegistry = GameBase::EntityRegistry;

	static const std::map<GameBase::Editor::AssetsView::AssetTypes, std::string_view> FILE_NAMES
	{
		{ GameBase::Editor::AssetsView::ASSET_UNKNOWN_FILE, "UnknownFile.png" },
		{ GameBase::Editor::AssetsView::ASSET_UNKNOWN_FOLDER, "Folder.png" },
		{ GameBase::Editor::AssetsView::ASSET_CPP_HEADER, "CppHeader.png" },
		{ GameBase::Editor::AssetsView::ASSET_CPP_SOURCE, "CppSource.png" },
		{ GameBase::Editor::AssetsView::ASSET_MODEL_FBX, "ModelFbx.png" },
		{ GameBase::Editor::AssetsView::ASSET_UP_DIRECTORY, "Updir.png" },
	};

	static const std::map<std::string_view, GameBase::Editor::AssetsView::AssetTypes> EXT_TO_TYPE
	{
		{ ".cpp", GameBase::Editor::AssetsView::ASSET_CPP_SOURCE },
		{ ".h", GameBase::Editor::AssetsView::ASSET_CPP_HEADER },
		{ ".hpp", GameBase::Editor::AssetsView::ASSET_CPP_HEADER },
		{ ".fbx", GameBase::Editor::AssetsView::ASSET_MODEL_FBX },
	};

	/// <summary>
	/// 各アセットを右クリックしたときのメニュー
	/// </summary>
	static const std::map<
		GameBase::Editor::AssetsView::AssetTypes,
		std::function<void(GameBase::EntityRegistry&, const fs::path&)>> MENU_CONTEXT
	{
		{
			AssetType::ASSET_MODEL_FBX, [](EntityRegistry& _registry, const fs::path& _file)
			{
				if (ImGui::MenuItem("Load"))
				{
					ModelHandle hModel{ Get<System::ModelRegistry>().Load(_file) };
					MessageBox(NULL, std::format("Loaded:{}", hModel).c_str(), "Fbx Loaded", MB_OK);
				}
			}
		}
	};
}

GameBase::Editor::AssetsView::AssetsView(const Config& _config) :
	ROOT_PATH_{ _config.rootPath },
	MIN_ICON_COUNT_{ _config.minIconCount },
	thumbnailSize_{ 64.0f },
	padding_{ 16.0f },
	thumbnailRange_
	{
		.minSize = 32.0f,
		.maxSize = 256.0f,
	},
	inCellMargin_{ 5.0f },
	textHeight_{ 20.0f },
	currentPath_{ _config.rootPath }
{
	for (int i = 0; i < ASSET_TYPE_MAX; i++)
	{
		AssetTypes type{ static_cast<AssetTypes>(i) };

		hIcons_.at(type) = Get<System::TextureRegistry>().Load(
			fs::path{ "./Assets/Images/Editor" } / FILE_NAMES.at(type));
	}
}

bool GameBase::Editor::AssetsView::OnGUI(EntityRegistry& _registry)
{
	bool onSelectedEvent{ false };

	cellSize_ =
	{
		thumbnailSize_ + padding_ + (inCellMargin_ * 2.0f),
		thumbnailSize_ + padding_ + (inCellMargin_ * 2.0f) + textHeight_,
	};

	ImGui::Begin("素材箱");

	ImGui::SliderFloat("アイコンサイズ", &thumbnailSize_, thumbnailRange_.minSize, thumbnailRange_.maxSize);
	ImGui::Separator();
	ImGui::BeginChild("icons", {}, false,
		ImGuiWindowFlags_NoTitleBar);

	// 横に表示するアイコン数
	int cloumnCount{ CalculateCloumnCount() };

	//ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, /*ImVec2(10.0f, 15.0f)*/ {}); // 横, 縦
	if (ImGui::BeginTable("AssetTable", cloumnCount))
	{
		if (CanGoUp())
		{
			// 1階層戻るボタン
			ImGui::TableNextColumn();

			ImGui::PushID("__updirectory__");

			bool _ = IsClickCellShow(
				_registry,
				ASSET_UP_DIRECTORY,
				"../");

			// ダブルクリックでディレクトリに入る処理など
			if (IsDoubleClickCell())
			{
				currentPath_ = currentPath_.parent_path();
			}

			ImGui::PopID();
		}

		for (auto& entry : fs::directory_iterator{ currentPath_ })
		{
			ImGui::TableNextColumn();

			// アイテムごとにIDをユニークにする（ファイル名などを利用）
			ImGui::PushID(entry.path().string().c_str());

			AssetTypes assetType{};

			if (entry.is_directory())  // ディレクトリ
			{
				assetType = ASSET_UNKNOWN_FOLDER;
			}
			else if (entry.is_regular_file())  // 普通のファイル
			{
				auto itr{ EXT_TO_TYPE.find(entry.path().extension().string()) };
				if (itr == EXT_TO_TYPE.end())
				{
					// 未登録のファイルタイプ
					assetType = ASSET_UNKNOWN_FILE;
				}
				else
				{
					// 登録済みのファイルタイプ
					assetType = itr->second;
				}
			}

			bool selected{ entry.path() == selectedPath_ };
			if (IsClickCellShow(
				_registry,
				assetType,
				entry.path().filename().string(),
				selected))
			{
				selectedPath_ = entry.path();
				onSelectedEvent = true;
			}

			// ダブルクリックでディレクトリに入る処理など
			if (IsDoubleClickCell())
			{
				if (entry.is_directory())
				{
					currentPath_ = entry.path(); // パスを更新
				}
			}

			ImGui::PopID();
		}
		ImGui::EndTable();
	}
	//ImGui::PopStyleVar(); // スタイルを戻すのを忘れずに

	ShowContextMenu();

	ImGui::EndChild();


	ImGui::End();
	return onSelectedEvent;
}

void GameBase::Editor::AssetsView::OnSelected(EntityRegistry& _registry, SelectedEvent& _event)
{
	if (_event.TryGetEvent<AssetsView>())
	{
		return;  // 自分自身の選択イベントは無視
	}
	else
	{
		selectedPath_ = fs::path{};
	}
}

void GameBase::Editor::AssetsView::SelectInExplorer(const fs::path& _path)
{
	fs::path absoluted{ fs::absolute(_path) };

	PIDLIST_ABSOLUTE pidl{ ILCreateFromPathW(absoluted.c_str()) };
	if (pidl)
	{
		SHOpenFolderAndSelectItems(pidl, 0, nullptr, 0);
		ILFree(pidl);
	}
}

void GameBase::Editor::AssetsView::OpenInExplorer(const fs::path& _path)
{
	ShellExecuteW(
		nullptr,
		L"explore",
		_path.c_str(),
		nullptr,
		nullptr,
		SW_SHOWNORMAL);
}

inline bool GameBase::Editor::AssetsView::IsDirectry(const AssetTypes _assetType)
{
	return _assetType == ASSET_UNKNOWN_FOLDER || _assetType == ASSET_UP_DIRECTORY;
}

bool GameBase::Editor::AssetsView::IsClickCellShow(
	EntityRegistry& _registry,
	const AssetTypes _assetType,
	const std::string_view _text,
	const bool _selected)
{
	bool isClick{ false };

	// サムネ情報
	ImVec2 size = { thumbnailSize_, thumbnailSize_ };
	ImTextureID icon{};
	Get<System::TextureRegistry>().RefAt(
		hIcons_[_assetType],
		[&icon](const Texture& _texture)
		{
			icon = reinterpret_cast<ImTextureID>(_texture.pShaderResourceView.Get());
		});

	// 全体をひとつのグループにし クリック判定などを取りやすくする
	ImGui::BeginGroup();
	ImGui::BeginChild(
		"Tile",
		{
			cellSize_.x,
			cellSize_.y,
		},
		true,
		ImGuiWindowFlags_NoScrollbar);

	if (_selected)
	{
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_HeaderHovered));
	}

	// 選択中ならハイライト、未選択なら背景透明
	ImVec4 bgColor;
	if (_selected)
	{
		bgColor = ImGui::GetStyleColorVec4(ImGuiCol_Header);
	}
	else
	{
		bgColor = ImVec4(0, 0, 0, 0);
	}
	ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
	isClick = ImGui::ImageButton("##thumbnail", icon, size);
	ImGui::PopStyleColor();

	// テキストを中央揃えにする、または折り返して表示
	ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + thumbnailSize_);
	ImGui::Text(_text.data());
	ImGui::PopTextWrapPos();

	// xxxxディレクトリではないなら右クリックメニューを出す
	/*if (_assetType != ASSET_UP_DIRECTORY
		&& _assetType != ASSET_UNKNOWN_FOLDER)*/
	{
		ShowCellContextMenu(_registry, _text, _assetType);
	}

	if (_selected)
	{
		ImGui::PopStyleColor();
	}

	ImGui::EndChild();
	ImGui::EndGroup();

	// グループの範囲を判定し、選択中なら枠を描画
	if (_selected)
	{
		ImVec2 min = ImGui::GetItemRectMin();
		ImVec2 max = ImGui::GetItemRectMax();
		// 少し外側に広げる（パディング）
		//min.x -= 2; min.y -= 2; max.x += 2; max.y += 2;

		ImGui::GetWindowDrawList()->AddRect(
			min, max,
			ImGui::GetColorU32(ImGuiCol_HeaderActive), // 選択色
			3.0f, // 角丸
			0,
			2.0f  // 線の太さ
		);
	}

	return isClick;
}

bool GameBase::Editor::AssetsView::IsDoubleClickCell()
{
	return ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left);
}

void GameBase::Editor::AssetsView::ShowContextMenu()
{
	if (ImGui::BeginPopupContextWindow("AssetWindowContextMenu"))
	{
		if (ImGui::MenuItem("New Folder"))
		{
			// 新しいフォルダの名前を決定（例: NewFolder, NewFolder (1)...）
			fs::path newPath = currentPath_ / "New Folder";
			int suffix = 1;
			while (fs::exists(newPath))
			{
				newPath = currentPath_ / ("New Folder (" + std::to_string(suffix++) + ")");
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

		if (ImGui::MenuItem("Refresh"))
		{
			// リフレッシュ処理
		}

		ImGui::Separator();

		if (ImGui::MenuItem("エクスプローラで開く"))
		{
			OpenInExplorer(currentPath_ / "");
		}

		ImGui::EndPopup();
	}
}

void GameBase::Editor::AssetsView::ShowCellContextMenu(
	EntityRegistry& _registry,
	const std::string_view _fileName,
	const AssetTypes _assetType)
{
	static std::array<char, 1024> renameBuffer{};
	static bool openRenameModal{ false };
	static std::string targetFileName{};

	// 右クリックの処理
	if (ImGui::BeginPopupContextWindow("AssetItemContextMenu"))
	{
		if (!IsDirectry(_assetType) && ImGui::MenuItem("Rename"))
		{
			targetFileName = _fileName;
			renameBuffer.fill('\0');
			std::copy(targetFileName.begin(), targetFileName.end(), renameBuffer.begin());
			openRenameModal = true;
		}

		auto itr{ MENU_CONTEXT.find(_assetType) };
		if (itr != MENU_CONTEXT.end())
		{
			auto& [type, func]{ *itr };
			func(_registry, currentPath_ / _fileName);
		}

		ImGui::Separator();

		if (ImGui::MenuItem("エクスプローラで開く"))
		{
			SelectInExplorer(currentPath_ / _fileName);
		}

		ImGui::EndPopup();
	}

	if (openRenameModal)
	{
		// 階層回避のためにここで処理する
		ImGui::OpenPopup("RenameFilePopup");
		openRenameModal = false;
	}

	// 名前変更ポップアップ
	if (ImGui::BeginPopupModal("RenameFilePopup", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Enter new name:");

		std::function<void()> renameProc
		{
			[this]()
			{
				fs::path newPath = currentPath_ / renameBuffer.data();
				try
				{
					if (!fs::exists(newPath))
					{
						fs::rename(currentPath_ / targetFileName, newPath);
					}
				}
				catch (const fs::filesystem_error& ex)
				{
					GB_ASSERT(false && "名前の変更に失敗",
						std::format("reason:{}", ex.what()));
				}
			}
		};

		// 入力フィールド（Enterキーで確定するように設定）
		if (ImGui::InputText(
			"##newName",
			renameBuffer.data(),
			renameBuffer.size(),
			ImGuiInputTextFlags_EnterReturnsTrue))
		{
			renameProc();
			ImGui::CloseCurrentPopup();
		}

		ImGui::Separator();

		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			renameProc();
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

int GameBase::Editor::AssetsView::CalculateCloumnCount()
{
	float panelWidth{ ImGui::GetContentRegionAvail().x };

	int cloumnCount{ static_cast<int>(panelWidth / cellSize_.x) };
	if (cloumnCount < MIN_ICON_COUNT_)
	{
		cloumnCount = MIN_ICON_COUNT_;
	}

	return cloumnCount;
}
