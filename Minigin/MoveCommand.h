#pragma once

#include "Command.h"
#include <glm/ext/vector_float3.hpp>

namespace dae
{
    class MoveCommand final : public Command
    {
    public:
        MoveCommand(int const playerNumber, glm::vec3 direction);

        void Execute() override;

    private:
        glm::vec3 m_Direction;
    };
}

