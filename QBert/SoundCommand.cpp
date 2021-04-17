#include "MiniginPCH.h"
#include "SoundCommand.h"

#include "ServiceLocator.h"

void QBert::SoundCommand::Execute()
{
	dae::ServiceLocator::GetSoundSystem().Play(0);
}
