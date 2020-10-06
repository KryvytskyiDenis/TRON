#include "trpch.h"
#include "Renderer/VertexArray.h"

#include <glad/glad.h>

static GLenum BufferAttributeTypeToOpenGLBaseType(BufferAttributeType type)
{
    switch (type)
    {
        case BufferAttributeType::Float: return GL_FLOAT;
        case BufferAttributeType::Float2: return GL_FLOAT;
        case BufferAttributeType::Float3: return GL_FLOAT;
        case BufferAttributeType::Float4: return GL_FLOAT;
        case BufferAttributeType::Int: return GL_INT;
        case BufferAttributeType::Int2: return GL_INT;
        case BufferAttributeType::Int3: return GL_INT;
        case BufferAttributeType::Int4: return GL_INT;
        case BufferAttributeType::Bool: return GL_BOOL;
    }

    ERROR("Unknown BufferttributeType!");
    TR_ASSERT(false);
    return 0;
}

VertexArray::VertexArray()
    : m_RendererId(0)
{
    glCreateVertexArrays(1, &m_RendererId);
}

VertexArray::~VertexArray()
{
}

void VertexArray::Bind()
{
    glBindVertexArray(m_RendererId);
}

void VertexArray::Unbind()
{
    glBindVertexArray(0);
}

void VertexArray::AddVertexBuffer(VertexBuffer* vertexBuffer)
{
    Bind();
    vertexBuffer->Bind();

    const VertexBufferLayout& layout = vertexBuffer->GetLayout();
    uint32_t vertexAttributeIndex = 0;
    size_t offset = 0;
    for (const auto& attribute : layout.GetAttributes())
    {
        glEnableVertexAttribArray(vertexAttributeIndex);
        glVertexAttribPointer(vertexAttributeIndex, attribute.GetComponentsCount(), BufferAttributeTypeToOpenGLBaseType(attribute.Type)
            , attribute.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)offset);
        offset += attribute.Offset;
    }
}
