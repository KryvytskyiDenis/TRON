#pragma once

#include "Renderer/VertexArray.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Shader.h"

class Renderer
{
public:
    void PreRender() const;
    void Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const std::shared_ptr<Shader> shader);
};

