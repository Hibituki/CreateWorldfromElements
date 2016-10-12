////////////////////////////////////////////////////////////
//
//			コーディングサポート
//
//
//
////////////////////////////////////////////////////////////


#pragma once

#include <type_traits>
#include <string>
#include <tchar.h>
#include <memory>


//安全なデバイス管理権限の開放
#define SAFE_UNACQUIRE(device) {if((device)){(device)->Unacquire();}}
//安全なリリース
#define SAFE_RELEASE(ptr) {if((ptr)){(ptr)->Release(); (ptr)=NULL;}}
//安全なデリート
#define SAFE_DELETE(ptr) {if((ptr)){delete(ptr); (ptr)=nullptr;}}
//安全な配列デリート
#define SAFE_DELETE_ARRAY(ptr) {if((ptr)){delete[](ptr); (ptr)=0;}}


// デリーター
namespace deleter
{

// リリースデリーター
template<class tReleaser>
static void Release(tReleaser* p)
{
	p->Release();
}

// 未取得化＆リリースデリーター
template<class tUnaquireReleaser>
static void UnaquireRelease(tUnaquireReleaser* p)
{
	p->Unacquire();
	p->Release();
}

}


// enum型の値を根底型にキャスト
template<class tEnum>
constexpr auto ToUType(tEnum enumerator) noexcept
{
	return static_cast<std::underlying_type_t<tEnum>>(enumerator);
}



typedef unsigned BitFlag;

typedef std::basic_string<TCHAR, std::char_traits<TCHAR>, std::allocator<TCHAR> > tstring;

template<class tInterface>
using Interface_ptr =std::unique_ptr<tInterface, decltype((&deleter::Release<tInterface>))>;