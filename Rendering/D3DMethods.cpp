


#include "D3DAPIs.h"
#include "../OtherUtility/CWEUtility.h"


// ��v�C���^�[�t�F�C�X
Interface_ptr<ID3D11Device> g_pDevice(nullptr, deleter::Release<ID3D11Device>);
Interface_ptr<ID3D11DeviceContext> g_pDeviceContext(nullptr, deleter::Release<ID3D11DeviceContext>);
Interface_ptr<IDXGIFactory> g_pFactory(nullptr, deleter::Release<IDXGIFactory>);

// �`��f�o�C�X�̃R�A�̎g�p����
bool RenderingPre()
{
	//�t�@�N�g���̊m�F
	if(!g_pFactory)
	{
		IDXGIFactory* pFactory =nullptr;
		if(FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&pFactory))))
		{
			return false;
		}

		g_pFactory.reset(pFactory);
	}

	//�f�o�C�X�ƃf�o�C�X�R���e�L�X�g�̊m�F
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


// �f�o�C�X�����X�g������
bool DeviceLost()
{
	if(RenderingPre()) {return false;}

	if(g_pDevice->GetDeviceRemovedReason() != S_OK) {return true;}
	
	return false;
}



// ======================= SwapChain ================================

// �f�t�H���g�f�X�g���N�^
SwapChain::~SwapChain()
{
	if(*this)
	{
		RawData()->SetFullscreenState(FALSE, nullptr);
	}
}


// ���[�u������Z�q
SwapChain& SwapChain::operator=(SwapChain&& rhs)
{
	RawData().swap(rhs.RawData());
}


// ���ꂢ��Ȃ����ǁA�f�t�H���g�쐬���̎Q�l�ɂ�����Ǝ���Ēu���܂��B
// �悤�悤����ɁA���̐l��
bool SwapChain::SetProperty(const Window& target)
{

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


// �����_�����O���ʂ�\��
void SwapChain::Present(unsigned syncInterval) const
{
	if(*this) {return;}
	RawData()->Present(syncInterval, 0);
}


// �o�b�t�@�e�N�X�`���̎擾
Texture2D&& SwapChain::GetBuffer() const
{
	ID3D11Texture2D* p{nullptr};
	if(FAILED(RawData()->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&p)))
	{
		return Texture2D();
	}

	return Texture2D(p);
}



// ======================= �t�@�N�g���֐� =============================

// �X���b�v�`�F�C��
SwapChain&& CreateSwapChain(const SwapChainDesc& desc)
{
	if(RenderingPre()) {return;}
	IDXGISwapChain* p{};
	g_pFactory->CreateSwapChain(g_pDevice.get(), &static_cast<DXGI_SWAP_CHAIN_DESC>(desc), &p);
	SwapChain result;
	result.SetRawData(p);
	return ::std::move(result);
}


#include <fstream>
// ���_�V�F�[�_�[
VertexShader&& CreateVertexShader(const tchar* file)
{
	unsigned fSize{};
	using fst =::std::fstream;
	::std::basic_ifstream<unsigned char> iFile{file, fst::binary|fst::in};
	iFile.seekg(0, fst::end);
	fSize =iFile.tellg();
	iFile.clear();
	iFile.seekg(0, fst::beg);
	fSize -=iFile.tellg();
	std::unique_ptr<unsigned char[]> buf =std::make_unique<unsigned char[]>(fSize);
	iFile.read(buf.get(), fSize);
	iFile.close();
	//�V�F�[�_�[�R�[�h�f�[�^�̎擾���֐���

	//����������̓��C�A�E�g�ƒ��_�V�F�[�_�[���쐬


	//���_�V�F�[�_�R�[�h�̃R���p�C��
	HRESULT hr;
	if(pShaderMacros) {D3DX11CompileFromFile(shaderFile, &pShaderMacros[0].m_ShaderMacro, NULL, functionName, "vs_5_0", compileOption, NULL, NULL, &pBlob, NULL, &hr);}
	else {D3DX11CompileFromFile(shaderFile, NULL, NULL, functionName, "vs_5_0", compileOption, NULL, NULL, &pBlob, NULL, &hr);}
	if(FAILED(hr))
	{
		FatalError::GetInstance().Outbreak(L"���_�V�F�[�_�̍쐬�Ɏ��s���܂���\n�t�@�C�����j�����Ă���\��������܂�");
		return false;
	}
	//���_�V�F�[�_�̍쐬
	if(FAILED(master.m_pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pVertexShader)))
	{
		//�J�����Ă���I��
		SAFE_RELEASE(pBlob);
		FatalError::GetInstance().Outbreak(L"���_�V�F�[�_�̍쐬�Ɏ��s���܂���\n�t�@�C�����j�����Ă���\��������܂�");
		return false;
	}

	//���̓��C�A�E�g�I�u�W�F�N�g�̍쐬
	if(!pInputElements) {return false;}
	if(FAILED(master.m_pDevice->CreateInputLayout(&pInputElements[0].m_IE, numElements, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &m_pInputLayout)))
	{
		//�J�����Ă���I��
		SAFE_RELEASE(pBlob);
		FatalError::GetInstance().Outbreak(L"���̓��C�A�E�g�I�u�W�F�N�g�̍쐬�Ɏ��s���܂���\n�������p���ł��܂���");
		return false;
	}
	//���_�V�F�[�_�I�u�W�F�N�g�̍쐬�I��
	SAFE_RELEASE(pBlob);
}



//***************************************************************************************
// ================================= ShaderObject.h ====================================
//***************************************************************************************

#include "ShaderObject.h"


// ======================= RenderTargetView ===========================



//***************************************************************************************
// =================================== Texture.h ======================================
//***************************************************************************************


