#include "Window.h"
#include <memory>



// デフォルトコールバック関数
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Window* p =reinterpret_cast<Window*>(GetWindowLong(hWnd, GWL_USERDATA));

	if(p && *p)
	{
		switch(msg)
		{
			//ウィンドウが破棄されたとき
		case WM_DESTROY:
			p->Close();
			break;
		}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
 }


// ========================== Windows ===================================



// ウィンドウ生成
bool Window::Create(unsigned width, unsigned height, const tstring& title)
{
	m_Instance =GetModuleHandle(NULL);
	m_Title =title;
	m_AspectRatio =(double)width / (double)height;

	WNDCLASSEX wcx;
	wcx.cbSize =sizeof(wcx);
	wcx.style =(CS_HREDRAW | CS_VREDRAW);
	wcx.lpfnWndProc =WndProc;
	wcx.cbClsExtra =0;
	wcx.cbWndExtra =0;
	wcx.hInstance =m_Instance;
	wcx.hIcon =static_cast<HICON>(LoadImage(m_Instance, IDI_APPLICATION, IMAGE_ICON, 0, 0, (LR_DEFAULTSIZE|LR_SHARED)));
	wcx.hCursor =static_cast<HCURSOR>(LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, (LR_DEFAULTSIZE|LR_SHARED)));
	wcx.hbrBackground =static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wcx.lpszMenuName =NULL;
	wcx.lpszClassName =title.c_str();
	wcx.hIconSm =wcx.hIcon;

	if(RegisterClassEx(&wcx) == 0)
	{
		m_IsEnable =false;
		return false;
	}

	//ウインドウの作成
	m_Handle =CreateWindow(
		title.c_str(),
		title.c_str(),
		WS_OVERLAPPEDWINDOW, 
		0, 
		0, 
		width, 
		height,
		NULL,
		NULL, 
		m_Instance, 
		NULL);

	if(m_Handle == NULL)
	{
		m_IsEnable =false;
		return false;
	}

	//ウィンドウサイズの調整
	RECT client;
	GetClientRect(m_Handle, &client);
	MoveWindow(m_Handle, 0, 0, width+(width-client.right), height+(height-client.bottom), TRUE);

	UpdateWindow(m_Handle);

	m_IsEnable =true;
	return true;
}


// このウィンドウの終了
void Window::Close()
{
	m_IsEnable =false;
	DestroyWindow(m_Handle);
	UnregisterClass(m_Title.c_str(), m_Instance);
}


// ウィンドウの表示
void Window::ShowWindow()
{
	::ShowWindow(m_Handle, SW_SHOW);
}


// メッセージの処理
void Window::ProcessMessage()
{
	MSG message;
	SetWindowLongPtr(m_Handle, GWLP_USERDATA, reinterpret_cast<LONG>(this));
	if(PeekMessage(&message, m_Handle, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&message);//メッセージを変換
		DispatchMessage(&message);//メッセージを送出
	}
}


// ウィンドウのクライアント領域の幅
int Window::Width() const
{
	RECT rc{};
	GetClientRect(m_Handle, &rc);

	return rc.right;
}


// ウィンドウのクライアント領域の高さ
int Window::Height() const
{
	RECT rc{};
	GetClientRect(m_Handle, &rc);

	return rc.bottom;
}