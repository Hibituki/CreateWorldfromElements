#pragma once


#include "D3DAPIs.h"
#include "../OtherUtility/CWEUtility.h"
#include "FwDeclear.h"


// D3D�C���^�[�t�F�C�X�̃��b�p�[�p���N���X
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


// �f�o�C�X�����X�g������
bool DeviceLost();


// �X���b�v�`�F�C��
class SwapChain : public D3DWrapper<IDXGISwapChain>
{
public:
	SwapChain();
	SwapChain(const Window& target);
	virtual ~SwapChain();

	// �X���b�v�`�F�C���̊e�평���ݒ�
	bool SetProperty(const Window& target);
	// �����_�����O���ʂ�\��
	void Present(unsigned syncInterval) const;
	// �o�b�t�@�[�e�N�X�`���̎擾
	Texture2D&& GetBuffer() const;

};



// =================== �C���^�[�t�F�C�X�t�@�N�g���֐� ===============================

namespace factory
{
// �X���b�v�`�F�C��
SwapChain&& CreateSwapChain();

// ���_�V�F�[�_�[
VertexShader&& CreateVertexShader();

// �W�I���g���V�F�[�_�[
GeometryShader&& CreateGeometryShader();

// �s�N�Z���V�F�[�_�[
PixelShader&& CreatePixelShader();


}//factory