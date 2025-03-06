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

void Engine::CalculateGeometry()
{
    m_Vertices.clear();
    // Use normalized device coordinates (-1 to 1)
    float halfWidth = 0.2f; // Smaller values to fit in screen

    float scale = 0.1f;
    float nozzleHalfWidth = m_Config->nozzleDiameter * scale / 2.0f;
    float chamberLength = m_Config->chamberLength * scale;
    float throatHalfWidth = m_Config->throatDiameter * scale / 2.0f;

    // Combustion chamber (rectangle as two triangles)
    // First triangle (bottom-left, bottom-right, top-right)
    m_Vertices.push_back(-halfWidth);
    m_Vertices.push_back(-0.5f);

    m_Vertices.push_back(halfWidth);
    m_Vertices.push_back(-0.5f);

    m_Vertices.push_back(halfWidth);
    m_Vertices.push_back(-0.5f + chamberLength);

    // Second triangle (bottom-left, top-right, top-left)
    m_Vertices.push_back(-halfWidth);
    m_Vertices.push_back(-0.5f);

    m_Vertices.push_back(halfWidth);
    m_Vertices.push_back(-0.5f + chamberLength);

    m_Vertices.push_back(-halfWidth);
    m_Vertices.push_back(-0.5f + chamberLength);

    // Nozzle (trapezoid as two triangles)
    // First triangle
    m_Vertices.push_back(throatHalfWidth);
    m_Vertices.push_back(-0.5f);

    m_Vertices.push_back(-throatHalfWidth);
    m_Vertices.push_back(-0.5f);

    m_Vertices.push_back(-nozzleHalfWidth);
    m_Vertices.push_back(-0.8f);

    // Second triangle
    m_Vertices.push_back(throatHalfWidth);
    m_Vertices.push_back(-0.5f);

    m_Vertices.push_back(-nozzleHalfWidth);
    m_Vertices.push_back(-0.8f);

    m_Vertices.push_back(nozzleHalfWidth);
    m_Vertices.push_back(-0.8f);
}

const std::vector<float> &Engine::GetVertices() const
{
    return m_Vertices;
}