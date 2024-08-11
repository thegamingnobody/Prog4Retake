#pragma once
#include "Command.h"

namespace dae
{
    class MuteCommand final : public Command
    {
    public:
        MuteCommand() {}

        void Execute() override;

    };
}

