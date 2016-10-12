///////////////////////////////////////////////////////////////////////////////////////////////
//
//							���̓f�o�C�X
//
//////////////////////////////////////////////////////////////////////////////////////////////


#pragma once


#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#include <XInput.h>

#include <memory>
#include <array>
#include <vector>
#include "../OtherUtility/CWEUtility.h"



namespace DeviceClass
{
// �f�o�C�X�̕���
enum DeviceClass : unsigned int
{
	All =DI8DEVCLASS_ALL,
	Other =DI8DEVCLASS_DEVICE,
	GameController =DI8DEVCLASS_GAMECTRL,
	Keyboard =DI8DEVCLASS_KEYBOARD,
	Pointer =DI8DEVCLASS_POINTER,
};
}


namespace EDFL
{
// �񋓂���f�o�C�X�̏��
enum EnumDeviceFlag : unsigned int
{
	All =DIEDFL_ALLDEVICES,
	AttachedOnly =DIEDFL_ATTACHEDONLY,
	ForceFeedback =DIEDFL_FORCEFEEDBACK,
	IncludeAliases =DIEDFL_INCLUDEALIASES,
	IncludeHidden =DIEDFL_INCLUDEHIDDEN,
};
}


// �f�o�C�X�̏��
struct DeviceID : private DIDEVICEINSTANCE
{
	friend GUID GetGUID(const DeviceID& id);

	DeviceID() {dwSize =sizeof(DIDEVICEINSTANCE);}
	DeviceID(const DIDEVICEINSTANCE& lhs): DIDEVICEINSTANCE(lhs) {}
	inline auto GetName() const {return tstring(tszInstanceName);}
};


// GUID���擾
inline GUID GetGUID(const DeviceID& id) {return id.guidInstance;}


// �ڑ�����Ă���̓��̓f�o�C�X��GUID���擾
std::vector<DeviceID> GetDeviceID(BitFlag devClass, BitFlag devScope =EDFL::AttachedOnly);


