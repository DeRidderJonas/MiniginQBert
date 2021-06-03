#pragma once
#include "Command.h"

namespace QBert
{
	class KillCommand final : public dae::Command
	{
	public:
		KillCommand(dae::Component* pDestination);
		void Execute() override;
	};

}
