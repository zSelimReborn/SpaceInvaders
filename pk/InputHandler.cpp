#include "InputHandler.h"

#include <iostream>
#include <GLFW/glfw3.h>

#include "Window.h"

InputHandler::InputHandler()
	: HandledPad(-1)
{
	ActiveKeys.clear();
	HandledKeys.clear();
}

void InputHandler::HandleKey(int InKey, InputType InType)
{
	std::pair<int, InputType> HandleKey(InKey, InType);
	std::pair<int, InputStatus> ActiveKey(InKey, InputStatus::None);

	HandledKeys.insert(HandleKey);
	ActiveKeys.insert(ActiveKey);
}

void InputHandler::HandlePad(int PadId)
{
	// TODO improve to handle multiple gamepads
	if (!IsValidPad(PadId))
	{
		return;
	}

	HandledPad = PadId;
}

void InputHandler::HandlePadKey(int InKey, InputType InType)
{
	if (!IsValidPad(HandledPad))
	{
		return;
	}

	std::pair<int, InputType> HandleKey(InKey, InType);
	std::pair<int, InputStatus> ActiveKey(InKey, InputStatus::None);

	HandledPadKeys.insert(HandleKey);
	ActivePadKeys.insert(ActiveKey);
}

void InputHandler::Update(const Window& InWindow, float Delta)
{
	UpdateKeys(InWindow, Delta);
	UpdatePadKeys(InWindow, Delta);
}

void InputHandler::Clean()
{
	CleanKeys();
	CleanPadKeys();
}

bool InputHandler::IsPressed(int InKey) const
{
	if (ActiveKeys.count(InKey) <= 0)
	{
		return false;
	}

	return ActiveKeys.at(InKey) == InputStatus::Pressing;
}

bool InputHandler::IsReleased(int InKey) const
{
	return ActiveKeys.count(InKey) <= 0 || ActiveKeys.at(InKey) == InputStatus::None;
}

bool InputHandler::IsPadPressed(int InKey) const
{
	if (ActivePadKeys.count(InKey) <= 0)
	{
		return false;
	}

	return ActivePadKeys.at(InKey) == InputStatus::Pressing;
}

bool InputHandler::IsPadReleased(int InKey) const
{
	return ActivePadKeys.count(InKey) <= 0 || ActivePadKeys.at(InKey) == InputStatus::None;
}

bool InputHandler::IsValidPad(int PadId) const
{
	return PadId >= GLFW_JOYSTICK_1 && PadId <= GLFW_JOYSTICK_LAST;
}

void InputHandler::UpdateKeys(const Window& InWindow, float Delta)
{
	for (const std::pair<int, InputType> KeyPair : HandledKeys)
	{
		int Key = KeyPair.first;
		if (InWindow.IsPressed(Key) && ActiveKeys[Key] == InputStatus::None)
		{
			ActiveKeys[Key] = InputStatus::Pressing;
		}
		else if (InWindow.IsReleased(Key))
		{
			ActiveKeys[Key] = InputStatus::None;
		}
	}
}

void InputHandler::UpdatePadKeys(const Window& InWindow, float Delta)
{
	if (!IsValidPad(HandledPad))
	{
		return;
	}

	const int IsPresent = glfwJoystickPresent(HandledPad);
	const int IsGamePad = glfwJoystickIsGamepad(HandledPad);
	if (!IsPresent || !IsGamePad)
	{
		return;
	}

	GLFWgamepadstate PadState;
	if (!glfwGetGamepadState(HandledPad, &PadState))
	{
		return;
	}

	for (const std::pair<int, InputType> KeyPair : HandledPadKeys)
	{
		int Key = KeyPair.first;
		if (PadState.buttons[Key] && ActivePadKeys[Key] == InputStatus::None)
		{
			ActivePadKeys[Key] = InputStatus::Pressing;
		}
		else if (!PadState.buttons[Key])
		{
			ActivePadKeys[Key] = InputStatus::None;
		}
	}
}

void InputHandler::CleanKeys()
{
	for (const std::pair<int, InputType> KeyPair : HandledKeys)
	{
		int Key = KeyPair.first;
		InputType Type = KeyPair.second;
		if (Type == InputType::Press && ActiveKeys[Key] == InputStatus::Pressing)
		{
			ActiveKeys[Key] = InputStatus::WaitingRelease;
		}
	}
}

void InputHandler::CleanPadKeys()
{
	for (const std::pair<int, InputType> KeyPair : HandledPadKeys)
	{
		int Key = KeyPair.first;
		InputType Type = KeyPair.second;
		if (Type == InputType::Press && ActivePadKeys[Key] == InputStatus::Pressing)
		{
			ActivePadKeys[Key] = InputStatus::WaitingRelease;
		}
	}
}
