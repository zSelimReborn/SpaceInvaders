#include "SaveSystem.h"

#include <fstream>
#include <sstream>

void SaveSystem::TriggerSave() const
{
	if (!IsValid())
	{
		return;
	}

	Save();
}

bool SaveSystem::IsValid() const
{
	return CurrentSave != nullptr && !CurrentFilePath.empty();
}

SaveSystem::SaveSystem() = default;

void SaveSystem::Save() const
{
	if (!IsValid())
	{
		return;
	}

	std::ofstream Writer;
	Writer.open(CurrentFilePath, std::ios::binary);
	CurrentSave->Write(Writer);
}
