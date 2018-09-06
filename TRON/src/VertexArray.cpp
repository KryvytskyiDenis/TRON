#include "VertexArray.h"

#include "VertexBufferLayout.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer & vb, const VertexBufferLayout & layout)
{
	Bind();
	vb.Bind(); // Bind an actual layout

			   // Set up layout for above bounded buffer
	const auto& elements = layout.GetElements(); // const auto& - read-only access
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		// Specify layout for binded buffer (specify layout means set to buffer what data it will represent)
		GLCall(glEnableVertexAttribArray(i));
		// index 0 of this vertex array is goint to actually be bound to the currently bound GL_ARRAY_BUFFER
		// по сути этот индекс указыват на конкретный VBO в VAO (если предсавить, что VAO это массив где каждый элемент - это отдельный VBO)
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset)); // in other words: this code links buffer with vao 
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}
