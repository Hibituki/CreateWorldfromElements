

// 定数バッファ
cbuffer g_World : register(b0)
{
	matrix World : packoffset(c0);
}

cbuffer g_Projection : register(b1)
{
	matrix Projection : packoffset(c0);
};

cbuffer g_Color : register(b2)
{
	float4 Color : packoffset(c0);
}



// ================================= VertexShader ==============================

// 頂点シェーダの入力データ
struct VSInput
{
	float3 m_Pos : POSITION; //頂点座標
	float2 m_UV : TEXCOORD0; //テクスチャ座標
};


// 頂点シェーダの出力データ
struct VSOutput
{
	float4 m_Pos : SV_POSITION; //頂点座標
	float2 m_UV : TEXCOORD0; //テクスチャ座標
};



// 頂点シェーダ関数
VSOutput VS(VSInput input)
{
	VSOutput output;
	// 頂点座標
    output.m_Pos =float4(input.m_Pos.xy, 0.0f, 1.0f);
    output.m_Pos =mul(output.m_Pos, World);
	output.m_Pos =mul(output.m_Pos, Projection);

	// テクスチャ座標の受け渡し
	output.m_UV =input.m_UV;
	
	// 出力
	return output;
}



// ================================= PixelShader ====================================


// テクスチャ
Texture2D Texture : register(t0);
SamplerState Sampler : register(s0);


// ピクセルシェーダの入力データ
struct PSInput
{
	float4 m_Pos : SV_POSITION; //頂点座標
	float2 m_UV : TEXCOORD0; //テクスチャ座標
};


// ピクセルシェーダの出力データ
struct PSOutput
{
	float4 m_Color : SV_TARGET;
};


// ピクセルシェーダ関数
PSOutput TexturePS(PSInput input)
{
	PSOutput op;
	op.m_Color =Texture.Sample(Sampler, input.m_UV);
	return op;
}

PSOutput ColorPS(PSInput input)
{
	PSOutput op;
	op.m_Color =Color;
	return op;
}