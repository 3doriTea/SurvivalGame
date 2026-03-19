#include "ModelRegistry.h"
#include "../Structure/ModelLoader/FbxLoader.h"

GameBase::System::ModelRegistry::ModelRegistry()
{}

GameBase::System::ModelRegistry::~ModelRegistry()
{}

void GameBase::System::ModelRegistry::Initialize()
{}

void GameBase::System::ModelRegistry::Update(EntityRegistry&)
{}

void GameBase::System::ModelRegistry::Release()
{}

GameBase::ModelHandle GameBase::System::ModelRegistry::Load(const fs::path& _file)
{
	if (_file.extension() == ".fbx")
	{
		Debugger::LogBegin("FBX_LOADING");
		FbxLoader loader{ _file };

		Model model{};
		if (loader.TryLoad(&model) == false)
		{
			// 失敗
			return INVALID_HANDLE;
		}

		models_.Emplace(model);

		Debugger::LogEnd();

	}
	else
	{
		GB_ASSERT(false && "FBXモデル以外に対応していません。");
	}


	return INVALID_HANDLE;
}
