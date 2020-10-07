#include "trpch.h"
#include "Renderer/Renderer.h"

void Renderer::PreRender() const
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const std::shared_ptr<Shader> shader)
{
    shader->Bind();
    vertexArray.Bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
