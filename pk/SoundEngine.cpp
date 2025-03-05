#include "SoundEngine.h"

#include <iostream>
#include <vector>

#include "Common.h"

void SoundEngine::Load(const std::string& SoundPath)
{
	if (!System || IsLoaded(SoundPath))
	{
		return;
	}

	FMOD_MODE Mode = FMOD_DEFAULT;
	Mode |= FMOD_2D;
	Mode |= FMOD_LOOP_OFF;
	Mode |= FMOD_CREATECOMPRESSEDSAMPLE;

	FMOD::Sound* SoundObject = nullptr;
	FMOD_RESULT Result = System->createSound(SoundPath.c_str(), Mode, nullptr, &SoundObject);
	if (Result == FMOD_RESULT::FMOD_OK)
	{
		LoadedSounds.insert(std::pair<std::string, FMOD::Sound*>(SoundPath, SoundObject));
	}
	else
	{
		std::cout << "Unable to load " << SoundPath << " " << Result << "\n";
	}
}

int SoundEngine::Play(const std::string& SoundPath, const float Volume)
{
	return Play(SoundPath, Volume, false, false);
}

int SoundEngine::Play(const std::string& SoundPath, const float Volume, bool bMuted, bool bLoop)
{
	if (!System || !IsLoaded(SoundPath))
	{
		return -1;
	}

	const float ActualVolume = Math::Clamp(Volume, 0.f, 1.f);
	FMOD::Sound* SoundObject = LoadedSounds[SoundPath];
	FMOD::Channel* Channel = nullptr;
	LastResult = System->playSound(SoundObject, nullptr, true, &Channel);
	if (LastResult == FMOD_OK)
	{
		Channel->setVolume(ActualVolume);
		Channel->setMute(bMuted);
		if (bLoop)
		{
			Channel->setLoopCount(0);
		}
		Channel->setPaused(false);
		int ChannelId = NextChannelId++;
		ActiveChannels.insert(ChannelPair(ChannelId, Channel));
		return ChannelId;
	}

	return -1;
}

void SoundEngine::Mute(int ChannelId, bool bMute)
{
	if (!IsActive(ChannelId))
	{
		return;
	}

	ActiveChannels[ChannelId]->setMute(bMute);
}

void SoundEngine::Stop(int ChannelId)
{
	if (!IsActive(ChannelId))
	{
		return;
	}

	ActiveChannels[ChannelId]->stop();
}

void SoundEngine::SetChannelVolume(const int ChannelId, const float NewVolume)
{
	if (!IsActive(ChannelId))
	{
		return;
	}

	const float ActualVolume = Math::Clamp(NewVolume, 0.f, 1.f);
	ActiveChannels[ChannelId]->setVolume(ActualVolume);
}

void SoundEngine::SetChannelPitch(const int ChannelId, const float Pitch)
{
	if (!IsActive(ChannelId))
	{
		return;
	}

	ActiveChannels[ChannelId]->setPitch(Pitch);
}

bool SoundEngine::IsPlaying(const int ChannelId) const
{
	if (!IsActive(ChannelId))
	{
		return false;
	}

	bool bPlaying = false;
	ActiveChannels.at(ChannelId)->isPlaying(&bPlaying);
	return bPlaying;
}

bool SoundEngine::IsLoaded(const std::string& SoundName) const
{
	return LoadedSounds.count(SoundName) > 0;
}

FMOD_RESULT SoundEngine::GetLastResult() const
{
	return LastResult;
}

void SoundEngine::Update(const float Delta)
{
	std::vector<ChannelsMap::iterator> StoppedSounds;

	for (ChannelsMap::iterator It = ActiveChannels.begin(), End = ActiveChannels.end(); It != End; ++It)
	{
		bool bIsPlaying = false;
		It->second->isPlaying(&bIsPlaying);
		if (!bIsPlaying)
		{
			StoppedSounds.push_back(It);
		}
	}

	for (ChannelsMap::iterator& It : StoppedSounds)
	{
		ActiveChannels.erase(It);
	}

	System->update();
}

SoundEngine::~SoundEngine()
{
	System->release();
}

bool SoundEngine::IsActive(const int ChannelId) const
{
	return ActiveChannels.count(ChannelId) > 0;
}

SoundEngine::SoundEngine()
	: NextChannelId(0)
{
	Initialize();
}

void SoundEngine::Initialize()
{
	LastResult = FMOD::System_Create(&System);

	System->init(32, FMOD_INIT_NORMAL, nullptr);
}
