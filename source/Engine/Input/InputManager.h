#ifndef M_INPUTMANAGER_H
#define M_INPUTMANAGER_H

#include "EngineSubsystem.h"
#include "Events/Event.h"

namespace Mazeline {

	enum MKey {
		KeyBackspace = 0x08,
		KeyTab = 0x09,

		KeyClear = 0x0C,
		KeyEnter = 0x0D,

		KeyShift = 0x10,
		KeyControl = 0x11,
		KeyAlt = 0x12,
		KeyPause = 0x13,
		KeyCapital = 0x14,

		KeyEscape = 0x1B,

		KeySpace = 0x20,
		KeyPageUp = 0x21,
		KeyPageDown = 0x22,
		KeyEnd = 0x23,
		KeyHome = 0x24,

		KeyLeft = 0x25,
		KeyUp = 0x26,
		KeyRight = 0x27,
		KeyDown = 0x28,
		KeySelect = 0x29,
		KeyPrint = 0x2A,
		KeyExecute = 0x2B,
		KeyPrintScreen = 0x2C,
		KeyInsert = 0x2D,
		KeyDelete = 0x2E,
		KeyHelp = 0x2F,

		Key0 = 0x30,
		Key1 = 0x31,
		Key2 = 0x32,
		Key3 = 0x33,
		Key4 = 0x34,
		Key5 = 0x35,
		Key6 = 0x36,
		Key7 = 0x37,
		Key8 = 0x38,
		Key9 = 0x39,

		KeyA = 0x41,
		KeyB = 0x42,
		KeyC = 0x43,
		KeyD = 0x44,
		KeyE = 0x45,
		KeyF = 0x46,
		KeyG = 0x47,
		KeyH = 0x48,
		KeyI = 0x49,
		KeyJ = 0x4A,
		KeyK = 0x4B,
		KeyL = 0x4C,
		KeyM = 0x4D,
		KeyN = 0x4E,
		KeyO = 0x4F,
		KeyP = 0x50,
		KeyQ = 0x51,
		KeyR = 0x52,
		KeyS = 0x53,
		KeyT = 0x54,
		KeyU = 0x55,
		KeyV = 0x56,
		KeyW = 0x57,
		KeyX = 0x58,
		KeyY = 0x59,
		KeyZ = 0x5A,

		KeyNumPad0 = 0x60,
		KeyNumPad1 = 0x61,
		KeyNumPad2 = 0x62,
		KeyNumPad3 = 0x63,
		KeyNumPad4 = 0x64,
		KeyNumPad5 = 0x65,
		KeyNumPad6 = 0x66,
		KeyNumPad7 = 0x67,
		KeyNumPad8 = 0x68,
		KeyNumPad9 = 0x69,

		KeyMultiply = 0x6A,
		KeyAdd = 0x6B,
		KeySubtract = 0x6D,
		KeyDecimal = 0x6E,
		KeyDivide = 0x6F,

		KeyF1 = 0x70,
		KeyF2 = 0x71,
		KeyF3 = 0x72,
		KeyF4 = 0x73,
		KeyF5 = 0x74,
		KeyF6 = 0x75,
		KeyF7 = 0x76,
		KeyF8 = 0x77,
		KeyF9 = 0x78,
		KeyF10 = 0x79,
		KeyF11 = 0x7A,
		KeyF12 = 0x7B,
		KeyF13 = 0x7C,
		KeyF14 = 0x7D,
		KeyF15 = 0x7E,
		KeyF16 = 0x7F,
		KeyF17 = 0x80,
		KeyF18 = 0x81,
		KeyF19 = 0x82,
		KeyF20 = 0x83,
		KeyF21 = 0x84,
		KeyF22 = 0x85,
		KeyF23 = 0x86,
		KeyF24 = 0x87,

		KeyNumLock = 0x90,
		KeyScrollLock = 0x91,

		KeyLShift = 0xA0,
		KeyRShift = 0xA1,
		KeyLControl = 0xA2,
		KeyRControl = 0xA3,
		KeyLMenu = 0xA4,
		KeyRMenu = 0xA5,

		KeyColon = 0xBA,
		KeyEquals = 0xBB,
		KeyComma = 0xBC,
		KeyMinus = 0xBD,
		KeyPeriod = 0xBE,

