#ifndef COUNTERSCOMMAND
#define COUNTERSCOMMAND

#include "Command.h"
#include "CountersComponent.h"

namespace dae
{
	class CountersCommand final : public Command
	{
	public:
		CountersCommand(int const playerNumber, const std::string& targetCounterName, int const addedValue);

		void Execute() override;

	private:
		std::string m_TargetCounterName;
		int m_AddedValue;
	};
}

#endif