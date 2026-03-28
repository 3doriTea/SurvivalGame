#include "EditorPipeLine.h"
#include "../System/ViewportSwitcher.h"
#include "../System/Presenter.h"
#include "../System/Direct3D.h"
#include "../System/ShaderCompiler.h"
#include "../System/TextureRegistry.h"
#include "../System/EnvConstant.h"



void GameBase::EditorPipeLine::Render(System::Renderer& _self, EntityRegistry& _registry)
{
	Get<System::ViewportSwitcher>().Switch(ViewportMode::Gamer);
	_self.beginEvent_.get()->Invoke(_registry);

	Get<System::Presenter>().BeginDraw();

	// レンダーキューの処理をする
	// 各ウィンドウ分、カメラ別で描画していく

	enum ConstantSlotOffset_
	{
		ConstantSlotOffset_Original,
		ConstantSlotOffset_Env,
	};

	// マテリアルを連続で描画するために持っておく
	const Material* pCurrentMaterial{ nullptr };
	for (auto& item : _self.renderQueue_)
	{
		Get<System::Direct3D>().Ref([&pCurrentMaterial, item, &_registry](const ComPtr<ID3D11DeviceContext>& _pContext)
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
						Shader& shader{ Get<System::ShaderCompiler>().At(hShader) };
						_pContext.Get()->VSSetShader(shader.pVertexShader.Get(), nullptr, 0);  // チョウテンシェーダ
						_pContext.Get()->PSSetShader(shader.pPixelShader.Get(), nullptr, 0);   // ピクセルシェーダ
						_pContext.Get()->IASetInputLayout(shader.pVertexInputLayout.Get());    // 頂点インプットレイアウト
						_pContext.Get()->RSSetState(shader.pRasterizerState.Get());            // ラスタライザステート
					}

					TextureHandle hDiffuseTexture{ item.pMaterial->base.TextureDiffuse() };
					if (hDiffuseTexture != INVALID_HANDLE)  // テクスチャが指定されているならセットする
					{
						Get<System::TextureRegistry>().RefAt(hDiffuseTexture, [_pContext](const Texture& _texture)
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
				Get<System::EnvConstant>().UpdateSendTo(ConstantSlotOffset_Env, item.transformMatrix, _registry);

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

	_self.renderEvent_.get()->Invoke(_registry);

	// --- ゲーム画面描画完了---

	// ゲーム画面の描画完了したから解除
	Get<System::Presenter>().ReleaseRenderTarget();

	Get<System::ViewportSwitcher>().Switch(ViewportMode::Editor);

	// 後処理
	_self.releasedTargetEvent_.get()->Invoke(_registry);

	_self.renderLateEvent_.get()->Invoke(_registry);

	Get<System::Presenter>().EndDraw();

	_self.endEvent_.get()->Invoke(_registry);
}
