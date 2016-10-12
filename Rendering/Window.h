#pragma once


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "../OtherUtility/CWEUtility.h"


// アプリWindow管理クラス
class Window
{
public:
//	Window();
//	~Window();
//	Window(Window&& lhs);
//	Window& operator=(Window&& lhs);
//	Window(const Window& rhs);
//	Window& operator=(Window& rhs);
	operator bool() {return m_IsEnable;}

	bool Create(unsigned width, unsigned height, const tstring& title);
	void Close();
	void ShowWindow();
	void ProcessMessage();
	HWND Handle() const {return m_Handle;}
	int Width() const;
	int Height() const;

private:
	HINSTANCE m_Instance;
	HWND m_Handle;
	double m_AspectRatio; //Width/Height
	bool m_IsEnable;
	tstring m_Title;

};