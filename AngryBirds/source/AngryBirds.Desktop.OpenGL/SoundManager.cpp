#include "stdafx.h"
#include "SoundManager.h"

SoundManagerC* SoundManagerC::sInstance = NULL;	//singleton, has pointer to itself

//makes an instance if there is none else returns the istance
SoundManagerC *SoundManagerC::CreateInstance()
{
	assert(sInstance == NULL);
	sInstance = new SoundManagerC();
	return sInstance;
}

void SoundManagerC::init()
{
	printf("Hey initing the sound manager!\n");
	
	//Create FMOD interface object
	result = FMOD::System_Create(&system);
	FMODErrorCheck(result);
	// Get number of sound cards 
	result = system->getNumDrivers(&numDrivers);
	FMODErrorCheck(result);

	// No sound cards (disable sound)
	if (numDrivers == 0)
	{
		result = system->setOutput(FMOD_OUTPUTTYPE_NOSOUND);//printf("%d\n", numDrivers);
		FMODErrorCheck(result);
	}
	// Initialise FMOD
	result = system->init(100, FMOD_INIT_NORMAL, 0);
	
	////start stream for background music
	system->createStream("content/Sounds/9c6ba3_Angry_Birds_Theme_Song.mp3", FMOD_DEFAULT, 0, &BGMusic);
	system->playSound(BGMusic, nullptr, false, &channel);
	channel->setMode(FMOD_LOOP_NORMAL);
	channel->setVolume(.5);
	channel->setLoopCount(-1);

	//load short sounds
	system->createSound("content/Sounds/3fc733_Angry_Birds_Bird_Fly_Sound_Effect.mp3", FMOD_DEFAULT, 0, &BirdReleased);
	system->createSound("content/Sounds/730c5b_Angry_Birds_TNT_Sound_Effect.mp3", FMOD_DEFAULT, 0, &Explosion);
	system->createSound("content/Sounds/d2ef5d_Angry_Birds_Level_Complete_Sound_Effect.mp3", FMOD_DEFAULT, 0, &LevelComplete);
	system->createSound("content/Sounds/14e4c1_Angry_Birds_Bird_Boss_Sound_Effect.mp3", FMOD_DEFAULT, 0, &LevelLost);
	system->createSound("content/Sounds/f40e61_Angry_Birds_Piglette_Damage_Sound_FX.mp3", FMOD_DEFAULT, 0, &DeadPig);
}

//unload all sounds and kill streams
void SoundManagerC::Close()
{
	FMODErrorCheck(DeadPig->release());
	FMODErrorCheck(LevelComplete->release());
	FMODErrorCheck(Explosion->release());
	FMODErrorCheck(BirdReleased->release());
	FMODErrorCheck(BGMusic->release());
	FMODErrorCheck(LevelLost->release());
	FMODErrorCheck(system->release());
}

//pauses or unpauses background music
void SoundManagerC::PauseBG(bool On)
{
	channel->setPaused(On);
}

void SoundManagerC::PlayBirdReleaseSound()
{
	system->playSound(BirdReleased, nullptr, false, nullptr);
}

void SoundManagerC::PlayExplosion()
{
	system->playSound(Explosion, nullptr, false, nullptr);
}

void SoundManagerC::PlayLevelComplete()
{
	system->playSound(LevelComplete, nullptr, false, nullptr);
}

void SoundManagerC::PlayLevelLost()
{
	system->playSound(LevelLost, nullptr, false, nullptr);
}

void SoundManagerC::PlayDeadPig()
{
	system->playSound(DeadPig, nullptr, false, nullptr);
}

//checks for errors when loading sounds and openning streams
void SoundManagerC::FMODErrorCheck(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		printf("FMOD error! %d, %s.\n", result, FMOD_ErrorString(result));
		exit(-1);
	}
}

