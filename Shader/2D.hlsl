

// �萔�o�b�t�@
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

// ���_�V�F�[�_�̓��̓f�[�^
struct VSInput
{
	float3 m_Pos : POSITION; //���_���W
	float2 m_UV : TEXCOORD0; //�e�N�X�`�����W
};


// ���_�V�F�[�_�̏o�̓f�[�^
struct VSOutput
{
	float4 m_Pos : SV_POSITION; //���_���W
	float2 m_UV : TEXCOORD0; //�e�N�X�`�����W
};



// ���_�V�F�[�_�֐�
VSOutput VS(VSInput input)
{
	VSOutput output;
	// ���_���W
    output.m_Pos =float4(input.m_Pos.xy, 0.0f, 1.0f);
    output.m_Pos =mul(output.m_Pos, World);
	output.m_Pos =mul(output.m_Pos, Projection);

	// �e�N�X�`�����W�̎󂯓n��
	output.m_UV =input.m_UV;
	
	// �o��
	return output;
}



// ================================= PixelShader ====================================


// �e�N�X�`��
Texture2D Texture : register(t0);
SamplerState Sampler : register(s0);


// �s�N�Z���V�F�[�_�̓��̓f�[�^
struct PSInput
{
	float4 m_Pos : SV_POSITION; //���_���W
	float2 m_UV : TEXCOORD0; //�e�N�X�`�����W
};


// �s�N�Z���V�F�[�_�̏o�̓f�[�^
struct PSOutput
{
	float4 m_Color : SV_TARGET;
};


// �s�N�Z���V�F�[�_�֐�
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