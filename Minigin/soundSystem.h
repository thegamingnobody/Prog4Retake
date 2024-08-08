#ifndef DAE_SOUNDSYSTEM
#define DAE_SOUNDSYSTEM

#include <string>

namespace dae
{
	using SoundId = unsigned short;

	class SoundSystem
	{
	public:
		SoundSystem() = default;
		virtual ~SoundSystem() = default;

		SoundSystem(const SoundSystem& other) = delete;
		SoundSystem(SoundSystem&& other) = delete;
		SoundSystem& operator=(const SoundSystem& other) = delete;
		SoundSystem& operator=(SoundSystem&& other) = delete;

		virtual void PlaySound(const SoundId soundId, const float volume) = 0;
		virtual void StopSound(const SoundId soundId) = 0;
		virtual void StopAllSounds() = 0;

		virtual void AddSound(const std::string& path) = 0;
	};
	
}

#endif