///////////////////////////////////////////////////////////////////////////////////////////////
//
//							入力デバイス
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
// デバイスの分類
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
// 列挙するデバイスの状態
enum EnumDeviceFlag : unsigned int
{
	All =DIEDFL_ALLDEVICES,
	AttachedOnly =DIEDFL_ATTACHEDONLY,
	ForceFeedback =DIEDFL_FORCEFEEDBACK,
	IncludeAliases =DIEDFL_INCLUDEALIASES,
	IncludeHidden =DIEDFL_INCLUDEHIDDEN,
};
}


// デバイスの情報
struct DeviceID : private DIDEVICEINSTANCE
{
	friend GUID GetGUID(const DeviceID& id);

	DeviceID() {dwSize =sizeof(DIDEVICEINSTANCE);}
	DeviceID(const DIDEVICEINSTANCE& lhs): DIDEVICEINSTANCE(lhs) {}
	inline auto GetName() const {return tstring(tszInstanceName);}
};


// GUIDを取得
inline GUID GetGUID(const DeviceID& id) {return id.guidInstance;}


// 接続されているの入力デバイスのGUIDを取得
std::vector<DeviceID> GetDeviceID(BitFlag devClass, BitFlag devScope =EDFL::AttachedOnly);


//キーコード
enum class KeyCode : unsigned char
{
	ESCAPE			=DIK_ESCAPE, //「Esc」キー
	Key1			=DIK_1, //「1」キー
	Key2			=DIK_2, //「2」キー
	Key3			=DIK_3, //「3」キー
	Key4			=DIK_4, //「4」キー
	Key5			=DIK_5, //「5」キー
	Key6			=DIK_6, //「6」キー
	Key7			=DIK_7, //「7」キー
	Key8			=DIK_8, //「8」キー
	Key9			=DIK_9, //「9」キー
	Key0			=DIK_0, //「0」キー
	MINUS			=DIK_MINUS,  //「-」キー
	EQUALS			=DIK_EQUALS, //「=」キー
	BACKSPACE		=DIK_BACK, //「Backspace」キー
	TAB				=DIK_TAB, //「Tab」キー
	Q				=DIK_Q, //「Q」キー
	W				=DIK_W, //「W」キー
	E				=DIK_E, //「E」キー
	R				=DIK_R, //「R」キー
	T				=DIK_T, //「T」キー
	Y				=DIK_Y, //「Y」キー
	U				=DIK_U, //「U」キー
	I				=DIK_I, //「I」キー
	O				=DIK_O, //「O」キー
	P				=DIK_P, //「P」キー
	LBRACKET		=DIK_LBRACKET, //「 [ 」キー
	RBRACKET		=DIK_RBRACKET, //「 ] 」キー
	ENTER			=DIK_RETURN, //「Enter」キー
	LCONTROL		=DIK_LCONTROL, //「Ctrl」キー（左）
	A				=DIK_A, //「A」キー
	S				=DIK_S, //「S」キー
	D				=DIK_D, //「D」キー
	F				=DIK_F, //「F」キー
	G				=DIK_G, //「G」キー
	H				=DIK_H, //「H」キー
	J				=DIK_J, //「J」キー
	K				=DIK_K, //「K」キー
	L				=DIK_L, //「L」キー
	SEMICOLON		=DIK_SEMICOLON, //「;」キー
	APOSTROPHE		=DIK_APOSTROPHE, //「 ' 」キー
	GRAVE			=DIK_GRAVE, //「 ` 」キー
	LSHIFT			=DIK_LSHIFT, //「Shift」キー（左）
	BACKSLASH		=DIK_BACKSLASH, //「\」キー
	Z				=DIK_Z, //「Z」キー
	X				=DIK_X, //「X」キー
	C				=DIK_C, //「C」キー
	V				=DIK_V, //「V」キー
	B				=DIK_B, //「B」キー
	N				=DIK_N, //「N」キー
	M				=DIK_M, //「M」キー
	COMMA			=DIK_COMMA, //「 , 」キー
	PERIOD			=DIK_PERIOD, //「 . 」キー
	SLASH			=DIK_SLASH, //「/」キー
	RSHIFT			=DIK_RSHIFT, //「Shift」キー（右）
	MULTIPLY		=DIK_MULTIPLY, //「*」キー（Numpad）
	LALT			=DIK_LMENU, //「Alt」キー（左）
	SPACE			=DIK_SPACE, //「Space」キー
	CAPSLOCK		=DIK_CAPITAL, //「Caps Lock」キー
	F1				=DIK_F1, //「F1」キー
	F2				=DIK_F2, //「F2」キー
	F3				=DIK_F3, //「F3」キー
	F4				=DIK_F4, //「F4」キー
	F5				=DIK_F5, //「F5」キー
	F6				=DIK_F6, //「F6」キー
	F7				=DIK_F7, //「F7」キー
	F8				=DIK_F8, //「F8」キー
	F9				=DIK_F9, //「F9」キー
	F10				=DIK_F10, //「F10」キー
	NUMLOCK			=DIK_NUMLOCK, //「Num Lock」キー
	SCROLL			=DIK_SCROLL, //「Scroll Lock」キー
	NUMPAD7			=DIK_NUMPAD7, //「7」キー（Numpad）
	NUMPAD8			=DIK_NUMPAD8, //「8」キー（Numpad）
	NUMPAD9			=DIK_NUMPAD9, //「9」キー（Numpad）
	SUBTRACT		=DIK_SUBTRACT, //「-」キー（Numpad）
	NUMPAD4			=DIK_NUMPAD4, //「4」キー（Numpad）
	NUMPAD5			=DIK_NUMPAD5, //「5」キー（Numpad）
	NUMPAD6			=DIK_NUMPAD6, //「6」キー（Numpad）
	ADD				=DIK_ADD, //「+」キー（Numpad）
	NUMPAD1			=DIK_NUMPAD1, //「1」キー（Numpad）
	NUMPAD2			=DIK_NUMPAD2, //「2」キー（Numpad）
	NUMPAD3			=DIK_NUMPAD3, //「3」キー（Numpad）
	NUMPAD0			=DIK_NUMPAD0, //「0」キー（Numpad）
	DECIMAL			=DIK_DECIMAL, //「.」キー（Numpad）
	F11				=DIK_F11, //「F11」キー
	F12				=DIK_F12, //「F12」キー
	F13				=DIK_F13, //「F13」キー
	F14				=DIK_F14, //「F14」キー
	F15				=DIK_F15, //「F15」キー
	KANA			=DIK_KANA, //「カナ」キー（日本語キーボード）
	CONVERT			=DIK_CONVERT, //「変換」キー（日本語キーボード）
	NOCONVERT		=DIK_NOCONVERT, //「無変換」キー（日本語キーボード）
	YEN				=DIK_YEN, //「\」キー（日本語キーボード）
	NUMPADEQUALS	=DIK_NUMPADEQUALS, //「=」キー（Numpad）
	CIRCUMFLEX		=DIK_CIRCUMFLEX, //「^」キー（日本語キーボード）
	AT				=DIK_AT, //「@」キー
	COLON			=DIK_COLON, //「:」キー
	UNDERLINE		=DIK_UNDERLINE, //「 _ 」キー
	KANJI			=DIK_KANJI, //「半角/全角」キー
	STOP			=DIK_STOP, //「Stop」キー
	NUMPADENTER		=DIK_NUMPADENTER, //「Enter」キー（Numpad）
	RCONTROL		=DIK_RCONTROL, //「Ctrl」キー（右）
	NUMPADCOMMA		=DIK_NUMPADCOMMA, //「 , 」キー（Numpad）
	DIVIDE			=DIK_DIVIDE, //「/」キー（Numpad）
	SYSRQ			=DIK_SYSRQ, //「Sys Rq」キー
	RALT			=DIK_RMENU, //「Alt」キー（右）
	PAUSE			=DIK_PAUSE, //「Pause」キー
	HOME			=DIK_HOME, //「Home」キー
	UP				=DIK_UP, //「↑」キー
	PAGEUP			=DIK_PRIOR, //「Page Up」キー
	LEFT			=DIK_LEFT, //「←」キー
	RIGHT			=DIK_RIGHT, //「→」キー
	END				=DIK_END, //「End」キー
	DOWN			=DIK_DOWN, //「↓」キー
	PAGEDOWN		=DIK_NEXT, //「Page Down」キー
	INSERT			=DIK_INSERT, //「Insert」キー
	DELETE_			=DIK_DELETE, //「Delete」キー
	LWIN			=DIK_LWIN, //「Windows」キー（左）
	RWIN			=DIK_RWIN, //「Windows」キー（右）
	APPS			=DIK_APPS, //「Menu」キー
	POWER			=DIK_POWER //「Power」キー
};


// キーボード
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


// マウスボタン入力用マスク
enum class MouseButton : unsigned char
{
	Left =0,
	Right =1,
	Middle =2,
};


// マウス
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


// XInputボタン用マスク
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


// XInputコントローラー
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