#ifndef DAE_SDL_SOUNDSYSTEM
#define DAE_SDL_SOUNDSYSTEM

#include "soundSystem.h"
#include <memory>
#include "Observer.h"
#include <thread>


namespace dae
{
	class DAE_SDL_SoundSystem final : public SoundSystem, public Observer
	{
	public:
		DAE_SDL_SoundSystem();
		~DAE_SDL_SoundSystem() override;

		DAE_SDL_SoundSystem(const DAE_SDL_SoundSystem& other) = delete;
		DAE_SDL_SoundSystem(DAE_SDL_SoundSystem&& other) = delete;
		DAE_SDL_SoundSystem& operator=(const DAE_SDL_SoundSystem& other) = delete;
		DAE_SDL_SoundSystem& operator=(DAE_SDL_SoundSystem&& other) = delete;

		void PlaySound(const SoundId soundId, const float volume = 0.1f) override;
		void StopSound(const SoundId soundId) override;
		void StopAllSounds() override;

		void Notify(const Event& event) override;

		void QuitThread();

		void AddSound(const std::string& path) override;

	private:
		class SDLSoundImpl;
		std::unique_ptr<SDLSoundImpl> m_Impl;

		struct SoundInfo
		{
			SoundInfo(const SoundId& soundId, float const volume)
				: m_SoundId(soundId)
				, m_Volume(volume)
			{

			}

			SoundId m_SoundId{};
			float m_Volume{};
		};
	};
}

#endif