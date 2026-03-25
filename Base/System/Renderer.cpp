#include "Renderer.h"
#include "GameTime.h"
#include "Presenter.h"
#include "Direct3D.h"
#include "Graphic/MainCamera.h"
#include "ViewportSwitcher.h"
#include "TextureRegistry.h"
#include "ShaderCompiler.h"
#include "EnvConstant.h"


GameBase::System::Renderer::Renderer() :
	beginEvent_{ Event<EntityRegistry&>::Create() },
	renderEvent_{ Event<EntityRegistry&>::Create() },
	releasedTargetEvent_{ Event<EntityRegistry&>::Create() },
	renderLateEvent_{ Event<EntityRegistry&>::Create() },
	endEvent_{ Event<EntityRegistry&>::Create() }
{
}

GameBase::System::Renderer::~Renderer()
{
}

void GameBase::System::Renderer::OnRegisterDependencies(FluentVectorAddOnly<SystemIndex>* _registry)
{
	_registry
		->Add(SystemRegistry::GetSystemIndex<GameTime>())
		->Add(SystemRegistry::GetSystemIndex<Presenter>())
		->Add(SystemRegistry::GetSystemIndex<MainCamera>())
		->Add(SystemRegistry::GetSystemIndex<ViewportSwitcher>())
		->Add(SystemRegistry::GetSystemIndex<Direct3D>())
		->Add(SystemRegistry::GetSystemIndex<TextureRegistry>())
		->Add(SystemRegistry::GetSystemIndex<ShaderCompiler>())
		->Add(SystemRegistry::GetSystemIndex<EnvConstant>())
		;
}

void GameBase::System::Renderer::Initialize()
{
}

