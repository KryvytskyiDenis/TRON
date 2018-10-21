#include "Renderer.h"

#include <iostream>

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << "): " << function <<
			" " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

void Renderer::Clear()
{
	glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray& va, /*const IndexBuffer& ib, */const Shader& shader, const int& size) const
{
	shader.Bind();
	va.Bind();
	//ib.Bind();

	/*GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));*/
	//DrawLines(size);
}

void Renderer::DrawLines(const VertexArray& va, /*const IndexBuffer& ib, */const Shader& shader, const int& size) const
{
	shader.Bind();
	va.Bind();
	GLCall(glDrawArrays(GL_LINES, 0, size));
}

void Renderer::DrawLineStrip(const VertexArray& va, /*const IndexBuffer& ib, */const Shader& shader, const int& size) const
{
	shader.Bind();
	va.Bind();
	GLCall(glDrawArrays(GL_LINE_STRIP, 0, size));
}
