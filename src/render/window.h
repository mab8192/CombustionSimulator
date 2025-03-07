#pragma once

#include <memory>
#include <string>

#include "../engine.h"

class Window
{
public:
    Window();
    ~Window();

    void Init(int width, int height, const std::string &title);
    void Close();
    void SetEngine(std::shared_ptr<Engine> engine);
    void Draw();

    bool ShouldClose() const;

private:
    bool m_Shutdown = false;

    std::shared_ptr<Engine> m_Engine;
};
