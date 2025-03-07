#pragma once

class ISaveFile
{
public:
	virtual void Load(std::ifstream& Reader) = 0;
	virtual void Write(std::ofstream& Writer) = 0;
	virtual ~ISaveFile() = default;
};
