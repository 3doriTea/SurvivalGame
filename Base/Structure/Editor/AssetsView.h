#pragma once
#include "IEditorView.h"
#include "../../Structure/AssetType.h"

namespace GameBase::Editor
{
	/// <summary>
	/// エディタ部品 : アセットビュー
	/// </summary>
	class AssetsView : public IEditorView
	{
	public:
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
		/// エクスプローラで選択状態にして開く
		/// </summary>
		/// <param name="_path">選択するパス</param>
		void SelectInExplorer(const fs::path& _path);

		/// <summary>
		/// エクスプローラで開く
		/// </summary>
		/// <param name="_path">開くパス</param>
		void OpenInExplorer(const fs::path& _path);

		/// <summary>
		/// アセットはディレクトリであるか
		/// </summary>
		/// <param name="_assetType">アセットの種類</param>
		/// <returns>ディレクトリである true / false</returns>
		bool IsDirectry(const AssetType _assetType);

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
			const AssetType _assetType,
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
			const AssetType _assetType);

		/// <summary>
		/// カラム数を計算する
		/// </summary>
		int CalculateCloumnCount();

		/// <summary>
		/// アセットを作成
		/// </summary>
		void CreateAsset();

		/// <summary>
		/// 作成するアセット名が無効か
		/// </summary>
		/// <returns>無効である true / false</returns>
		bool IsInvalidCreateAssetName();

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

		struct
		{
			std::array<char, MAX_PATH> name;
			AssetType type;  // 選択中のアセットタイプ
		} createAssetOption_;

		// アセットアイコンハンドル
		std::array<TextureHandle, AssetType_MAX> hIcons_;
	};
}
