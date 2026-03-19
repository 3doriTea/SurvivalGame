#pragma once
#include <Library/ufbx/ufbx.h>
#include "../Model.h"

namespace GameBase
{
	class FbxLoader
	{
	public:
		FbxLoader(const fs::path& _modelFile);
		inline FbxLoader() = default;

		bool TryLoad(Model* _pOut);


	private:
		fs::path modelFile_;  // ファイルパス
	};
}
