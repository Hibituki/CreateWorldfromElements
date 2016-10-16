#pragma once


#include "D3DAPIs.h"
#include "../OtherUtility/CWEUtility.h"
#include "FwDeclear.h"


// D3D�C���^�[�t�F�C�X�̃��b�p�[�p���N���X
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


// �R�A�����̏�����
bool InitCore();
// �f�o�C�X�����X�g������
bool DeviceLost();


// �X���b�v�`�F�C��
class SwapChain : public D3DWrapper<IDXGISwapChain>
{
public:
	SwapChain() =default;
	SwapChain(SwapChain&& rhs) =default;
	virtual ~SwapChain();
	SwapChain& operator=(SwapChain&& rhs);

	// �����_�����O���ʂ�\��
	void Present(unsigned syncInterval) const;
	// �o�b�t�@�[�e�N�X�`���̎擾
	Texture2D&& GetBuffer() const;

};

// �X���b�v�`�F�C���̃J�X�^���p�L�q
struct SwapChainDesc : public DXGI_SWAP_CHAIN_DESC
{
};



// =================== �C���^�[�t�F�C�X�t�@�N�g���֐� ===============================

// �X���b�v�`�F�C��
SwapChain&& CreateSwapChain(const SwapChainDesc& desc);
// ���_�V�F�[�_�[
VertexShader&& CreateVertexShader(const tchar* file, const InputElementDesc* pDescs, size_t numDescs);
// �W�I���g���V�F�[�_�[
GeometryShader&& CreateGeometryShader();
// �s�N�Z���V�F�[�_�[
PixelShader&& CreatePixelShader();
// �T���v���[�X�e�[�g
SamplerState&& CreateSamplerState();
// �u�����h�X�e�[�g
BlendState&& CreateBlendState();
// ���X�^���C�U�X�e�[�g
RasterizerState&& CreateRasterizerState();
// �[�x�X�e���V���X�e�[�g
DepthStencilState&& CreateDepthStencilState();
// �R���X�^���g�o�b�t�@
ConstantBuffer&& CreateConstantBuffer();
// �����_�[�^�[�Q�b�g�r���[
RenderTargetView&& CreateRenderTargetView();
// �[�x�X�e���V���r���[
DepthStencilView&& CreateDepthStencilView();
// �V�F�[�_�[���\�[�X�r���[
ShaderResourceView&& CreateShaderResourceView();
// 2D�e�N�X�`���[
Texture2D&& CreateTexture2D();


