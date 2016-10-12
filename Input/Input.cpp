#include "Input.h"


// 入力デバイスのコア
std::unique_ptr<IDirectInput8, decltype(&deleter::Release<IDirectInput8>)> g_pInputCoreDevice(nullptr, deleter::Release<IDirectInput8>);

// 入力デバイスのコアの使用準備
bool InputPre()
{
	if(!g_pInputCoreDevice)
	{
		IDirectInput8* p =nullptr;
		if(FAILED(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&p, NULL)))
		{
			return false;
		}
		g_pInputCoreDevice.reset(p);
	}
	return true;
}


// デバイス取得用コールバック関数
BOOL CALLBACK EnumDevicesCallback(const DIDEVICEINSTANCE* pDIDInstance, VOID* pContext)
{
	std::vector<DeviceID>* pDevicesID =(std::vector<DeviceID>*)pContext;

	pDevicesID->push_back(*pDIDInstance);

	return DIENUM_CONTINUE;
}

// 接続されているの入力デバイスのGUIDを取得
std::vector<DeviceID> GetDeviceID(BitFlag devClass, BitFlag devScope)
{
	std::vector<DeviceID> result;

	if(!InputPre()) {return result;}

	g_pInputCoreDevice->EnumDevices(devClass, EnumDevicesCallback, (void*)&result, devScope);

	return result;
}




// ================================== Keyboard =========================================

// コンストラクタ
Keyboard::Keyboard():
	m_pDevice(nullptr, deleter::UnaquireRelease),
	m_KeyState()
{
	Reset();
}

// デストラクタ
Keyboard::~Keyboard()
{

}

// 指定されたデバイス用に生成
Keyboard::Keyboard(const DeviceID& id):
	m_pDevice(nullptr, deleter::UnaquireRelease),
	m_KeyState()
{
	Reset(id);
}

// ムーブコンストラクタ
Keyboard::Keyboard(Keyboard&& rhs):
	m_pDevice(std::move(rhs.m_pDevice)),
	m_KeyState(rhs.m_KeyState)
{
}

// ムーブ代入演算子
Keyboard& Keyboard::operator=(Keyboard&& rhs)
{
	m_pDevice =std::move(rhs.m_pDevice);
	m_KeyState =rhs.m_KeyState;
	return *this;
}

// bool演算子
Keyboard::operator bool() const
{
	bool result =false;

	result =(m_pDevice && true);

	return result;
}


// 状態配列を更新
void Keyboard::Update()
{
	if(FAILED(m_pDevice->GetDeviceState(256, &m_KeyState)))
	{
		m_pDevice->Acquire();
		m_pDevice->GetDeviceState(256, &m_KeyState);
	}
}


// 初期化
bool Keyboard::Reset(const GUID& guid)
{
	m_pDevice.~unique_ptr();

	if(!InputPre()) {return false;}

	IDirectInputDevice8* p =nullptr;
	if(FAILED(g_pInputCoreDevice->CreateDevice(guid, &p, NULL)))
	{
		return false;
	}

	//初期化の適用
	m_pDevice.reset(p);

	if(FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		m_pDevice.~unique_ptr();
		return false;
	}
	//アクセス権の決定
	//TODO: 追加設定できるようにする
	if(FAILED(m_pDevice->SetCooperativeLevel(NULL, (DISCL_BACKGROUND|DISCL_NONEXCLUSIVE))))
	{
		m_pDevice.~unique_ptr();
		return false;
	}

	return true;
}



// ================================== Mouse =========================================

// コンストラクタ
Mouse::Mouse():
	m_pDevice(nullptr, deleter::UnaquireRelease),
	m_State()
{
	Reset();
}

// デストラクタ
Mouse::~Mouse()
{

}

// 指定されたデバイス用に生成
Mouse::Mouse(const DeviceID& id):
	m_pDevice(nullptr, deleter::UnaquireRelease),
	m_State()
{
	Reset(id);
}


// ムーブコンストラクタ
Mouse::Mouse(Mouse&& rhs):
	m_pDevice(std::move(rhs.m_pDevice)),
	m_State(rhs.m_State)
{
	
}

// ムーブ代入演算子
Mouse& Mouse::operator=(Mouse&& rhs)
{
	m_pDevice =std::move(rhs.m_pDevice);
	m_State =rhs.m_State;
	return *this;
}

// bool演算子
Mouse::operator bool() const
{
	bool result =false;

	result =(m_pDevice && true);

	return result;
}


// 状態オブジェクトを更新
void Mouse::Update()
{
	if(FAILED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &m_State)))
	{
		m_pDevice->Acquire();
		m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &m_State);
	}
}


// 初期化
bool Mouse::Reset(const GUID& guid)
{
	m_pDevice.~unique_ptr();

	if(!InputPre()) {return false;}

	IDirectInputDevice8* p =nullptr;
	if(FAILED(g_pInputCoreDevice->CreateDevice(guid, &p, NULL)))
	{
		return false;
	}

	//初期化の適用
	m_pDevice.reset(p);

	if(FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse2)))
	{
		m_pDevice.~unique_ptr();
		return false;
	}
	//アクセス権の決定
	//TODO: 追加設定できるようにする
	if(FAILED(m_pDevice->SetCooperativeLevel(NULL, (DISCL_BACKGROUND|DISCL_NONEXCLUSIVE))))
	{
		m_pDevice.~unique_ptr();
		return false;
	}

	//マウスのヘッダー
	DIPROPDWORD aDIProp;
	aDIProp.diph.dwSize =sizeof(DIPROPDWORD);
	aDIProp.diph.dwHeaderSize =sizeof(DIPROPHEADER);
	aDIProp.diph.dwObj =0;
	aDIProp.diph.dwHow =DIPH_DEVICE;
	aDIProp.dwData =DIPROPAXISMODE_REL;
	if(FAILED(m_pDevice->SetProperty(DIPROP_AXISMODE, &aDIProp.diph)))
	{
		m_pDevice.~unique_ptr();
		return false;
	}

	return true;
}



// =================================== XInput ======================================

// コンストラクタ
XInput::XInput():
	m_Number(-1)
{
	if(XInputGetState(ToUType(Number::Controller1), &m_State) == ERROR_SUCCESS)
	{
		m_Number =0;
	}
}

// デストラクタ
XInput::~XInput()
{

}

// コントローラ－番号指定コンストラクタ
XInput::XInput(XInput::Number number):
	m_Number(-1)
{
	if(XInputGetState(ToUType(number), &m_State) == ERROR_SUCCESS)
	{
		m_Number =0;
	}
}


// 更新処理
void XInput::Update()
{
	if(*this)
	{
		XInputGetState(m_Number, &m_State);
	}
}