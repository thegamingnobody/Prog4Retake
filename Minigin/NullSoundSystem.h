#pragma once
#include "soundSystem.h"
#include <string>

namespace dae
{
	class NullSoundSystem final : public SoundSystem
	{
	public:
		NullSoundSystem() = default;
		~NullSoundSystem() override = default;

		NullSoundSystem(const NullSoundSystem& other) = delete;
		NullSoundSystem(NullSoundSystem&& other) = delete;
		NullSoundSystem& operator=(const NullSoundSystem& other) = delete;
		NullSoundSystem& operator=(NullSoundSystem&& other) = delete;

		void PlaySound(const SoundId, const float) override {};
		void StopSound(const SoundId) override {};
		void StopAllSounds() override {};

		void AddSound(const std::string&) override {};
	};
}