#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"

#include "Renderer.hpp"

VertexArray::VertexArray()
	: m_RendererID(0)
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	// !!! No GLCall() because destructor is non-throwing function !!!
	glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	m_LastStride = layout.GetStride();
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)(uintptr_t)(offset)));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}