////////////////////////////////////////////////////////////
//
//			�G���[�o�̓N���X�p�\�[�X�t�@�C��
//
//
//
////////////////////////////////////////////////////////////





#include "CWEError.h"
#include "CWEGraphicManager.h"
#include "CWEIncludedAPI.h"
#include <tchar.h>



namespace CWE
{


// �f�o�b�O�p�o��
bool OutputDebugStringF(const wchar_t* formatString, ...)
{
	va_list argl;
	wchar_t buf[512];
	va_start(argl, formatString);
	_vstprintf_s(buf, 512, formatString, argl);
	va_end(argl);
	OutputDebugString(buf);
	return true;
}



// ===================== FatalError ========================

// �R���X�g���N�^
FatalError::FatalError():
	m_IsFatalError(false)
{
}

// �f�X�g���N�^
FatalError::~FatalError()
{
	//�v���I�ȃG���[���������Ă����烁�b�Z�[�W��\������
	if(m_IsFatalError)
	{
		MessageBox(NULL, m_ErrorMessage.c_str(), L"�v���I�ȃG���[", (MB_OK|MB_ICONERROR));
	}
}

// �C���X�^���X�̎擾
FatalError& FatalError::GetInstance()
{
	static FatalError instance;
	return instance;
}


// �v���I�G���[�̔���
void FatalError::Outbreak(const wchar_t errorMessage[])
{
	if(errorMessage == nullptr)
	{
		m_ErrorMessage =L"�v���I�ȃG���[���������܂���\n�������p���ł��܂���";
	}
	else
	{
		m_ErrorMessage =errorMessage;
	}
	m_IsFatalError =true;

	// �\�t�g�ւ̏I���v��
	CWE::Graphic::GetInstance().m_Windows.EndRequest();
}

bool FatalError::NoInfoOutbreak() const
{
	CWE::Graphic::GetInstance().m_Windows.EndRequest();
	return true;
}




}