		KeyInvalid = 0xFF
	};

	enum MButton {
		MouseLeft = 0x01,
		MouseRight = 0x02,
		MouseMiddle = 0x04,
		MouseXButton1 = 0x05,
		MouseXButton2 = 0x06,
	};

	struct KeyEventArgs {
		MKey Key;
	};

	struct KeyInputEventArgs {
		char* Character;
		bool KeyRepeat;
	};

	struct KeyCommandEventArgs {
		MKey Key;
		int ModCount;
		MKey* Modifiers;
	};

	enum MouseMoveEventType {
		MMETPosition,
		MMETScroll
	};
	
	struct MouseEventArgs {
		MButton Button;
		int X;
		int Y;
	};

	struct MouseMoveEventArgs {
		int X;
		int Y;
		int dX;
		int dY;
	};

	struct MouseScrollEventArgs {
		int ScrollUp;
		int ScrollRight;
	};

	class InputManager : public EngineSubsystem {
	public:
		static InputManager& GetInstance();
		~InputManager();

		void RegisterKeyPressHandler(EventCallback<VoidEvent, KeyEventArgs>* callback);
		void RegisterKeyDownHandler(EventCallback<VoidEvent, KeyEventArgs>* callback);
		void RegisterKeyInputHandler(EventCallback<VoidEvent, KeyInputEventArgs>* callback);
		void RegisterKeyCommandHandler(EventCallback<VoidEvent, KeyCommandEventArgs>* callback);
		void RegisterMouseClickHandler(EventCallback<VoidEvent, MouseEventArgs>* callback);
		void RegisterMouseDownHandler(EventCallback<VoidEvent, MouseEventArgs>* callback);
		void RegisterMouseMoveHandler(EventCallback<VoidEvent, MouseMoveEventArgs>* callback);
		void RegisterMouseScrollHandler(EventCallback<VoidEvent, MouseScrollEventArgs>* callback);

		void RemoveKeyPressHandler(EventCallback<VoidEvent, KeyEventArgs>* callback);
		void RemoveKeyDownHandler(EventCallback<VoidEvent, KeyEventArgs>* callback);
		void RemoveKeyInputHandler(EventCallback<VoidEvent, KeyInputEventArgs>* callback);
		void RemoveKeyCommandHandler(EventCallback<VoidEvent, KeyCommandEventArgs>* callback);
		void RemoveMouseClickHandler(EventCallback<VoidEvent, MouseEventArgs>* callback);
		void RemoveMouseDownHandler(EventCallback<VoidEvent, MouseEventArgs>* callback);
		void RemoveMouseMoveHandler(EventCallback<VoidEvent, MouseMoveEventArgs>* callback);
		void RemoveMouseScrollHandler(EventCallback<VoidEvent, MouseScrollEventArgs>* callback);

		void RaiseKeyPressEvent(KeyEventArgs& e);
		void RaiseKeyDownEvent(KeyEventArgs& e);
		void RaiseKeyInputEvent(KeyInputEventArgs& e);
		void RaiseKeyCommandEvent(KeyCommandEventArgs& e);
		void RaiseMouseClickEvent(MouseEventArgs& e);
		void RaiseMouseDownEvent(MouseEventArgs& e);
		void RaiseMouseMoveEvent(MouseMoveEventArgs& e);
		void RaiseMouseScrollEvent(MouseScrollEventArgs& e);

		bool IsKeyDown(MKey key);
		bool IsMouseButtonDown(MButton button);

		bool Load();
		bool Unload();

		InputManager(const InputManager&) = delete;
		InputManager& operator = (const InputManager&) = delete;
	private:
		InputManager();

		Event<VoidEvent, KeyEventArgs> m_onKeyPress;
		Event<VoidEvent, KeyEventArgs> m_onKeyDown;
		Event<VoidEvent, KeyInputEventArgs> m_onKeyInput;
		Event<VoidEvent, KeyCommandEventArgs> m_onKeyCommand;

		Event<VoidEvent, MouseEventArgs> m_onMouseDown;
		Event<VoidEvent, MouseEventArgs> m_onMouseClick;
		Event<VoidEvent, MouseMoveEventArgs> m_onMouseMove;
		Event<VoidEvent, MouseScrollEventArgs> m_onMouseScroll;
	};
}

#endif