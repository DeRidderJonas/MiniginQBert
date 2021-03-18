#pragma once
#include "Command.h"

class MuteCommand : public dae::Command
{
public:
	~MuteCommand() override = default;
	void Execute() override;
};