void GameBase::System::Renderer::Update(EntityRegistry& _registry)
{
	if (!Get<GameTime>().IsFrameDue())
	{
		return;  // フレーム更新タイミング以外は無視
	}

	// 描画キューをソートする
	std::sort(
		renderQueue_.begin(),
		renderQueue_.end(),
		[](const RenderItem& _left, const RenderItem& _right) -> bool
		{
			return _left.sortKey.value < _right.sortKey.value;
		});

	Get<ViewportSwitcher>().Switch(ViewportMode::Gamer);
	beginEvent_.get()->Invoke(_registry);

	Get<Presenter>().BeginDraw();

	// レンダーキューの処理をする
	// 各ウィンドウ分、カメラ別で描画していく

	enum ConstantSlotOffset_
	{
		ConstantSlotOffset_Original,
		ConstantSlotOffset_Env,
	};

	// マテリアルを連続で描画するために持っておく
	const Material* pCurrentMaterial{ nullptr };
	for (auto& item : renderQueue_)
	{
		Get<Direct3D>().Ref([&pCurrentMaterial, item, &_registry](const ComPtr<ID3D11DeviceContext>& _pContext)
			{
				// 描画するマテリアルをチェンジ
				if (pCurrentMaterial != item.pMaterial)
				{
					pCurrentMaterial = item.pMaterial;

					enum SamplerSlotOffset_
					{
						TextureSlotOffset_Diffuse,
						TextureSlotOffset_Normal,
					};

					ShaderHandle hShader{ pCurrentMaterial->base.Shader() };
					if (hShader != INVALID_HANDLE)
					{
						 Shader& shader{ Get<ShaderCompiler>().At(hShader) };
						 _pContext.Get()->VSSetShader(shader.pVertexShader.Get(), nullptr, 0);  // チョウテンシェーダ
						 _pContext.Get()->PSSetShader(shader.pPixelShader.Get(), nullptr, 0);   // ピクセルシェーダ
						 _pContext.Get()->IASetInputLayout(shader.pVertexInputLayout.Get());    // 頂点インプットレイアウト
						 _pContext.Get()->RSSetState(shader.pRasterizerState.Get());            // ラスタライザステート
					}

					TextureHandle hDiffuseTexture{ item.pMaterial->base.TextureDiffuse() };
					if (hDiffuseTexture != INVALID_HANDLE)  // テクスチャが指定されているならセットする
					{
						Get<TextureRegistry>().RefAt(hDiffuseTexture, [_pContext](const Texture& _texture)
							{
								_pContext.Get()->PSSetSamplers(
									TextureSlotOffset_Diffuse,
									1,
									_texture.pSampler.GetAddressOf());
								_pContext.Get()->PSSetShaderResources(
									TextureSlotOffset_Diffuse,
									1,
									_texture.pShaderResourceView.GetAddressOf());
							});

					}

					// コンスタントバッファのセット
					_pContext.Get()->VSSetConstantBuffers(ConstantSlotOffset_Original, 1, pCurrentMaterial->pConstantBuffer.GetAddressOf());
					_pContext.Get()->PSSetConstantBuffers(ConstantSlotOffset_Original, 1, pCurrentMaterial->pConstantBuffer.GetAddressOf());

					// コンスタントバッファの送信
					HRESULT hResult{};
					D3D11_MAPPED_SUBRESOURCE pData{ nullptr };
					if (hResult = _pContext.Get()->Map(
						item.pMaterial->pConstantBuffer.Get(),
						0,
						D3D11_MAP_WRITE_DISCARD,
						0,
						&pData);
						SUCCEEDED(hResult))
					{
						memcpy_s(
							pData.pData,
							pData.RowPitch,
							item.pMaterial->base.GetPtr(),
							item.pMaterial->base.Size());
						_pContext.Get()->Unmap(item.pMaterial->pConstantBuffer.Get(), 0);
					}
				}

				// 環境情報を環境定数スロットに送る
				Get<EnvConstant>().UpdateSendTo(ConstantSlotOffset_Env, item.transformMatrix, _registry);

				uint32_t offset{ 0 };
				_pContext.Get()->IASetVertexBuffers(
					0,
					1,
					item.pMesh->pVertexBuffer.GetAddressOf(),
					&item.pMesh->vertexStructSize,
					&offset);  // TODO: 配列にして頂点を渡せるようにする

				// インデックスバッファをセット
				_pContext.Get()->IASetIndexBuffer(item.pMesh->pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

				// 描画！
				_pContext.Get()->DrawIndexed(item.pMesh->indexCount, 0, 0);
				
			});
	}

	renderEvent_.get()->Invoke(_registry);

	// --- ゲーム画面描画完了---

	// ゲーム画面の描画完了したから解除
	Get<Presenter>().ReleaseRenderTarget();

	Get<ViewportSwitcher>().Switch(ViewportMode::Editor);

	// 後処理
	releasedTargetEvent_.get()->Invoke(_registry);

	renderLateEvent_.get()->Invoke(_registry);

	Get<Presenter>().EndDraw();

	endEvent_.get()->Invoke(_registry);

	renderQueue_.clear();
}

void GameBase::System::Renderer::Release()
{
}

void GameBase::System::Renderer::OnBegin(const std::function<void(EventSubject<EntityRegistry&>&)>& _callback)
{
	_callback(beginEvent_);
}

void GameBase::System::Renderer::OnRender(const std::function<void(EventSubject<EntityRegistry&>&)>& _callback)
{
	_callback(renderEvent_);
}

void GameBase::System::Renderer::OnReleasedTarget(const std::function<void(EventSubject<EntityRegistry&>&)>& _callback)
{
	_callback(releasedTargetEvent_);
}

void GameBase::System::Renderer::OnRenderLate(const std::function<void(EventSubject<EntityRegistry&>&)>& _callback)
{
	_callback(renderLateEvent_);
}

void GameBase::System::Renderer::OnEnd(const std::function<void(EventSubject<EntityRegistry&>&)>& _callback)
{
	_callback(endEvent_);
}

void GameBase::System::Renderer::Enqueue(RenderItem&& _renderItem)
{
	renderQueue_.emplace_back(std::move(_renderItem));
}
