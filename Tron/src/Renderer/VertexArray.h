#pragma once

#include "Renderer/VertexBuffer.h"

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void Bind() const;
    void Unbind() const;

    void AddVertexBuffer(VertexBuffer* vertexBuffer);

private:
    uint32_t m_RendererId;
};

