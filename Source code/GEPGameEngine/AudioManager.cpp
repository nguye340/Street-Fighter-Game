#include <algorithm>
#include "AudioManager.h"


AudioManager::AudioManager()
{

}

bool AudioManager::Init()
{

	if (Mix_Init(MIX_INIT_MP3) == 0)
	{
		cout << "Mixer init failed\n";
		return false;
	}
	else
	{
		Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 4096);
		Mix_AllocateChannels(16);
		return true;
	}
}

void AudioManager::SetMusicVolume(int volLevel)
{
	int clampedVolume =
		std::clamp(volLevel, 0, 128);

	Mix_VolumeMusic(clampedVolume);

}

void AudioManager::PlayMusic(int id, int loops)
{
	Mix_PlayMusic(m_vMusicTracks[id], loops);

}

void AudioManager::PlaySound(int id, int channel, int loops)
{
	Mix_PlayChannel(channel, m_vSounds[id], loops);
}

void AudioManager::LoadMusic(const char* path)
{
	//check if it is loadable first...
	Mix_Music* t = Mix_LoadMUS(path);
	if (t != nullptr)
	{
		m_vMusicTracks.push_back(t);
	}
	else
	{
		//error
		cout << "Music load failed. \n";
	}
}

void AudioManager::LoadSound(const char* path)
{
	Mix_Chunk* t = Mix_LoadWAV(path);
	if (t != nullptr)
		m_vSounds.push_back(t);
	else
		cout << "Sound load failed. \n";
}

void AudioManager::ToggleMusic()
{
	//Mix_PausedMusic() returns 1 if paused.
	if (Mix_PausedMusic() == 1)
		Mix_ResumeMusic();
	else
		Mix_PauseMusic();

}

void AudioManager::UnloadMusic()
{
	//how to unload a single music track from memory:
	//Mix_FreeMusic(  )


	//how do you unload the entire vector?
	//iterate through the vector,
	//unload each index, and set its index to nullptr
	//after your do the above, 
	//clear the vector & shrink to fita
	if (m_mMusicLocalCache.size() > 0)
	{
		for (const auto& p : m_mMusicLocalCache)
		{
			//std::cout << p.first << '\t' << p.second << std::endl;
			Mix_FreeMusic(m_vMusicTracks[p.second]);
			m_vMusicTracks[p.second] = nullptr;
			delete m_vMusicTracks[p.second];

			//m_vMusicTracks.erase(m_vMusicTracks.begin() + p.second);

		}
		m_vMusicTracks.clear();
		m_vMusicTracks.shrink_to_fit();
	}
	else if (m_mMusicTrackIds.size() > 0)
	{
		for (int i = 0; i < (int)m_vMusicTracks.size(); i++)
		{
			cout << "work***** - " << i;
			Mix_FreeMusic(m_vMusicTracks[i]);
			m_vMusicTracks[i] = nullptr;
			delete m_vMusicTracks[i];
			m_vMusicTracks.erase(m_vMusicTracks.begin() + i);
		}
		m_vMusicTracks.clear();
		m_vMusicTracks.shrink_to_fit();
	}
}

void AudioManager::UnloadSound()
{
	//how to unload a single audio chunk from memory:
	//Mix_FreeChunk(    )
	if (m_mSoundLocalCache.size() > 0)
	{
		for (const auto& p : m_mMusicLocalCache)
		{
			//std::cout << p.first << '\t' << p.second << std::endl;
			Mix_FreeChunk(m_vSounds[p.second]);
			delete m_vSounds[p.second];
			m_vSounds[p.second] = nullptr;
			m_vSounds.erase(m_vSounds.begin() + p.second);
		}
		m_vSounds.shrink_to_fit();
	}
	else if (m_mSoundTrackIds.size() > 0)
	{
		for (int i = 0; i < (int)m_vSounds.size(); i++)
		{
			Mix_FreeChunk(m_vSounds[i]);
			m_vSounds[i] = nullptr;
			delete m_vSounds[i];
			m_vSounds.erase(m_vSounds.begin() + i);
		}

		m_vSounds.clear();
		m_vSounds.shrink_to_fit();
	}
	
}

AudioManager::~AudioManager()
{
	UnloadMusic();
	UnloadSound();

	Mix_CloseAudio();
	Mix_Quit();
}

void AudioManager::LoadMusicGlobal(const char* path, const char* labelName)
{
	//check if it is loadable first...
	Mix_Music* t = Mix_LoadMUS(path);
	if (t != nullptr)
	{
		m_pTotalTrack++;
		m_vMusicTracks.push_back(t);
		m_mMusicTrackIds.insert({ labelName, m_pTotalTrack });
	}
	else
	{
		//error
		cout << "Music load failed. \n";
	}
}

void AudioManager::LoadMusicLocal(const char* path, const char* labelName)
{
	//check if it is loadable first...
	Mix_Music* t = Mix_LoadMUS(path);
	if (t != nullptr)
	{
		m_vMusicTracks.push_back(t);
		m_mMusicLocalCache.insert({ labelName, m_pTotalTrack });
	}
	else
	{
		//error
		cout << "Local music load failed. \n";
	}
}

void AudioManager::PlayMusicGlobal(const char * label)
{
	if (m_mMusicTrackIds.find(label) != m_mMusicTrackIds.end())
	{
		int i = m_mMusicTrackIds.at(label);

		Mix_PlayMusic(m_vMusicTracks[i - 1], true);
	}
	
}

void AudioManager::PlayMusicLocal(const char* label)
{
	if (m_mMusicLocalCache.find(label) != m_mMusicLocalCache.end())
	{
		int i = m_mMusicLocalCache.at(label);
		cout << "HEYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY - " << i;
		Mix_PlayMusic(m_vMusicTracks[i], true);
	}

}

void AudioManager::LoadSoundLocal(const char* path, const char* labelName)
{
	Mix_Chunk* t = Mix_LoadWAV(path);
	if (t != nullptr)
	{
		m_vSounds.push_back(t);
		m_mSoundLocalCache.insert({labelName, m_pTotalSound});
	}

	else
		cout << "Sound load failed. \n";
}

void AudioManager::PlaySoundLocal(const char* label)
{
	if (m_mSoundLocalCache.find(label) != m_mSoundLocalCache.end())
	{
		Mix_PlayChannel(-1, m_vSounds[m_mSoundLocalCache.at(label) - 1], 2);
	}
	//Mix_PlayMusic(m_vMusicTracks[id], loops);

}

void AudioManager::LoadSoundGlobal(const char* path, const char* labelName)
{
	Mix_Chunk* t = Mix_LoadWAV(path);
	if (t != nullptr)
	{
		m_vSounds.push_back(t);
		m_mSoundTrackIds.insert({ labelName, m_pTotalSound });
	}

	else
		cout << "Sound load failed. \n";
}

void AudioManager::PlaySoundGlobal(const char* label)
{
	if (m_mSoundTrackIds.find(label) != m_mSoundTrackIds.end())
	{
		Mix_PlayChannel(-1, m_vSounds[m_mSoundTrackIds.at(label)], 1);
	}
	//Mix_PlayMusic(m_vMusicTracks[id], loops);

}