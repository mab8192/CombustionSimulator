#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <vector>
#include <string>

#include "../engine.h"

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void Init(int width, int height, const std::string &title);
    GLFWwindow *GetWindow() const;
    void BeginRender();
    void Shutdown();
    void DrawEngine(const Engine &engine);
    void DrawImGui();
    bool ShouldClose() const;

private:
    GLFWwindow *window;
    GLuint vao, vbo, shaderProgram;
    bool m_Shutdown = false;
    void CompileShaders();
};
