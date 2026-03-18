#pragma once

namespace GameBase
{
	struct TextureLoadConfig
	{
		TextureLoadConfig() :
			filter{ D3D11_FILTER_MIN_MAG_MIP_POINT },
			addressU{ D3D11_TEXTURE_ADDRESS_CLAMP },
			addressV{ D3D11_TEXTURE_ADDRESS_CLAMP },
			addressW{ D3D11_TEXTURE_ADDRESS_CLAMP },
			mipLODLevel{ 1.0f },
			format{ DXGI_FORMAT_R8G8B8A8_UNORM },
			viewDimension{ D3D11_SRV_DIMENSION_TEXTURE2D },
			buffer{ .Texture2D{ .MipLevels = 1 } }
		{}

		D3D11_FILTER filter;
		D3D11_TEXTURE_ADDRESS_MODE addressU;
		D3D11_TEXTURE_ADDRESS_MODE addressV;
		D3D11_TEXTURE_ADDRESS_MODE addressW;
		FLOAT mipLODLevel;

		DXGI_FORMAT format;
		D3D11_SRV_DIMENSION viewDimension;

		union
		{
			D3D11_BUFFER_SRV Buffer;
			D3D11_TEX1D_SRV Texture1D;
			D3D11_TEX1D_ARRAY_SRV Texture1DArray;
			D3D11_TEX2D_SRV Texture2D;
			D3D11_TEX2D_ARRAY_SRV Texture2DArray;
			D3D11_TEX2DMS_SRV Texture2DMS;
			D3D11_TEX2DMS_ARRAY_SRV Texture2DMSArray;
			D3D11_TEX3D_SRV Texture3D;
			D3D11_TEXCUBE_SRV TextureCube;
			D3D11_TEXCUBE_ARRAY_SRV TextureCubeArray;
			D3D11_BUFFEREX_SRV BufferEx;
		} buffer;
	};
}
