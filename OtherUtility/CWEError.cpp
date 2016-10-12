////////////////////////////////////////////////////////////
//
//			エラー出力クラス用ソースファイル
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


// デバッグ用出力
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

// コンストラクタ
FatalError::FatalError():
	m_IsFatalError(false)
{
}

// デストラクタ
FatalError::~FatalError()
{
	//致命的なエラーが発生していたらメッセージを表示する
	if(m_IsFatalError)
	{
		MessageBox(NULL, m_ErrorMessage.c_str(), L"致命的なエラー", (MB_OK|MB_ICONERROR));
	}
}

// インスタンスの取得
FatalError& FatalError::GetInstance()
{
	static FatalError instance;
	return instance;
}


// 致命的エラーの発生
void FatalError::Outbreak(const wchar_t errorMessage[])
{
	if(errorMessage == nullptr)
	{
		m_ErrorMessage =L"致命的なエラーが発生しました\n処理を継続できません";
	}
	else
	{
		m_ErrorMessage =errorMessage;
	}
	m_IsFatalError =true;

	// ソフトへの終了要求
	CWE::Graphic::GetInstance().m_Windows.EndRequest();
}

bool FatalError::NoInfoOutbreak() const
{
	CWE::Graphic::GetInstance().m_Windows.EndRequest();
	return true;
}




}