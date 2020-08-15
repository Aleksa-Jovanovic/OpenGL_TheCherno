#pragma once

class IndexBuffer
{
private:
	unsigned int m_RendererID; 
	unsigned int m_Count; //Number of indeces!
public:
	IndexBuffer(const unsigned int* data, unsigned int count); //We use count here meaning number of elements (we no type of data so we dont need size in bytes)
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_Count; }
};