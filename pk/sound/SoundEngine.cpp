#include "SoundEngine.h"

#include <iostream>
#include <vector>

#include "../core/utils/Common.h"

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

SoundEngine::Id SoundEngine::Play(const std::string& SoundPath, const float Volume)
{
	return Play(SoundPath, Volume, false, false);
}

SoundEngine::Id SoundEngine::Play(const std::string& SoundPath, float Volume, bool bMuted, bool bLoop)
{
	if (!System || !IsLoaded(SoundPath))
	{
		return 0;
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
			Channel->setLoopCount(-1);
			Channel->setMode(FMOD_LOOP_NORMAL);
		}
		else
		{
			Channel->setLoopCount(0);
		}
		Channel->setPaused(false);
		Id ChannelId = NextChannelId++;
		ActiveChannels.insert(ChannelPair(ChannelId, Channel));
		return ChannelId;
	}

	return 0;
}

void SoundEngine::Mute(Id ChannelId, bool bMute)
{
	if (!IsActive(ChannelId))
	{
		return;
	}

	ActiveChannels[ChannelId]->setMute(bMute);
}

void SoundEngine::Stop(Id ChannelId)
{
	if (!IsActive(ChannelId))
	{
		return;
	}

	ActiveChannels[ChannelId]->stop();
}

void SoundEngine::SetChannelVolume(Id ChannelId, float NewVolume)
{
	if (!IsActive(ChannelId))
	{
		return;
	}

	const float ActualVolume = Math::Clamp(NewVolume, 0.f, 1.f);
	ActiveChannels[ChannelId]->setVolume(ActualVolume);
}

void SoundEngine::SetChannelPitch(Id ChannelId, float Pitch)
{
	if (!IsActive(ChannelId))
	{
		return;
	}

	ActiveChannels[ChannelId]->setPitch(Pitch);
}

bool SoundEngine::IsPlaying(Id ChannelId) const
{
	if (!IsActive(ChannelId))
	{
		return false;
	}

	bool bPlaying = false;
	FMOD_RESULT Result = ActiveChannels.at(ChannelId)->isPlaying(&bPlaying);
	if (Result != FMOD_OK)
	{
		return false;
	}

	return bPlaying;
}

bool SoundEngine::IsLooping(Id ChannelId) const
{
	if (!IsActive(ChannelId))
	{
		return false;
	}

	FMOD_RESULT Result;
	FMOD::Channel* Channel = ActiveChannels.at(ChannelId);
	FMOD_MODE LoopMode;
	Result = Channel->getMode(&LoopMode);
	if (Result != FMOD_OK || LoopMode == FMOD_DEFAULT)
	{
		return false;
	}

	int LoopCount = 0;
	Result = Channel->getLoopCount(&LoopCount);
	if (Result != FMOD_OK)
	{
		return false;
	}

	return LoopCount != 0;
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
		Id ChannelId = It->first;
		if (!IsPlaying(ChannelId) && !IsLooping(ChannelId))
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

bool SoundEngine::IsActive(Id ChannelId) const
{
	return ActiveChannels.count(ChannelId) > 0;
}

SoundEngine::SoundEngine()
	: NextChannelId(1)
{
	Initialize();
}

void SoundEngine::Initialize()
{
	LastResult = FMOD::System_Create(&System);

	System->init(32, FMOD_INIT_NORMAL, nullptr);
}
