#pragma once

class IndexBuffer 
{
public:
	IndexBuffer(const unsigned int* p_Data, size_t p_IndexCount);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;
private:
	unsigned int m_RendererID;
};
