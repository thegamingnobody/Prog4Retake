#pragma once
#include "Command.h"

namespace dae
{
    class SkipLevelCommand final : public Command
    {
    public:
        SkipLevelCommand() {}

        void Execute() override;

    };
}

