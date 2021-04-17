#pragma once
#include "Command.h"

namespace QBert
{
	class KillCommand : public dae::Command
	{
	public:
		KillCommand(dae::Component* pDestination);
		void Execute() override;
	};

}
