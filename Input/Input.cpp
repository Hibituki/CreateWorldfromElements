#include "Input.h"


// ���̓f�o�C�X�̃R�A
std::unique_ptr<IDirectInput8, decltype(&deleter::Release<IDirectInput8>)> g_pInputCoreDevice(nullptr, deleter::Release<IDirectInput8>);

// ���̓f�o�C�X�̃R�A�̎g�p����
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


// �f�o�C�X�擾�p�R�[���o�b�N�֐�
BOOL CALLBACK EnumDevicesCallback(const DIDEVICEINSTANCE* pDIDInstance, VOID* pContext)
{
	std::vector<DeviceID>* pDevicesID =(std::vector<DeviceID>*)pContext;

	pDevicesID->push_back(*pDIDInstance);

	return DIENUM_CONTINUE;
}

// �ڑ�����Ă���̓��̓f�o�C�X��GUID���擾
std::vector<DeviceID> GetDeviceID(BitFlag devClass, BitFlag devScope)
{
	std::vector<DeviceID> result;

	if(!InputPre()) {return result;}

	g_pInputCoreDevice->EnumDevices(devClass, EnumDevicesCallback, (void*)&result, devScope);

	return result;
}




// ================================== Keyboard =========================================

// �R���X�g���N�^
Keyboard::Keyboard():
	m_pDevice(nullptr, deleter::UnaquireRelease),
	m_KeyState()
{
	Reset();
}

// �f�X�g���N�^
Keyboard::~Keyboard()
{

}

// �w�肳�ꂽ�f�o�C�X�p�ɐ���
Keyboard::Keyboard(const DeviceID& id):
	m_pDevice(nullptr, deleter::UnaquireRelease),
	m_KeyState()
{
	Reset(id);
}

// ���[�u�R���X�g���N�^
Keyboard::Keyboard(Keyboard&& rhs):
	m_pDevice(std::move(rhs.m_pDevice)),
	m_KeyState(rhs.m_KeyState)
{
}

// ���[�u������Z�q
Keyboard& Keyboard::operator=(Keyboard&& rhs)
{
	m_pDevice =std::move(rhs.m_pDevice);
	m_KeyState =rhs.m_KeyState;
	return *this;
}

// bool���Z�q
Keyboard::operator bool() const
{
	bool result =false;

	result =(m_pDevice && true);

	return result;
}


// ��Ԕz����X�V
void Keyboard::Update()
{
	if(FAILED(m_pDevice->GetDeviceState(256, &m_KeyState)))
	{
		m_pDevice->Acquire();
		m_pDevice->GetDeviceState(256, &m_KeyState);
	}
}


// ������
bool Keyboard::Reset(const GUID& guid)
{
	m_pDevice.~unique_ptr();

	if(!InputPre()) {return false;}

	IDirectInputDevice8* p =nullptr;
	if(FAILED(g_pInputCoreDevice->CreateDevice(guid, &p, NULL)))
	{
		return false;
	}

	//�������̓K�p
	m_pDevice.reset(p);

	if(FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		m_pDevice.~unique_ptr();
		return false;
	}
	//�A�N�Z�X���̌���
	//TODO: �ǉ��ݒ�ł���悤�ɂ���
	if(FAILED(m_pDevice->SetCooperativeLevel(NULL, (DISCL_BACKGROUND|DISCL_NONEXCLUSIVE))))
	{
		m_pDevice.~unique_ptr();
		return false;
	}

	return true;
}



// ================================== Mouse =========================================

// �R���X�g���N�^
Mouse::Mouse():
	m_pDevice(nullptr, deleter::UnaquireRelease),
	m_State()
{
	Reset();
}

// �f�X�g���N�^
Mouse::~Mouse()
{

}

// �w�肳�ꂽ�f�o�C�X�p�ɐ���
Mouse::Mouse(const DeviceID& id):
	m_pDevice(nullptr, deleter::UnaquireRelease),
	m_State()
{
	Reset(id);
}


// ���[�u�R���X�g���N�^
Mouse::Mouse(Mouse&& rhs):
	m_pDevice(std::move(rhs.m_pDevice)),
	m_State(rhs.m_State)
{
	
}

// ���[�u������Z�q
Mouse& Mouse::operator=(Mouse&& rhs)
{
	m_pDevice =std::move(rhs.m_pDevice);
	m_State =rhs.m_State;
	return *this;
}

// bool���Z�q
Mouse::operator bool() const
{
	bool result =false;

	result =(m_pDevice && true);

	return result;
}


// ��ԃI�u�W�F�N�g���X�V
void Mouse::Update()
{
	if(FAILED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &m_State)))
	{
		m_pDevice->Acquire();
		m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &m_State);
	}
}


// ������
bool Mouse::Reset(const GUID& guid)
{
	m_pDevice.~unique_ptr();

	if(!InputPre()) {return false;}

	IDirectInputDevice8* p =nullptr;
	if(FAILED(g_pInputCoreDevice->CreateDevice(guid, &p, NULL)))
	{
		return false;
	}

	//�������̓K�p
	m_pDevice.reset(p);

	if(FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse2)))
	{
		m_pDevice.~unique_ptr();
		return false;
	}
	//�A�N�Z�X���̌���
	//TODO: �ǉ��ݒ�ł���悤�ɂ���
	if(FAILED(m_pDevice->SetCooperativeLevel(NULL, (DISCL_BACKGROUND|DISCL_NONEXCLUSIVE))))
	{
		m_pDevice.~unique_ptr();
		return false;
	}

	//�}�E�X�̃w�b�_�[
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

// �R���X�g���N�^
XInput::XInput():
	m_Number(-1)
{
	if(XInputGetState(ToUType(Number::Controller1), &m_State) == ERROR_SUCCESS)
	{
		m_Number =0;
	}
}

// �f�X�g���N�^
XInput::~XInput()
{

}

// �R���g���[���|�ԍ��w��R���X�g���N�^
XInput::XInput(XInput::Number number):
	m_Number(-1)
{
	if(XInputGetState(ToUType(number), &m_State) == ERROR_SUCCESS)
	{
		m_Number =0;
	}
}


// �X�V����
void XInput::Update()
{
	if(*this)
	{
		XInputGetState(m_Number, &m_State);
	}
}