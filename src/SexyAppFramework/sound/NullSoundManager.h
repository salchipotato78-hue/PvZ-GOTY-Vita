/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
 *
 * Silent sound backend for platforms without an audio implementation yet
 * (PS Vita first build). Accepts every call, plays nothing.
 */

#ifndef __NULLSOUNDMANAGER_H__
#define __NULLSOUNDMANAGER_H__

#include "SoundManager.h"
#include "SoundInstance.h"
#include <string>

namespace Sexy
{

class NullSoundInstance : public SoundInstance
{
public:
	void Release() override {}
	void SetBaseVolume(double) override {}
	void SetBasePan(int) override {}
	void AdjustPitch(double) override {}
	void SetVolume(double) override {}
	void SetPan(int) override {}
	bool Play(bool, bool) override { return false; }
	void Stop() override {}
	bool IsPlaying() override { return false; }
	bool IsReleased() override { return true; }
	double GetVolume() override { return 0.0; }
};

class NullSoundManager : public SoundManager
{
	NullSoundInstance mInstance;
	intptr_t mNextId = 0;
	double mMasterVolume = 1.0;

public:
	bool Initialized() override { return true; }

	bool LoadSound(intptr_t, const std::string&) override { return true; }
	intptr_t LoadSound(const std::string&) override { return mNextId++; }
	void ReleaseSound(intptr_t) override {}

	void SetVolume(double) override {}
	bool SetBaseVolume(intptr_t, double) override { return true; }
	bool SetBasePan(intptr_t, int) override { return true; }

	SoundInstance* GetSoundInstance(intptr_t) override { return &mInstance; }

	void ReleaseSounds() override {}
	void ReleaseChannels() override {}

	double GetMasterVolume() override { return mMasterVolume; }
	void SetMasterVolume(double theVolume) override { mMasterVolume = theVolume; }

	void Flush() override {}
	void StopAllSounds() override {}
	intptr_t GetFreeSoundId() override { return mNextId; }
	int GetNumSounds() override { return 0; }
};

}

#endif //__NULLSOUNDMANAGER_H__
