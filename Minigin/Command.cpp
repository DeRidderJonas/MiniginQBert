#include "MiniginPCH.h"
#include "Command.h"

dae::Command::Command(GameObject* pDestination)
	: m_pGameObject{pDestination}
	, m_pComponent{nullptr}
{
}

dae::Command::Command(Component* pDestination)
	: m_pComponent{pDestination}
	, m_pGameObject{nullptr}
{
}
