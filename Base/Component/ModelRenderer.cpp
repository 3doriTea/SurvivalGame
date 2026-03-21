#include "ModelRenderer.h"

GameBase::Component::ModelRenderer::ModelRenderer() :
	hModel{ INVALID_HANDLE }
{
}

void GameBase::Component::ModelRenderer::OnLoad(const YAML::Node& _node, const SchemaLoadBundle& _bundle)
{
	hModel = _bundle.assetIdToModel.at(_node["hModel"]["assetId"].as<std::string>());
}

void GameBase::Component::ModelRenderer::OnSave(YAML::Emitter& _emitter, const SchemaLoadBundle& _bundle)
{
	_emitter << YAML::Key << "hModel" << YAML::Value << YAML::BeginMap;
	_emitter << YAML::Key << "assetId" << YAML::Value << _bundle.modelToAssetId.at(hModel) << YAML::EndMap;
}
