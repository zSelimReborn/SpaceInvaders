#include "InputHandler.h"

#include "Window.h"

InputHandler::InputHandler()
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

void InputHandler::Update(const Window& InWindow, float Delta)
{
	for (const std::pair<int, InputType> KeyPair : HandledKeys)
	{
		int Key = KeyPair.first;
		InputType Type = KeyPair.second;

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

void InputHandler::Clean()
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
