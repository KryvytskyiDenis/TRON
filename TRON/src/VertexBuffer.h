#pragma once

// VBO
class VertexBuffer
{
private:
	unsigned int m_RendererID; // unique id (At high level as in OpenGL indices)
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};