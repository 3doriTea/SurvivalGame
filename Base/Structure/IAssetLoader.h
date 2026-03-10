#pragma once

namespace GameBase
{
	/// <summary>
	/// アセットファイルを読み取るクラス
	/// </summary>
	struct IAssetLoader
	{
		IAssetLoader() = default;
		~IAssetLoader() = default;

		/// <summary>
		/// 試しにアセットを読み込む
		/// </summary>
		/// <param name="_file">ファイルのパス</param>
		/// <returns>読み込めた true / false</returns>
		virtual bool TryLoad(const fs::path& _file) = 0;
	};
}
