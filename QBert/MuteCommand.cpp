#include "MuteCommand.h"
#include <ServiceLocator.h>

void dae::MuteCommand::Execute()
{
	dae::ServiceLocator::GetSoundSystem().ToggleMute();
}