//�L�[�R�[�h
enum class KeyCode : unsigned char
{
	ESCAPE			=DIK_ESCAPE, //�uEsc�v�L�[
	Key1			=DIK_1, //�u1�v�L�[
	Key2			=DIK_2, //�u2�v�L�[
	Key3			=DIK_3, //�u3�v�L�[
	Key4			=DIK_4, //�u4�v�L�[
	Key5			=DIK_5, //�u5�v�L�[
	Key6			=DIK_6, //�u6�v�L�[
	Key7			=DIK_7, //�u7�v�L�[
	Key8			=DIK_8, //�u8�v�L�[
	Key9			=DIK_9, //�u9�v�L�[
	Key0			=DIK_0, //�u0�v�L�[
	MINUS			=DIK_MINUS,  //�u-�v�L�[
	EQUALS			=DIK_EQUALS, //�u=�v�L�[
	BACKSPACE		=DIK_BACK, //�uBackspace�v�L�[
	TAB				=DIK_TAB, //�uTab�v�L�[
	Q				=DIK_Q, //�uQ�v�L�[
	W				=DIK_W, //�uW�v�L�[
	E				=DIK_E, //�uE�v�L�[
	R				=DIK_R, //�uR�v�L�[
	T				=DIK_T, //�uT�v�L�[
	Y				=DIK_Y, //�uY�v�L�[
	U				=DIK_U, //�uU�v�L�[
	I				=DIK_I, //�uI�v�L�[
	O				=DIK_O, //�uO�v�L�[
	P				=DIK_P, //�uP�v�L�[
	LBRACKET		=DIK_LBRACKET, //�u [ �v�L�[
	RBRACKET		=DIK_RBRACKET, //�u ] �v�L�[
	ENTER			=DIK_RETURN, //�uEnter�v�L�[
	LCONTROL		=DIK_LCONTROL, //�uCtrl�v�L�[�i���j
	A				=DIK_A, //�uA�v�L�[
	S				=DIK_S, //�uS�v�L�[
	D				=DIK_D, //�uD�v�L�[
	F				=DIK_F, //�uF�v�L�[
	G				=DIK_G, //�uG�v�L�[
	H				=DIK_H, //�uH�v�L�[
	J				=DIK_J, //�uJ�v�L�[
	K				=DIK_K, //�uK�v�L�[
	L				=DIK_L, //�uL�v�L�[
	SEMICOLON		=DIK_SEMICOLON, //�u;�v�L�[
	APOSTROPHE		=DIK_APOSTROPHE, //�u ' �v�L�[
	GRAVE			=DIK_GRAVE, //�u ` �v�L�[
	LSHIFT			=DIK_LSHIFT, //�uShift�v�L�[�i���j
	BACKSLASH		=DIK_BACKSLASH, //�u\�v�L�[
	Z				=DIK_Z, //�uZ�v�L�[
	X				=DIK_X, //�uX�v�L�[
	C				=DIK_C, //�uC�v�L�[
	V				=DIK_V, //�uV�v�L�[
	B				=DIK_B, //�uB�v�L�[
	N				=DIK_N, //�uN�v�L�[
	M				=DIK_M, //�uM�v�L�[
	COMMA			=DIK_COMMA, //�u , �v�L�[
	PERIOD			=DIK_PERIOD, //�u . �v�L�[
	SLASH			=DIK_SLASH, //�u/�v�L�[
	RSHIFT			=DIK_RSHIFT, //�uShift�v�L�[�i�E�j
	MULTIPLY		=DIK_MULTIPLY, //�u*�v�L�[�iNumpad�j
	LALT			=DIK_LMENU, //�uAlt�v�L�[�i���j
	SPACE			=DIK_SPACE, //�uSpace�v�L�[
	CAPSLOCK		=DIK_CAPITAL, //�uCaps Lock�v�L�[
	F1				=DIK_F1, //�uF1�v�L�[
	F2				=DIK_F2, //�uF2�v�L�[
	F3				=DIK_F3, //�uF3�v�L�[
	F4				=DIK_F4, //�uF4�v�L�[
	F5				=DIK_F5, //�uF5�v�L�[
	F6				=DIK_F6, //�uF6�v�L�[
	F7				=DIK_F7, //�uF7�v�L�[
	F8				=DIK_F8, //�uF8�v�L�[
	F9				=DIK_F9, //�uF9�v�L�[
	F10				=DIK_F10, //�uF10�v�L�[
	NUMLOCK			=DIK_NUMLOCK, //�uNum Lock�v�L�[
	SCROLL			=DIK_SCROLL, //�uScroll Lock�v�L�[
	NUMPAD7			=DIK_NUMPAD7, //�u7�v�L�[�iNumpad�j
	NUMPAD8			=DIK_NUMPAD8, //�u8�v�L�[�iNumpad�j
	NUMPAD9			=DIK_NUMPAD9, //�u9�v�L�[�iNumpad�j
	SUBTRACT		=DIK_SUBTRACT, //�u-�v�L�[�iNumpad�j
	NUMPAD4			=DIK_NUMPAD4, //�u4�v�L�[�iNumpad�j
	NUMPAD5			=DIK_NUMPAD5, //�u5�v�L�[�iNumpad�j
	NUMPAD6			=DIK_NUMPAD6, //�u6�v�L�[�iNumpad�j
	ADD				=DIK_ADD, //�u+�v�L�[�iNumpad�j
	NUMPAD1			=DIK_NUMPAD1, //�u1�v�L�[�iNumpad�j
	NUMPAD2			=DIK_NUMPAD2, //�u2�v�L�[�iNumpad�j
	NUMPAD3			=DIK_NUMPAD3, //�u3�v�L�[�iNumpad�j
	NUMPAD0			=DIK_NUMPAD0, //�u0�v�L�[�iNumpad�j
	DECIMAL			=DIK_DECIMAL, //�u.�v�L�[�iNumpad�j
	F11				=DIK_F11, //�uF11�v�L�[
	F12				=DIK_F12, //�uF12�v�L�[
	F13				=DIK_F13, //�uF13�v�L�[
	F14				=DIK_F14, //�uF14�v�L�[
	F15				=DIK_F15, //�uF15�v�L�[
	KANA			=DIK_KANA, //�u�J�i�v�L�[�i���{��L�[�{�[�h�j
	CONVERT			=DIK_CONVERT, //�u�ϊ��v�L�[�i���{��L�[�{�[�h�j
	NOCONVERT		=DIK_NOCONVERT, //�u���ϊ��v�L�[�i���{��L�[�{�[�h�j
	YEN				=DIK_YEN, //�u\�v�L�[�i���{��L�[�{�[�h�j
	NUMPADEQUALS	=DIK_NUMPADEQUALS, //�u=�v�L�[�iNumpad�j
	CIRCUMFLEX		=DIK_CIRCUMFLEX, //�u^�v�L�[�i���{��L�[�{�[�h�j
	AT				=DIK_AT, //�u@�v�L�[
	COLON			=DIK_COLON, //�u:�v�L�[
	UNDERLINE		=DIK_UNDERLINE, //�u _ �v�L�[
	KANJI			=DIK_KANJI, //�u���p/�S�p�v�L�[
	STOP			=DIK_STOP, //�uStop�v�L�[
	NUMPADENTER		=DIK_NUMPADENTER, //�uEnter�v�L�[�iNumpad�j
	RCONTROL		=DIK_RCONTROL, //�uCtrl�v�L�[�i�E�j
	NUMPADCOMMA		=DIK_NUMPADCOMMA, //�u , �v�L�[�iNumpad�j
	DIVIDE			=DIK_DIVIDE, //�u/�v�L�[�iNumpad�j
	SYSRQ			=DIK_SYSRQ, //�uSys Rq�v�L�[
	RALT			=DIK_RMENU, //�uAlt�v�L�[�i�E�j
	PAUSE			=DIK_PAUSE, //�uPause�v�L�[
	HOME			=DIK_HOME, //�uHome�v�L�[
	UP				=DIK_UP, //�u���v�L�[
	PAGEUP			=DIK_PRIOR, //�uPage Up�v�L�[
	LEFT			=DIK_LEFT, //�u���v�L�[
	RIGHT			=DIK_RIGHT, //�u���v�L�[
	END				=DIK_END, //�uEnd�v�L�[
	DOWN			=DIK_DOWN, //�u���v�L�[
	PAGEDOWN		=DIK_NEXT, //�uPage Down�v�L�[
	INSERT			=DIK_INSERT, //�uInsert�v�L�[
	DELETE_			=DIK_DELETE, //�uDelete�v�L�[
	LWIN			=DIK_LWIN, //�uWindows�v�L�[�i���j
	RWIN			=DIK_RWIN, //�uWindows�v�L�[�i�E�j
	APPS			=DIK_APPS, //�uMenu�v�L�[
	POWER			=DIK_POWER //�uPower�v�L�[
};


