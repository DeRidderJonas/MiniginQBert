#pragma once
#include "Command.h"

namespace QBert
{
	class SoundCommand final : public dae::Command
	{
	public:
		~SoundCommand() override = default;
		void Execute() override;
	};

}
