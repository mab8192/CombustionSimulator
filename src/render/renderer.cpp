#include <iostream>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <stdexcept>

#include "renderer.h"

Renderer::Renderer() : window(nullptr), vao(0), vbo(0), shaderProgram(0) {}

Renderer::~Renderer()
{
    Shutdown();
}

void Renderer::Init(int width, int height, const std::string &title)
{
    // Initialize GLFW
    if (!glfwInit())
        throw std::runtime_error("Failed to initialize GLFW");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    glViewport(0, 0, width, height);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set up vertex buffers
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    CompileShaders();

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");
}

GLFWwindow *Renderer::GetWindow() const
{
    return window;
}

void Renderer::CompileShaders()
{
    const char *vertexShaderSource = R"(
        #version 450 core
        layout(location = 0) in vec2 aPos;
        void main() {
            gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
        }
    )";

    const char *fragmentShaderSource = R"(
        #version 450 core
        out vec4 FragColor;
        void main() {
            FragColor = vec4(1.0, 1.0, 1.0, 1.0); // Orange color to make it more visible
        }
    )";

    // Compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    // Check for vertex shader compilation errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // Compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // Check for fragment shader compilation errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // Link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Renderer::BeginRender()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Renderer::DrawEngine(const Engine &rocket)
{
    const std::vector<float> &vertices = rocket.GetVertices();

    if (vertices.empty())
    {
        return; // Safety check
    }

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glUseProgram(shaderProgram);

    // Draw using triangles - each shape is now made of triangles
    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 2);

    glBindVertexArray(0);
}

void Renderer::DrawImGui()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Renderer::Shutdown()
{
    if (m_Shutdown) return;

    m_Shutdown = true;

    try
    {
        // First shutdown ImGui
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        // Only delete GL resources if we have a valid context
        if (window && glfwGetCurrentContext() == window)
        {
            // Delete OpenGL resources
            if (vao)
            {
                glDeleteVertexArrays(1, &vao);
                vao = 0;
            }
            if (vbo)
            {
                glDeleteBuffers(1, &vbo);
                vbo = 0;
            }
            if (shaderProgram)
            {
                glDeleteProgram(shaderProgram);
                shaderProgram = 0;
            }
        }

        // Finally destroy the window and terminate GLFW
        if (window)
        {
            glfwDestroyWindow(window);
            window = nullptr;
        }

        // Only call terminate once, and only if it was initialized
        if (glfwGetCurrentContext() != nullptr)
        {
            glfwTerminate();
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error during renderer shutdown: " << e.what() << std::endl;
    }
}

bool Renderer::ShouldClose() const
{
    return glfwWindowShouldClose(window);
}