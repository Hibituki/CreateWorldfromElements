#pragma once


#include "D3DAPIs.h"
#include "../OtherUtility/CWEUtility.h"
#include "FwDeclear.h"


// D3Dインターフェイスのラッパー用基底クラス
template<class tRawType>
class D3DWrapper
{
public:
	using RawType =tRawType;

	D3DWrapper(): m_pRawData(nullptr, deleter::Release<tRawType>) {}
	virtual ~D3DWrapper() =default;
	D3DWrapper(const tRawType* newData): m_pRawData(newData, deleter::Release<tRawType>) {}
	inline operator bool() const {return m_pRawData!=nullptr;}
	inline D3DWrapper& operator=(D3DWrapper&& rhs) {return m_pRawData =rhs.m_pRawData;}
	inline const tRawType* const ReadRawData() const {return m_pRawData.get();}
	inline void SetRawData(const tRawType* newData) {m_pRawData.reset(newData);}

protected:
	inline Interface_ptr<tRawType>& RawData() {return m_pRawData;}
	inline const Interface_ptr<tRawType>& RawData() const {return m_pRawData;}

private:
	Interface_ptr<tRawType> m_pRawData;

};


// コア部分の初期化
bool InitCore();
// デバイスがロストしたか
bool DeviceLost();


// スワップチェイン
class SwapChain : public D3DWrapper<IDXGISwapChain>
{
public:
	SwapChain() =default;
	SwapChain(SwapChain&& rhs) =default;
	virtual ~SwapChain();
	SwapChain& operator=(SwapChain&& rhs);

	// レンダリング結果を表示
	void Present(unsigned syncInterval) const;
	// バッファーテクスチャの取得
	Texture2D&& GetBuffer() const;

};

// スワップチェインのカスタム用記述
struct SwapChainDesc : public DXGI_SWAP_CHAIN_DESC
{
};



// =================== インターフェイスファクトリ関数 ===============================

// スワップチェイン
SwapChain&& CreateSwapChain(const SwapChainDesc& desc);
// 頂点シェーダー
VertexShader&& CreateVertexShader(const tchar* file, const InputElementDesc* pDescs, size_t numDescs);
// ジオメトリシェーダー
GeometryShader&& CreateGeometryShader();
// ピクセルシェーダー
PixelShader&& CreatePixelShader();
// サンプラーステート
SamplerState&& CreateSamplerState();
// ブレンドステート
BlendState&& CreateBlendState();
// ラスタライザステート
RasterizerState&& CreateRasterizerState();
// 深度ステンシルステート
DepthStencilState&& CreateDepthStencilState();
// コンスタントバッファ
ConstantBuffer&& CreateConstantBuffer();
// レンダーターゲットビュー
RenderTargetView&& CreateRenderTargetView();
// 深度ステンシルビュー
DepthStencilView&& CreateDepthStencilView();
// シェーダーリソースビュー
ShaderResourceView&& CreateShaderResourceView();
// 2Dテクスチャー
Texture2D&& CreateTexture2D();


