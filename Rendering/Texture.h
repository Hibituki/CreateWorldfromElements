#pragma once


#include "D3DAPIs.h"
#include "../OtherUtility/CWEUtility.h"


class Texture2D
{
public:
	Texture2D() =default;
	~Texture2D() =default;
	Texture2D(ID3D11Texture2D* p): m_pResource(p, deleter::Release<ID3D11Texture2D>) {}
	operator bool() const {return m_pResource!=nullptr;}


private:
	Interface_ptr<ID3D11Texture2D> m_pResource;

};