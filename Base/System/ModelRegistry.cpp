#include "ModelRegistry.h"
#include "../Structure/ModelLoader/FbxLoader.h"
#include "MeshRegistry.h"

GameBase::System::ModelRegistry::ModelRegistry()
{}

GameBase::System::ModelRegistry::~ModelRegistry()
{}

void GameBase::System::ModelRegistry::OnRegisterDependencies(FluentVectorAddOnly<SystemIndex>* _registry)
{
	_registry
		->Add(SystemRegistry::GetSystemIndex<MeshRegistry>());
	;
}

void GameBase::System::ModelRegistry::Initialize()
{
	models_.SetDefaultResource(Model{});
}

void GameBase::System::ModelRegistry::Update(EntityRegistry&)
{}

void GameBase::System::ModelRegistry::Release()
{}

GameBase::ModelHandle GameBase::System::ModelRegistry::Load(const fs::path& _file)
{
	ModelHandle hModel{ models_.Find([&_file](const Model& _model) { return !_model.filePath.empty() && fs::equivalent(_model.filePath, _file); }) };

	if (hModel != INVALID_HANDLE)
	{
		return hModel;  // すでに読み込み済みならそれを取得
	}

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

		return models_.Emplace(model);

		Debugger::LogEnd();

	}
	else
	{
		GB_ASSERT(false && "FBXモデル以外に対応していません。");
	}


	return INVALID_HANDLE;
}

const GameBase::Model& GameBase::System::ModelRegistry::At(const std::string& _fileName)
{
	return models_.At(Find(_fileName));
}

GameBase::ModelHandle GameBase::System::ModelRegistry::Find(const std::string& _fileName)
{
	if (!fs::is_regular_file(_fileName))
	{
		return INVALID_HANDLE;  // ファイルじゃないなら無視
	}
	return models_.Find([_fileName](const Model& _model)
		{
			if (!fs::is_regular_file(_model.filePath))
			{
				return false;  // ファイルじゃないなら不一致
			}
			return fs::equivalent(_model.filePath, _fileName);
		});
}
