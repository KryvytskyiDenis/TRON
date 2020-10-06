#pragma once

#include "Renderer/VertexBufferLayout.h"

class VertexBuffer
{
public:
    VertexBuffer(const void* data, uint32_t size);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;

    void SetLayout(const VertexBufferLayout& layout) { m_Layout = layout; }
    const VertexBufferLayout& GetLayout() const { return m_Layout; }

private:
    uint32_t m_RendererId;
    VertexBufferLayout m_Layout;
};

