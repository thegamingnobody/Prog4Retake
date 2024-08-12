#ifndef DAE_EVENT
#define DAE_EVENT

#include <any>
#include <Tuple>
#include <iostream>

namespace dae
{
	enum class EventType
	{
		MoveObject,
		TransformChanged,
		RequestMovement,
		IsTileValid,
		ConfirmMovement,
		MoveObjectTo,
		MoveFinished,

		ToggleTile,

		PlayerDied,

		RespawnPlayer,

		PlaySound,
		StopSound,
		StopAllSounds,

		LoadNextRound,
		GameOver,
		StartRound,
	};

	struct Event
	{
		template<class... EventArguments>
		Event(const EventType& eventType, std::tuple<EventArguments...> arguments, int const playerNumber)
			: m_type(eventType)
			, m_args(arguments)
			, m_playerNumber(playerNumber)
		{
		}

		template<class... EventArguments>
		std::tuple<EventArguments...> GetArgumentsAsTuple() const
		{
			try
			{
				return std::any_cast<std::tuple<EventArguments...>>(m_args);
			}
			catch (const std::bad_any_cast& e)
			{
				std::cout << "Invalid Event Arguments!\n";
				throw e;
			}
		}

		EventType m_type;
		int const m_playerNumber;
	private:
		std::any m_args;
	};
}

#endif