#pragma once

#include <fmod/fmod.hpp>

#include <map>
#include <string>

class SoundEngine
{
public:
	typedef unsigned int Id;

	typedef std::map<std::string, FMOD::Sound*> SoundsMap;
	typedef std::pair<std::string, FMOD::Sound*> SoundPair;
	typedef std::map<Id, FMOD::Channel*> ChannelsMap;
	typedef std::pair<Id, FMOD::Channel*> ChannelPair;

	static SoundEngine& Get()
	{
		static SoundEngine Instance;
		return Instance;
	}

	void Load(const std::string& SoundPath);
	Id Play(const std::string& SoundPath, float Volume);
	Id Play(const std::string& SoundPath, float Volume, bool bMuted, bool bLoop);
	void Mute(Id ChannelId, bool bMute);
	void Stop(Id ChannelId);
	void SetChannelVolume(Id ChannelId, float NewVolume);
	void SetChannelPitch(Id ChannelId, float Pitch);
	bool IsPlaying(Id ChannelId) const;
	bool IsLooping(Id ChannelId) const;

	bool IsLoaded(const std::string& SoundName) const;

	FMOD_RESULT GetLastResult() const;

	void Update(const float Delta);

	SoundEngine(const SoundEngine&) = delete;
	void operator=(const SoundEngine&) = delete;

	~SoundEngine();

private:
	bool IsActive(Id ChannelId) const;
	SoundEngine();
	void Initialize();

	FMOD::System* System;
	FMOD_RESULT LastResult;

	SoundsMap LoadedSounds;
	ChannelsMap ActiveChannels;
	Id NextChannelId;
};