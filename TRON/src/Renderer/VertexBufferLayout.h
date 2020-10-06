#pragma once

enum class BufferAttributeType
{
    None,
    Float,
    Float2,
    Float3,
    Float4,
    Int,
    Int2,
    Int3,
    Int4,
    Bool
};

static uint32_t BufferAttributeTypeSize(BufferAttributeType type)
{
    switch (type)
    {
        case BufferAttributeType::Float: return sizeof(float);
        case BufferAttributeType::Float2: return 2 * sizeof(float);
        case BufferAttributeType::Float3: return 3 * sizeof(float);
        case BufferAttributeType::Float4: return 4 * sizeof(float);
        case BufferAttributeType::Int: return sizeof(int);
        case BufferAttributeType::Int2: return 2 * sizeof(int);
        case BufferAttributeType::Int3: return 3 * sizeof(int);
        case BufferAttributeType::Int4: return 4 * sizeof(int);
        case BufferAttributeType::Bool: return sizeof(bool);
    }

    ERROR("Unknown BufferttributeType!");
    TR_ASSERT(false);
    return 0;
}

struct VertexBufferAttribute
{
    BufferAttributeType Type;
    uint32_t Size;
    size_t Offset;
    bool Normalized;

    VertexBufferAttribute() = default;

    VertexBufferAttribute(BufferAttributeType type, bool normalized = false);

    uint32_t GetComponentsCount() const
    {
        switch (Type)
        {
            case BufferAttributeType::Float: return 1;
            case BufferAttributeType::Float2: return 2;
            case BufferAttributeType::Float3: return 3;
            case BufferAttributeType::Float4: return 4;
            case BufferAttributeType::Int: return 1;
            case BufferAttributeType::Int2: return 2;
            case BufferAttributeType::Int3: return 3;
            case BufferAttributeType::Int4: return 4;
            case BufferAttributeType::Bool: return 1;
        }

        ERROR("Unknown BufferttributeType!");
        TR_ASSERT(false);
        return 0;
    }
};


class VertexBufferLayout
{
public:
    VertexBufferLayout();

    VertexBufferLayout(const std::initializer_list<VertexBufferAttribute>& attributes);

    void CalculateOffsetsAndStride();

    uint32_t GetStride() const { return m_Stride; }
    const std::vector<VertexBufferAttribute>& GetAttributes() const { return m_Attributes; }

private:
    std::vector<VertexBufferAttribute> m_Attributes;
    uint32_t m_Stride;
};

