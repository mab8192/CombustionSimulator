#include "engine.h"

Engine::Engine(std::shared_ptr<EngineConfiguration> config)
    : m_Config(config)
{
}

std::shared_ptr<EngineConfiguration> Engine::GetConfiguration() const
{
    return m_Config;
}

void Engine::UpdateConfiguration(std::shared_ptr<EngineConfiguration> config)
{
    m_Config = config;
}

float Engine::ComputeThrust() const
{
    return 0.0f;
}

float Engine::ComputeExhaustVelocity() const
{
    return 0.0f;
}
