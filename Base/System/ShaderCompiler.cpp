#include "ShaderCompiler.h"
#include "Assets.h"
#include <d3d11shader.h>
#include <d3dcompiler.h>

#pragma comment(lib, "D3DCompiler.lib")


GameBase::System::ShaderCompiler::ShaderCompiler()
{}

GameBase::System::ShaderCompiler::~ShaderCompiler()
{}

void GameBase::System::ShaderCompiler::OnRegisterDependencies(FluentVectorAddOnly<SystemIndex>*_registry)
{
	_registry
		->Add(SystemRegistry::GetSystemIndex<Assets>())
	;
}

void GameBase::System::ShaderCompiler::Initialize()
{
	Get<Assets>().Ref([](const std::vector<fs::path>& _filesPath)
	{
		
	}, AssetsType::Shader);
}

void GameBase::System::ShaderCompiler::Update()
{}

void GameBase::System::ShaderCompiler::Release()
{}
