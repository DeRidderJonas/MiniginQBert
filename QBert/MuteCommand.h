#pragma once
#include "Command.h"

class MuteCommand final : public dae::Command
{
public:
	~MuteCommand() override = default;
	void Execute() override;
};

