#pragma once


#include "D3DAPIs.h"
#include "../OtherUtility/CWEUtility.h"




// 入力エレメント用のデータ分類
enum class InputClassification
{
	VertexData =0,
	InstanceData,
};

// ラスタライザステートプリセット
enum class RasterizerStatus
{
	Normal =0,
	NonCull,
	WireFrame,
};


// サンプラステートプリセット
enum class SamplerStatus
{
	Liner =0, //線形補間
	Point, //補間なし
	Shadow,
};


// ブレンドステートプリセット
enum class BlendStatus
{
	Normal,
};


// 深度ステンシルステートプリセット
enum class DepthStencilStatus
{
	Normal,
};


// フォーマット
typedef DXGI_FORMAT Format;


// 入力エレメント
struct IAInputElement
{
public:
	IAInputElement(const char* semanticName, unsigned semanticIndex, Format format, unsigned inputSlot, unsigned alignedByteOffset, InputClassification inputSlotClass, unsigned instanceDataStepRate);

	void Set(const std::string& semanticName, unsigned semanticIndex, Format format, unsigned inputSlot, unsigned alignedByteOffset, InputClassification inputSlotClass, unsigned instanceDataStepRate);

private:
	D3D11_INPUT_ELEMENT_DESC m_IE;
};


// 頂点シェーダオブジェクト
struct VertexShader
{
public:
	// 入力レイアウト&頂点シェーダオブジェクトの作成
	template<unsigned numElements>
	bool Create(const tstring& shaderFile, const std::string& functionName, const IAInputElement (&pInputElements)[numElements]);

private:
	Interface_ptr<ID3D11InputLayout> m_pInputLayout;
	Interface_ptr<ID3D11VertexShader> m_pVertexShader;

};


// ジオメトリシェーダオブジェクト
struct GeometryShader
{
public:
	// ジオメトリシェーダオブジェクトの作成
	bool Create(const tstring& shaderFile, const std::string& functionName);

private:
	Interface_ptr<ID3D11GeometryShader> m_pGeometryShader;

};


// ピクセルシェーダオブジェクト
struct PixelShader
{
public:
	// ピクセルシェーダオブジェクトの作成
	bool Create(const tstring& shaderFile, const std::string& functionName);

private:
	Interface_ptr<ID3D11PixelShader> m_pPixelShader;

};



// サンプラーステートオブジェクト
struct SamplerState
{
public:
	// プリセットから作成
	bool Create(SamplerStatus preset);

private:
	Interface_ptr<ID3D11SamplerState> m_pSamplerState;

	// シャドウマップ用の作成
	bool CreateForShadow();
};


// ブレンドステートオブジェクト
struct BlendState
{
public:
	// プリセットから作成
	bool Create(BlendStatus preset);

private:
	Interface_ptr<ID3D11BlendState> m_pBlendState;

};


// ラスタライザステートオブジェクト
struct RasterizerState
{
public:
	// プリセットから作成
	bool Create(RasterizerStatus preset);

private:
	Interface_ptr<ID3D11RasterizerState> m_RasterizerState;

};


// 深度ステンシルステート
struct DepthStencilState
{
public:
	// プリセットから作成
	bool Create(DepthStencilStatus preset);

private:
	Interface_ptr<ID3D11DepthStencilState> m_pDepthStencilState;

};


// コンスタントバッファオブジェクト
struct ConstantBuffer
{
public:
	// コンスタントバッファの作成
	bool Create(size_t bufferSize);

	// 書き込み開始
	bool Map();
	//書き込み終了
	void Unmap();
	//サブリソースへの書き込み
	void* const Access();

private:
	Interface_ptr<ID3D11Buffer> m_pConstantBuffer;
	D3D11_MAPPED_SUBRESOURCE m_Resource;
	bool m_Mapped;

};


// レンダーターゲットビューオブジェクト
struct RenderTargetView
{
private:
	Interface_ptr<ID3D11RenderTargetView> m_pRenderTargetView;

public:
	// クリア
	void Clear();
};



// 深度ステンシルビュー記述オブジェクト
typedef D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc;


// 深度ステンシルビューオブジェクト
struct DepthStencilView
{
public:
	// 作成
	template<class tResource>
	bool Create(tResource& resource, const DepthStencilViewDesc& desc);
	// クリア
	void Clear();

private:
	Interface_ptr<ID3D11DepthStencilView> m_pDepthStencilView;

};


// シェーダーリソースビューオブジェクト
struct ShaderResourceView
{
public:
	// 作成
	template<class tResource>
	bool Create(tResource& resource);
	// クリア
	void Clear();

private:
	Interface_ptr<ID3D11ShaderResourceView> m_pShaderResourceView;
};