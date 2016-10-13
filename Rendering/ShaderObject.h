#pragma once


#include "D3DAPIs.h"
#include "../OtherUtility/CWEUtility.h"




// ���̓G�������g�p�̃f�[�^����
enum class InputClassification
{
	VertexData =0,
	InstanceData,
};

// ���X�^���C�U�X�e�[�g�v���Z�b�g
enum class RasterizerStatus
{
	Normal =0,
	NonCull,
	WireFrame,
};


// �T���v���X�e�[�g�v���Z�b�g
enum class SamplerStatus
{
	Liner =0, //���`���
	Point, //��ԂȂ�
	Shadow,
};


// �u�����h�X�e�[�g�v���Z�b�g
enum class BlendStatus
{
	Normal,
};


// �[�x�X�e���V���X�e�[�g�v���Z�b�g
enum class DepthStencilStatus
{
	Normal,
};


// �t�H�[�}�b�g
typedef DXGI_FORMAT Format;


// ���̓G�������g
struct IAInputElement
{
public:
	IAInputElement(const char* semanticName, unsigned semanticIndex, Format format, unsigned inputSlot, unsigned alignedByteOffset, InputClassification inputSlotClass, unsigned instanceDataStepRate);

	void Set(const std::string& semanticName, unsigned semanticIndex, Format format, unsigned inputSlot, unsigned alignedByteOffset, InputClassification inputSlotClass, unsigned instanceDataStepRate);

private:
	D3D11_INPUT_ELEMENT_DESC m_IE;
};


// ���_�V�F�[�_�I�u�W�F�N�g
struct VertexShader
{
public:
	// ���̓��C�A�E�g&���_�V�F�[�_�I�u�W�F�N�g�̍쐬
	template<unsigned numElements>
	bool Create(const tstring& shaderFile, const std::string& functionName, const IAInputElement (&pInputElements)[numElements]);

private:
	Interface_ptr<ID3D11InputLayout> m_pInputLayout;
	Interface_ptr<ID3D11VertexShader> m_pVertexShader;

};


// �W�I���g���V�F�[�_�I�u�W�F�N�g
struct GeometryShader
{
public:
	// �W�I���g���V�F�[�_�I�u�W�F�N�g�̍쐬
	bool Create(const tstring& shaderFile, const std::string& functionName);

private:
	Interface_ptr<ID3D11GeometryShader> m_pGeometryShader;

};


// �s�N�Z���V�F�[�_�I�u�W�F�N�g
struct PixelShader
{
public:
	// �s�N�Z���V�F�[�_�I�u�W�F�N�g�̍쐬
	bool Create(const tstring& shaderFile, const std::string& functionName);

private:
	Interface_ptr<ID3D11PixelShader> m_pPixelShader;

};



// �T���v���[�X�e�[�g�I�u�W�F�N�g
struct SamplerState
{
public:
	// �v���Z�b�g����쐬
	bool Create(SamplerStatus preset);

private:
	Interface_ptr<ID3D11SamplerState> m_pSamplerState;

	// �V���h�E�}�b�v�p�̍쐬
	bool CreateForShadow();
};


// �u�����h�X�e�[�g�I�u�W�F�N�g
struct BlendState
{
public:
	// �v���Z�b�g����쐬
	bool Create(BlendStatus preset);

private:
	Interface_ptr<ID3D11BlendState> m_pBlendState;

};


// ���X�^���C�U�X�e�[�g�I�u�W�F�N�g
struct RasterizerState
{
public:
	// �v���Z�b�g����쐬
	bool Create(RasterizerStatus preset);

private:
	Interface_ptr<ID3D11RasterizerState> m_RasterizerState;

};


// �[�x�X�e���V���X�e�[�g
struct DepthStencilState
{
public:
	// �v���Z�b�g����쐬
	bool Create(DepthStencilStatus preset);

private:
	Interface_ptr<ID3D11DepthStencilState> m_pDepthStencilState;

};


// �R���X�^���g�o�b�t�@�I�u�W�F�N�g
struct ConstantBuffer
{
public:
	// �R���X�^���g�o�b�t�@�̍쐬
	bool Create(size_t bufferSize);

	// �������݊J�n
	bool Map();
	//�������ݏI��
	void Unmap();
	//�T�u���\�[�X�ւ̏�������
	void* const Access();

private:
	Interface_ptr<ID3D11Buffer> m_pConstantBuffer;
	D3D11_MAPPED_SUBRESOURCE m_Resource;
	bool m_Mapped;

};


// �����_�[�^�[�Q�b�g�r���[�I�u�W�F�N�g
struct RenderTargetView
{
private:
	Interface_ptr<ID3D11RenderTargetView> m_pRenderTargetView;

public:
	// �N���A
	void Clear();
};



// �[�x�X�e���V���r���[�L�q�I�u�W�F�N�g
typedef D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc;


// �[�x�X�e���V���r���[�I�u�W�F�N�g
struct DepthStencilView
{
public:
	// �쐬
	template<class tResource>
	bool Create(tResource& resource, const DepthStencilViewDesc& desc);
	// �N���A
	void Clear();

private:
	Interface_ptr<ID3D11DepthStencilView> m_pDepthStencilView;

};


// �V�F�[�_�[���\�[�X�r���[�I�u�W�F�N�g
struct ShaderResourceView
{
public:
	// �쐬
	template<class tResource>
	bool Create(tResource& resource);
	// �N���A
	void Clear();

private:
	Interface_ptr<ID3D11ShaderResourceView> m_pShaderResourceView;
};