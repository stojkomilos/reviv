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

    static void init() { get()->iInit(); }
    static void setClearColor(const Vec4f& color) { get()->iSetClearColor(color); }
    static void clear() { get()->iClear(); }
    static void drawArrays(const Mesh& mesh) { get()->iDrawArrays(mesh); }
    static void drawElements(const Mesh& mesh) { get()->iDrawElements(mesh); }

    static RenderCommand* get()
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