#pragma once

#include "Renderer/VertexBuffer.h"

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void Bind();
    void Unbind();

    void AddVertexBuffer(VertexBuffer* vertexBuffer);

private:
    uint32_t m_RendererId;
};

