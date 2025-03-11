#pragma once

#include <fstream>
#include <memory>

#include "../utils/Common.h"
#include "ISaveFile.h"

namespace pk
{
	class SaveSystem
	{
	public:
		typedef std::shared_ptr<ISaveFile> SaveFilePtr;

		static SaveSystem& Get()
		{
			static SaveSystem Instance;
			return Instance;
		}

		template<typename T>
		void TriggerLoad(const std::string& FilePath);
		void TriggerSave() const;

		bool IsValid() const;

		template<typename T>
		std::shared_ptr<T> GetSaveObject() const;

	private:
		SaveSystem();

		template<typename T>
		void Load(const std::string& FilePath);
		void Save() const;

		std::string CurrentFilePath;
		SaveFilePtr CurrentSave;
	};

	template<typename T>
	void SaveSystem::TriggerLoad(const std::string& FilePath)
	{
		static_assert(std::is_base_of<ISaveFile, T>::value);
		if (FilePath.empty())
		{
			return;
		}

		if (!File::Exists(FilePath))
		{
			File::Create(FilePath, std::ios_base::binary);
		}

		Load<T>(FilePath);
	}

	template<typename T>
	void SaveSystem::Load(const std::string& FilePath)
	{
		static_assert(std::is_base_of<ISaveFile, T>::value);

		std::ifstream Reader;
		Reader.open(FilePath, std::ios::binary);
		CurrentSave = std::make_shared<T>();
		CurrentSave->Load(Reader);
		CurrentFilePath = FilePath;
	}

	template <typename T>
	std::shared_ptr<T> SaveSystem::GetSaveObject() const
	{
		static_assert(std::is_base_of<ISaveFile, T>::value);

		return std::dynamic_pointer_cast<T>(CurrentSave);
	}
}