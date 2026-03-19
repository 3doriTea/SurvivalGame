#include "FbxLoader.h"
#include <Library/ufbx/ufbx.h>
#include <System/MaterialRegistry.h>
#include <System/MeshRegistry.h>


GameBase::FbxLoader::FbxLoader(const fs::path& _modelFile) :
	modelFile_{ _modelFile }
{}

bool GameBase::FbxLoader::TryLoad(Model* _pOut)
{
	ufbx_error error{};
	ufbx_load_opts options{};

	options.target_axes = ufbx_axes_left_handed_y_up;  // 左手座標系
	options.target_unit_meters = 1.0f;

	ufbx_scene* pScene{ ufbx_load_file(modelFile_.string().c_str(), &options, &error) };
	GB_ASSERT(pScene && "FBXシーンの読み込みに失敗",
		std::format("{}", error.description.data));
	if (!pScene)
	{
		return false;
	}

	for (ufbx_node* pNode : pScene->nodes)
	{
		Debugger::LogF("{}", pNode->name.data);
	}

	ufbx_free_scene(pScene);
	
	return false;  // TODO 後で成功にする
}
