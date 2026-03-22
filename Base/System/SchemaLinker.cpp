#include "SchemaLinker.h"
#include "Assets.h"
#include "ModelRegistry.h"
#include "GameTime.h"


GameBase::System::SchemaLinker::SchemaLinker() :
	loadBundle_{},
	preModel2AssetId_{}
{}

GameBase::System::SchemaLinker::~SchemaLinker()
{}

void GameBase::System::SchemaLinker::OnRegisterDependencies(FluentVectorAddOnly<SystemIndex>* _registry)
{
	_registry
		->Add(SystemRegistry::GetSystemIndex<Assets>())
		->Add(SystemRegistry::GetSystemIndex<ModelRegistry>())
		->Add(SystemRegistry::GetSystemIndex<GameTime>())
	;
}

void GameBase::System::SchemaLinker::Initialize()
{
	// 各アセットのハンドルも登録
	preModel2AssetId_.emplace(INVALID_HANDLE, "0");
	preModel2AssetId_.emplace(INVALID_HANDLE, "");
	Get<System::Assets>().Ref([this](const std::vector<fs::path>& _modelsPath)
		{
			for (auto& file : _modelsPath)
			{
				ModelHandle hModel{ Get<ModelRegistry>().Load(file) };

				if (hModel == INVALID_HANDLE)
				{
					continue;  // 読み込み失敗なら無視
				}

				preModel2AssetId_.emplace(hModel, file.string());
			}
		},
		AssetType_ModelFbx);
}

void GameBase::System::SchemaLinker::Update(EntityRegistry& _registry)
{
	if (!Get<GameTime>().IsFrameDue())
	{
		return;
	}

	loadBundle_.entityToFileId.clear();
	loadBundle_.fileIdToEntity.clear();

	loadBundle_.entityToFileId.emplace(INVALID_ENTITY, INVALID_ENTITY);
	loadBundle_.fileIdToEntity.emplace(INVALID_ENTITY, INVALID_ENTITY);

	auto view{ _registry.GetView() };
	view.ForEach([this](const Entity _entity)
		{
			loadBundle_.entityToFileId.emplace(_entity, _entity);
			loadBundle_.fileIdToEntity.emplace(_entity, _entity);
		});

	loadBundle_.modelToAssetId.clear();
	loadBundle_.assetIdToModel.clear();
	for (auto& [hModel, assetId] : preModel2AssetId_)
	{
		loadBundle_.modelToAssetId.emplace(hModel, assetId);
		loadBundle_.assetIdToModel.emplace(assetId, hModel);
	}
	
}

void GameBase::System::SchemaLinker::Release()
{}

void GameBase::System::SchemaLinker::Reload()
{
}

void GameBase::System::SchemaLinker::RefLoadBundle(const std::function<void(const SchemaLoadBundle&)>&_callback) const
{
	_callback(loadBundle_);
}
