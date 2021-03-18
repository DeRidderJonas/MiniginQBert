#include "MuteCommand.h"

#include "ServiceLocator.h"

void MuteCommand::Execute()
{
	dae::ServiceLocator::GetSoundSystem().ToggleMute();
}
