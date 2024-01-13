#pragma once
#include <vector>
#include "SDL_mixer.h"  //required for audio
#include <iostream>
#include <map>

using namespace std;


class AudioManager
{
private:
	vector<Mix_Music*> m_vMusicTracks;
	vector<Mix_Chunk*> m_vSounds;

	// global music and sound tracks
	map<const char* , int> m_mMusicTrackIds;
	map<const char*, int> m_mSoundTrackIds;

	// local music and sound tracks
	map<const char*, int> m_mMusicLocalCache;
	map<const char*, int> m_mSoundLocalCache;

	int m_pTotalTrack = 0;
	int m_pTotalSound = 0;


public:
	AudioManager();
	~AudioManager();
	bool Init();

	//Set volume of the music
	void SetMusicVolume(int volLevel);

	///loops=-1 means infinite
	void PlayMusic(int id, int loops = -1);


	void PlaySound(int id, int channel = -1, int loops = 0);


	void LoadMusic(const char* path);
	void LoadSound(const char* path);


	void UnloadMusic();
	void UnloadSound();

	//Toggle music:: so that you can pause and resume the current music track
	void ToggleMusic();

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	void LoadMusicGlobal(const char* path, const char* labelName);
	void PlayMusicGlobal(const char* lableName);
	// LoadMusicLocal("Audio/level1/death.mp3", "Death");
	void LoadMusicLocal(const char* path, const char* labelName);
	void PlayMusicLocal(const char* lableName);


	void LoadSoundGlobal(const char* path, const char* labelName);
	void PlaySoundGlobal(const char* lableName);
	void LoadSoundLocal(const char* path, const char* labelName);
	void PlaySoundLocal(const char* lableName);

};