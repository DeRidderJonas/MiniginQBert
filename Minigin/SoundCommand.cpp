#include "MiniginPCH.h"
#include "SoundCommand.h"

#include "ServiceLocator.h"

void QBert::SoundCommand::Execute()
{
	dae::ServiceLocator::GetSoundSystem().QueueSound(0);
}
