#include "InputManager.h"

#ifdef _WIN32
#include <Windows.h>
#endif

using namespace Mazeline;

VoidEvent g_voidEvent;

InputManager::InputManager() {
	m_hasLoaded = false;
}

bool InputManager::Load() {
	return true;
}

bool InputManager::Unload() {
	return true;
}

void InputManager::RegisterKeyPressHandler(EventCallback<VoidEvent, KeyEventArgs>* callback) {
	m_onKeyPress.AddCallback(callback);
}

void InputManager::RegisterKeyDownHandler(EventCallback<VoidEvent, KeyEventArgs>* callback) {
	m_onKeyDown.AddCallback(callback);
}

void InputManager::RegisterKeyInputHandler(EventCallback<VoidEvent, KeyInputEventArgs>* callback) {
	m_onKeyInput.AddCallback(callback);
}

void InputManager::RegisterKeyCommandHandler(EventCallback<VoidEvent, KeyCommandEventArgs>* callback) {
	m_onKeyCommand.AddCallback(callback);
}

void InputManager::RegisterMouseClickHandler(EventCallback<VoidEvent, MouseEventArgs>* callback) {
	m_onMouseClick.AddCallback(callback);
}

void InputManager::RegisterMouseDownHandler(EventCallback<VoidEvent, MouseEventArgs>* callback) {
	m_onMouseDown.AddCallback(callback);
}

void InputManager::RegisterMouseMoveHandler(EventCallback<VoidEvent, MouseMoveEventArgs>* callback) {
	m_onMouseMove.AddCallback(callback);
}

void InputManager::RegisterMouseScrollHandler(EventCallback<VoidEvent, MouseScrollEventArgs>* callback) {
	m_onMouseScroll.AddCallback(callback);
}

void InputManager::RemoveKeyPressHandler(EventCallback<VoidEvent, KeyEventArgs>* callback) {
	m_onKeyPress.RemoveCallback(callback);
}

void InputManager::RemoveKeyDownHandler(EventCallback<VoidEvent, KeyEventArgs>* callback) {
	m_onKeyDown.RemoveCallback(callback);
}

void InputManager::RemoveKeyInputHandler(EventCallback<VoidEvent, KeyInputEventArgs>* callback) {
	m_onKeyInput.RemoveCallback(callback);
}

void InputManager::RemoveKeyCommandHandler(EventCallback<VoidEvent, KeyCommandEventArgs>* callback) {
	m_onKeyCommand.RemoveCallback(callback);
}

void InputManager::RemoveMouseClickHandler(EventCallback<VoidEvent, MouseEventArgs>* callback) {
	m_onMouseClick.RemoveCallback(callback);
}

void InputManager::RemoveMouseDownHandler(EventCallback<VoidEvent, MouseEventArgs>* callback) {
	m_onMouseDown.RemoveCallback(callback);
}

void InputManager::RemoveMouseMoveHandler(EventCallback<VoidEvent, MouseMoveEventArgs>* callback) {
	m_onMouseMove.RemoveCallback(callback);
}

void InputManager::RemoveMouseScrollHandler(EventCallback<VoidEvent, MouseScrollEventArgs>* callback) {
	m_onMouseScroll.RemoveCallback(callback);
}

void InputManager::RaiseKeyPressEvent(KeyEventArgs& e) {
	m_onKeyPress.Fire(e, g_voidEvent);
}

void InputManager::RaiseKeyDownEvent(KeyEventArgs& e) {
	m_onKeyDown.Fire(e, g_voidEvent);
}

void InputManager::RaiseKeyInputEvent(KeyInputEventArgs& e) {
	m_onKeyInput.Fire(e, g_voidEvent);
}

void InputManager::RaiseKeyCommandEvent(KeyCommandEventArgs& e) {
	m_onKeyCommand.Fire(e, g_voidEvent);
}

void InputManager::RaiseMouseClickEvent(MouseEventArgs& e) {
	m_onMouseClick.Fire(e, g_voidEvent);
}

void InputManager::RaiseMouseDownEvent(MouseEventArgs& e) {
	m_onMouseDown.Fire(e, g_voidEvent);
}

void InputManager::RaiseMouseMoveEvent(MouseMoveEventArgs& e) {
	m_onMouseMove.Fire(e, g_voidEvent);
}

void InputManager::RaiseMouseScrollEvent(MouseScrollEventArgs& e) {
	m_onMouseScroll.Fire(e, g_voidEvent);
}

bool InputManager::IsMouseButtonDown(MButton button) {
#ifdef _WIN32
	return ((unsigned short)GetKeyState(button)) >> 15;
#else
#endif
}

bool InputManager::IsKeyDown(MKey key) {
#ifdef _WIN32
	return ((unsigned short)GetKeyState(key)) >> 15;
#else
#endif
}

InputManager& InputManager::GetInstance() {
	static InputManager instance;
	return instance;
}

InputManager::~InputManager() {

}