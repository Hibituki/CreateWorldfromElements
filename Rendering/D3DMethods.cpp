


#include "D3DAPIs.h"
#include "../OtherUtility/CWEUtility.h"


// 主要インターフェイス
Interface_ptr<ID3D11Device> g_pDevice(nullptr, deleter::Release<ID3D11Device>);
Interface_ptr<ID3D11DeviceContext> g_pDeviceContext(nullptr, deleter::Release<ID3D11DeviceContext>);
Interface_ptr<IDXGIFactory> g_pFactory(nullptr, deleter::Release<IDXGIFactory>);

// 描画デバイスのコアの使用準備
bool RenderingPre()
{
	//ファクトリの確認
	if(!g_pFactory)
	{
		IDXGIFactory* pFactory =nullptr;
		if(FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&pFactory))))
		{
			return false;
		}

		g_pFactory.reset(pFactory);
	}

	//デバイスとデバイスコンテキストの確認
	if(!g_pDevice || !g_pDeviceContext)
	{
		const D3D_FEATURE_LEVEL futureLevel =D3D_FEATURE_LEVEL_11_0;
		D3D_FEATURE_LEVEL futureLevelOutput;
		ID3D11Device* pDevice =nullptr;
		ID3D11DeviceContext* pDeviceContext =nullptr;

		if(FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &futureLevel, 1, D3D11_SDK_VERSION,
			&pDevice, &futureLevelOutput, &pDeviceContext)))
		{
			return false;
		}

		g_pDevice.reset(pDevice);
		g_pDeviceContext.reset(pDeviceContext);
	}

	return true;
}




//***************************************************************************************
// =================================== Core.h =========================================
//***************************************************************************************

#include "Core.h"
#include "Window.h"
#include "Texture.h"


// デバイスがロストしたか
bool DeviceLost()
{
	if(RenderingPre()) {return false;}

	if(g_pDevice->GetDeviceRemovedReason() != S_OK) {return true;}
	
	return false;
}



// ======================= SwapChain ================================

// デフォルトコンストラクタ
SwapChain::SwapChain()
{

}

// 出力ウィンドウを指定して生成
SwapChain::SwapChain(const Window& target)
{
	SetProperty(target);
}

// デフォルトデストラクタ
SwapChain::~SwapChain()
{
	if(*this)
	{
		RawData()->SetFullscreenState(FALSE, nullptr);
	}
}


// 各種初期設定
bool SwapChain::SetProperty(const Window& target)
{
	if(RenderingPre()) {return false;}

	IDXGISwapChain* pSwapChain =nullptr;
	DXGI_SWAP_CHAIN_DESC desc{};
	desc.BufferCount =1;
	desc.BufferDesc.Width =target.Width();
	desc.BufferDesc.Height =target.Height();
	desc.BufferDesc.Format =DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	desc.BufferDesc.RefreshRate.Numerator =60;
	desc.BufferDesc.RefreshRate.Denominator =1;
	desc.BufferDesc.ScanlineOrdering =DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferDesc.Scaling =DXGI_MODE_SCALING_UNSPECIFIED;
	desc.SampleDesc.Count =1;
	desc.SampleDesc.Quality =0;
	desc.BufferUsage =DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow =target.Handle();
	desc.Windowed =TRUE;
	desc.SwapEffect =DXGI_SWAP_EFFECT_DISCARD;
	desc.Flags =DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	if(FAILED(g_pFactory->CreateSwapChain(g_pDevice.get(), &desc, &pSwapChain)))
	{
		return false;
	}

	SetRawData(pSwapChain);

	return true;
}


// レンダリング結果を表示
void SwapChain::Present(unsigned syncInterval) const
{
	if(*this) {return;}
	RawData()->Present(syncInterval, 0);
}


// バッファテクスチャの取得
Texture2D&& SwapChain::GetBuffer() const
{
	ID3D11Texture2D* p{nullptr};
	if(FAILED(RawData()->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&p)))
	{
		return Texture2D();
	}

	return Texture2D(p);
}



//***************************************************************************************
// ================================= ShaderObject.h ====================================
//***************************************************************************************

#include "ShaderObject.h"


// ======================= RenderTargetView ===========================



//***************************************************************************************
// =================================== Texture.h ======================================
//***************************************************************************************


