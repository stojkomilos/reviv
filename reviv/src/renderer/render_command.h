#pragma once

#include"vertex_array.h"
#include"core/mat.h"

class RenderCommand
{
public:
    ~RenderCommand() = default;
    RenderCommand(const RenderCommand&) = delete;
    RenderCommand& operator=(const RenderCommand&) = delete;

    static void init() { getInstance()->iInit(); }
    static void setClearColor(const Vec4f& color) { getInstance()->iSetClearColor(color); }
    static void clear() { getInstance()->iClear(); }
    static void drawArrays(const Vao& object) { getInstance()->iDrawArrays(object); }

    static RenderCommand* getInstance()
    {
        static RenderCommand instance;
        return &instance;
    }

private:
    RenderCommand() = default;

    void iInit();
    void iDrawArrays(const Vao& object);
    void iSetClearColor(const Vec4f& color);
    void iClear();
};