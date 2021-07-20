#pragma once

#include"vertex_array.h"
#include"core/mat.h"
#include"renderer/mesh.h"

class RenderCommand
{
public:
    ~RenderCommand() = default;
    RenderCommand(const RenderCommand&) = delete;
    RenderCommand& operator=(const RenderCommand&) = delete;

    static void init() { getInstance()->iInit(); }
    static void setClearColor(const Vec4f& color) { getInstance()->iSetClearColor(color); }
    static void clear() { getInstance()->iClear(); }
    static void drawArrays(const Mesh& mesh) { getInstance()->iDrawArrays(mesh); }
    static void drawElements(const Mesh& mesh) { getInstance()->iDrawElements(mesh); }

    static RenderCommand* getInstance()
    {
        static RenderCommand instance;
        return &instance;
    }

private:
    RenderCommand() = default;

    void iInit();
    void iDrawArrays(const Mesh& mesh);
    void iDrawElements(const Mesh& mesh);
    void iSetClearColor(const Vec4f& color);
    void iClear();
};