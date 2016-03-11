#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include "fmod.hpp"
#include "fmod_errors.h"

class SoundManagerC
{
public:
	static SoundManagerC *CreateInstance();
	static SoundManagerC *GetInstance() { return sInstance; };
	~SoundManagerC(){};

	void	init();
	void	shutdown();
	void FMODErrorCheck(FMOD_RESULT result);
	void PlayBirdReleaseSound();
	void PlayExplosion();
	void PlayLevelComplete();
	void PlayLevelLost();
	void PlayDeadPig();
	void PauseBG(bool On = true);
	void Close();

private:
	static SoundManagerC *sInstance;
	FMOD::Sound *BGMusic;
	FMOD::Sound *BirdReleased;
	FMOD::Sound *Explosion;
	FMOD::Sound *LevelComplete;
	FMOD::Sound *LevelLost;
	FMOD::Sound *DeadPig;
	FMOD::System *system;
	FMOD_RESULT result;
	unsigned int version;
	int numDrivers;
	FMOD_SPEAKERMODE speakerMode;
	//FMOD_CAPS caps;
	char name[256];
	FMOD::Channel *channel;
};

#endif