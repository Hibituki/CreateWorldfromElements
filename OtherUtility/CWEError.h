////////////////////////////////////////////////////////////
//
//			�G���[�o�̓N���X�p�w�b�_�t�@�C��
//
//
//
////////////////////////////////////////////////////////////



#ifndef INCLUDED_CWE_ERROR_H
#define INCLUDED_CWE_ERROR_H



#include <string>


namespace CWE
{

// �f�o�b�O�p�o��
bool OutputDebugStringF(const wchar_t* formatString, ...);


// �v���I�ȃG���[�N���X
class FatalError
{
private:
	friend void End();

	bool m_IsFatalError; //�v���I�G���[�����o���ꂽ���ǂ���
	std::wstring m_ErrorMessage; //�v���I�G���[�̓��e


	FatalError();
	~FatalError();
	FatalError(const FatalError&) =delete;
	FatalError& operator=(const FatalError&) =delete;

	// �v���I�G���[�̗L�����擾
	inline bool GetIs() {return m_IsFatalError;}

public:
	static FatalError& GetInstance();

	//�v���I�G���[�̔�����`����
	void Outbreak(const wchar_t errorMessage[] =nullptr);
	bool NoInfoOutbreak() const;

};


// �A�T�[�g
#if defined(_DEBUG) || defined(DEBUG)
#define CWE_ASSERT(expression, ...)\
			(!(expression) && CWE::OutputDebugStringF(L"%s(%d): Assert has failed. : ("#expression")\n", __FILEW__, __LINE__, __VA_ARGS__) && CWE::FatalError::GetInstance().NoInfoOutbreak()) 
#else
#define CWE_ASSERT(...)
#endif


}// CWE


#endif