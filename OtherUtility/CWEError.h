////////////////////////////////////////////////////////////
//
//			エラー出力クラス用ヘッダファイル
//
//
//
////////////////////////////////////////////////////////////



#ifndef INCLUDED_CWE_ERROR_H
#define INCLUDED_CWE_ERROR_H



#include <string>


namespace CWE
{

// デバッグ用出力
bool OutputDebugStringF(const wchar_t* formatString, ...);


// 致命的なエラークラス
class FatalError
{
private:
	friend void End();

	bool m_IsFatalError; //致命的エラーが検出されたかどうか
	std::wstring m_ErrorMessage; //致命的エラーの内容


	FatalError();
	~FatalError();
	FatalError(const FatalError&) =delete;
	FatalError& operator=(const FatalError&) =delete;

	// 致命的エラーの有無を取得
	inline bool GetIs() {return m_IsFatalError;}

public:
	static FatalError& GetInstance();

	//致命的エラーの発生を伝える
	void Outbreak(const wchar_t errorMessage[] =nullptr);
	bool NoInfoOutbreak() const;

};


// アサート
#if defined(_DEBUG) || defined(DEBUG)
#define CWE_ASSERT(expression, ...)\
			(!(expression) && CWE::OutputDebugStringF(L"%s(%d): Assert has failed. : ("#expression")\n", __FILEW__, __LINE__, __VA_ARGS__) && CWE::FatalError::GetInstance().NoInfoOutbreak()) 
#else
#define CWE_ASSERT(...)
#endif


}// CWE


#endif