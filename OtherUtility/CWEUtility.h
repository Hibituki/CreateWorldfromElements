////////////////////////////////////////////////////////////
//
//			�R�[�f�B���O�T�|�[�g
//
//
//
////////////////////////////////////////////////////////////


#pragma once

#include <type_traits>
#include <string>
#include <tchar.h>
#include <memory>


//���S�ȃf�o�C�X�Ǘ������̊J��
#define SAFE_UNACQUIRE(device) {if((device)){(device)->Unacquire();}}
//���S�ȃ����[�X
#define SAFE_RELEASE(ptr) {if((ptr)){(ptr)->Release(); (ptr)=NULL;}}
//���S�ȃf���[�g
#define SAFE_DELETE(ptr) {if((ptr)){delete(ptr); (ptr)=nullptr;}}
//���S�Ȕz��f���[�g
#define SAFE_DELETE_ARRAY(ptr) {if((ptr)){delete[](ptr); (ptr)=0;}}


// �f���[�^�[
namespace deleter
{

// �����[�X�f���[�^�[
template<class tReleaser>
static void Release(tReleaser* p)
{
	p->Release();
}

// ���擾���������[�X�f���[�^�[
template<class tUnaquireReleaser>
static void UnaquireRelease(tUnaquireReleaser* p)
{
	p->Unacquire();
	p->Release();
}

}


// enum�^�̒l������^�ɃL���X�g
template<class tEnum>
constexpr auto ToUType(tEnum enumerator) noexcept
{
	return static_cast<std::underlying_type_t<tEnum>>(enumerator);
}



typedef unsigned BitFlag;

typedef std::basic_string<TCHAR, std::char_traits<TCHAR>, std::allocator<TCHAR> > tstring;

template<class tInterface>
using Interface_ptr =std::unique_ptr<tInterface, decltype((&deleter::Release<tInterface>))>;