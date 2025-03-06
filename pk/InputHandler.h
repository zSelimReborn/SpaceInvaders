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
	void HandlePad(int PadId);
	void HandlePadKey(int InKey, InputType InType);
	void Update(const Window& InWindow, float Delta);
	void Clean();

	bool IsPressed(int InKey) const;
	bool IsReleased(int InKey) const;
	bool IsPadPressed(int InKey) const;
	bool IsPadReleased(int InKey) const;

private:
	bool IsValidPad(int PadId) const;
	void UpdateKeys(const Window& InWindow, float Delta);
	void UpdatePadKeys(const Window& InWindow, float Delta);

	void CleanKeys();
	void CleanPadKeys();

	std::map<int, InputStatus> ActiveKeys;
	std::map<int, InputType> HandledKeys;

	int HandledPad;
	std::map<int, InputStatus> ActivePadKeys;
	std::map<int, InputType> HandledPadKeys;
};