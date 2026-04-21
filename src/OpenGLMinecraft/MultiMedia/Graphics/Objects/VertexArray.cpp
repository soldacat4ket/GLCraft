#include "VertexArray.h"
#include "VertexBufferLayout.h"

#include "OpenGLMinecraft/MultiMedia/Graphics/GLCommon.h"

VertexArray::VertexArray() 
{
	glGenVertexArrays(1, &m_RendererID);
	Bind();
}

VertexArray::~VertexArray() 
{
    glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::AddBuffer(const VertexBuffer& p_VB, const VertexBufferLayout& p_Layout) 
{
	Bind();
	p_VB.Bind();
	const auto& Elements = p_Layout.GetElements();
	unsigned int Offset = 0;

	for (unsigned int i = 0; i < Elements.size(); i++) {
		const auto& Element = Elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(
			i, 
			Element.count,
			Element.type,
			Element.normalized,
			p_Layout.GetStride(), reinterpret_cast<const void*>(static_cast<uintptr_t>(Offset)));
		Offset += Element.count * VertexBufferElement::GetSizeOfType(Element.type);
	}
}

void VertexArray::Bind() const 
{
	glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const 
{
	glBindVertexArray(0);
}