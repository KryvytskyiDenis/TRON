#include "trpch.h"
#include "Renderer/VertexBufferLayout.h"

VertexBufferAttribute::VertexBufferAttribute(BufferAttributeType type, bool normalized)
    : Type(type)
    , Size(BufferAttributeTypeSize(type))
    , Offset(0)
    , Normalized(normalized)
{
}

VertexBufferLayout::VertexBufferLayout()
    : m_Stride(0)
{
}

VertexBufferLayout::VertexBufferLayout(const std::initializer_list<VertexBufferAttribute>& attributes)
    : m_Attributes(attributes)
{
    CalculateOffsetsAndStride();
}

void VertexBufferLayout::CalculateOffsetsAndStride()
{
    size_t offset = 0;
    m_Stride = 0;
    for (auto& attribute : m_Attributes)
    {
        attribute.Offset = offset;
        offset += attribute.Size;
        m_Stride += offset;
    }
}
