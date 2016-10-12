#pragma once


#include "D3DAPIs.h"
#include "../OtherUtility/CWEUtility.h"
#include "FwDeclear.h"


// D3Dインターフェイスのラッパー用基底クラス
template<class tRawType>
class D3DWrapper
{
public:
	D3DWrapper(): m_pRawData(nullptr, deleter::Release<tRawType>) {}
	virtual ~D3DWrapper() =default;
	D3DWrapper(const tRawType* newData): m_pRawData(newData, deleter::Release<tRawType>) {}
	inline operator bool() const {return m_pRawData!=nullptr;}
	inline const tRawType* const ReadRawData() const {return m_pRawData.get();}
	inline void SetRawData(const tRawType* newData) {m_pRawData.reset(newData);}

protected:
	inline Interface_ptr<tRawType>& RawData() {return m_pRawData;}
	inline const Interface_ptr<tRawType>& RawData() const {return m_pRawData;}

private:
	Interface_ptr<tRawType> m_pRawData;

};


// デバイスがロストしたか
bool DeviceLost();


// スワップチェイン
class SwapChain : public D3DWrapper<IDXGISwapChain>
{
public:
	SwapChain();
	SwapChain(const Window& target);
	virtual ~SwapChain();

	// スワップチェインの各種初期設定
	bool SetProperty(const Window& target);
	// レンダリング結果を表示
	void Present(unsigned syncInterval) const;
	// バッファーテクスチャの取得
	Texture2D&& GetBuffer() const;

};



// =================== インターフェイスファクトリ関数 ===============================

namespace factory
{
// スワップチェイン
SwapChain&& CreateSwapChain();

// 頂点シェーダー
VertexShader&& CreateVertexShader();

// ジオメトリシェーダー
GeometryShader&& CreateGeometryShader();

// ピクセルシェーダー
PixelShader&& CreatePixelShader();


}//factory