#pragma once

#include <vector>

#include "OpenGLMinecraft/MultiMedia/Graphics/GLCommon.h"
#include "OpenGLMinecraft/Debug.h"

struct VertexBufferElement 
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;
	static unsigned int GetSizeOfType(unsigned int type) 
	{
		switch (type) 
		{
			case GL_FLOAT:
				return 4;
			case GL_UNSIGNED_INT:
				return 4;
			case GL_UNSIGNED_BYTE:
				return 1;
		}
		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout 
{
public:
	VertexBufferLayout()
	: m_Stride(0) {}
	~VertexBufferLayout() {}

	inline void Reset() { m_Stride = 0; m_Elements.clear(); }

	template<typename T>
	void Push(unsigned int p_Count) {}

	template<>
	void Push<float>(unsigned int p_Count)
	{
		m_Elements.push_back({ GL_FLOAT, p_Count, GL_FALSE });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * p_Count;
	}

	template<>
	void Push<unsigned int>(unsigned int p_Count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, p_Count, GL_FALSE });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * p_Count;
	}

	template<>
	void Push<unsigned char>(unsigned int p_Count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, p_Count, GL_TRUE });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * p_Count;
	}

	inline const std::vector<VertexBufferElement>& GetElements() const& { return m_Elements;  }
	inline unsigned int GetStride() const { return m_Stride;  }
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
};