// �L�[�{�[�h
class Keyboard
{
public:
	Keyboard();
	~Keyboard();
	Keyboard(const DeviceID& id);
	Keyboard(Keyboard&& rhs);
	Keyboard& operator=(Keyboard&& rhs);
	operator bool() const;

	void Update();
	inline const std::array<unsigned char, 256>& GetState() const noexcept {return m_KeyState;}
	inline bool Key(KeyCode code) const noexcept {return (m_KeyState[ToUType(code)] & 0x80) != 0;}
	inline auto Reset() {return Reset(GUID_SysKeyboard);}
	inline auto Reset(const DeviceID& id) {return Reset(GetGUID(id));}

private:
	std::unique_ptr<IDirectInputDevice8, decltype(&deleter::UnaquireRelease)> m_pDevice;
	std::array<unsigned char, 256> m_KeyState;

	bool Reset(const GUID& guid);

};


// �}�E�X�{�^�����͗p�}�X�N
enum class MouseButton : unsigned char
{
	Left =0,
	Right =1,
	Middle =2,
};


// �}�E�X
class Mouse
{
public:
	struct State
	{
		int m_X,
			m_Y,
			m_Z;
		std::array<unsigned char, 8> m_Button;
	};

	Mouse();
	~Mouse();
	Mouse(const DeviceID& id);
	Mouse(Mouse&& rhs);
	Mouse& operator=(Mouse&& rhs);
	operator bool() const;

	void Update();
	inline const State& GetState() const noexcept {return m_State;}
	inline bool Button(MouseButton button) const noexcept {return (m_State.m_Button[ToUType(button)] & 0x80) != 0;}
	inline auto Reset() {return Reset(GUID_SysMouse);}
	inline auto Reset(const DeviceID& id) {return Reset(GetGUID(id));}

private:
	std::unique_ptr<IDirectInputDevice8, decltype(&deleter::UnaquireRelease)> m_pDevice;
	State m_State;

	bool Reset(const GUID& guid);

};


// XInput�{�^���p�}�X�N
enum class XIButton : unsigned int
{
	DPad_Up =XINPUT_GAMEPAD_DPAD_UP,
	DPad_Down =XINPUT_GAMEPAD_DPAD_DOWN,
	DPad_Left =XINPUT_GAMEPAD_DPAD_LEFT,
	DPad_Right =XINPUT_GAMEPAD_DPAD_RIGHT,
	Start =XINPUT_GAMEPAD_START,
	Back =XINPUT_GAMEPAD_BACK,
	Left_Thumb =XINPUT_GAMEPAD_LEFT_THUMB,
	Right_Thumb =XINPUT_GAMEPAD_RIGHT_THUMB,
	Left_Shoulder =XINPUT_GAMEPAD_LEFT_SHOULDER,
	Right_Shoulder =XINPUT_GAMEPAD_RIGHT_SHOULDER,
	A =XINPUT_GAMEPAD_A,
	B =XINPUT_GAMEPAD_B,
	X =XINPUT_GAMEPAD_X,
	Y =XINPUT_GAMEPAD_Y,
};


// XInput�R���g���[���[
class XInput
{
public:
	enum class Number : unsigned
	{
		Controller1 =0,
		Controller2,
		Controller3,
		Controller4,
	};

	struct State
	{
		unsigned char m_LeftTrigger;
		unsigned char m_RightTrigger;
		short m_ThumbLX;
		short m_ThumbLY;
		short m_ThumbRX;
		short m_ThumbRY;
	};

	XInput();
	~XInput();
	XInput(Number number);
	inline operator bool() const {return (-1 != m_Number);}

	void Update();
	inline bool Button(XIButton button) const noexcept {return (m_State.Gamepad.wButtons & ToUType(button))!=0;}
	inline bool Reset(Number number) {return (XInputGetState(ToUType(number), &m_State) == ERROR_SUCCESS);}
	inline const State& GetState() const {return m_State.Gamepad;}
	

private:
	XINPUT_STATE m_State;
	int m_Number;

};