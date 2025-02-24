#pragma once

#include <vector>
#include <map>

class Window;

enum class InputType : std::uint8_t
{
	Press,
	Hold
};

enum class InputStatus : std::uint8_t
{
	None,
	Pressing,
	WaitingRelease
};

class InputHandler
{
public:
	InputHandler();

	void HandleKey(int InKey, InputType InType);
	void Update(const Window& InWindow, float Delta);
	void Clean();

	bool IsPressed(int InKey) const;
	bool IsReleased(int InKey) const;

private:
	std::map<int, InputStatus> ActiveKeys;
	std::map<int, InputType> HandledKeys;
};