#include "MiniginPCH.h"
#include "KillCommand.h"
#include "HealthComponent.h"

QBert::KillCommand::KillCommand(dae::Component* pDestination)
	: Command{pDestination}
{
}

void QBert::KillCommand::Execute()
{
	static_cast<HealthComponent*>(m_pComponent)->Kill();
}
