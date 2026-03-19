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
		enum AssetTypes : int
		{
			ASSET_UNKNOWN_FILE,
			ASSET_UNKNOWN_FOLDER,
			ASSET_CPP_HEADER,
			ASSET_CPP_SOURCE,
			ASSET_MODEL_FBX,
			ASSET_UP_DIRECTORY,
			ASSET_TYPE_MAX,
		};

		struct Config
		{
			Config() :
				minIconCount{ 1 },
				rootPath{ "./Assets" }
			{}

			int minIconCount; 
			fs::path rootPath;
		};
	public:
		AssetsView(const Config& _config = {});
		inline ~AssetsView() = default;

		/// <summary>
		/// ImGUI描画タイミング
		/// </summary>
		bool OnGUI(EntityRegistry&) override;

		void OnSelected(EntityRegistry& _registry, SelectedEvent& _event) override;

	private:
		/// <summary>
		/// 1つ上の階層に行けるか
		/// </summary>
		/// <returns>行ける true / false</returns>
		inline bool CanGoUp() { return currentPath_.lexically_normal() != ROOT_PATH_.lexically_normal(); }

		/// <summary>
		/// アセットのセルを表示する
		/// </summary>
		bool IsClickCellShow(
			EntityRegistry& _registry,
			const AssetTypes _assetType,
			const std::string_view _text,
			const bool _selected = false);
		/// <summary>
		/// セルがダブルクリックされたか
		/// </summary>
		/// <returns>ダブルクリックされた true / false</returns>
		bool IsDoubleClickCell();

		/// <summary>
		/// 右クリックなどで出るメニュー表示
		/// </summary>
		void ShowContextMenu();

		/// <summary>
		/// 右クリックなどで出るセルのメニュー表示
		/// </summary>
		void ShowCellContextMenu(
			EntityRegistry& _registry,
			const std::string_view _fileName,
			const AssetTypes _assetType);

		/// <summary>
		/// カラム数を計算する
		/// </summary>
		int CalculateCloumnCount();

	private:
		const fs::path ROOT_PATH_;  // アセットフォルダのパス
		const int MIN_ICON_COUNT_;  // 少なくとも表示されるアイコン数

		Vec2 cellSize_;     // タイルサイズ
		float thumbnailSize_;  // サムネサイズ
		float padding_;        // 余白

		float inCellMargin_;  // セル内の余白
		float textHeight_;    // テキストの高さ

		struct
		{
			float minSize;
			float maxSize;
		} thumbnailRange_;

		fs::path currentPath_;  // 開いているディレクトリパス
		fs::path selectedPath_;  // 選択中のファイルパス

		// アセットアイコンハンドル
		std::array<TextureHandle, ASSET_TYPE_MAX> hIcons_;
	};
}
