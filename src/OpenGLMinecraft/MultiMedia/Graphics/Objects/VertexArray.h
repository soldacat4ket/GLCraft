#pragma once

#include "OpenGLMinecraft/MultiMedia/Graphics/Objects/VertexBuffer.h"

class VertexBufferLayout;

class VertexArray 
{
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& p_VB, const VertexBufferLayout& p_Layout);

	void Bind() const;
	void Unbind() const;

private:
    unsigned int m_RendererID;
};
