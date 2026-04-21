#pragma once

#include <filesystem>

class Texture 
{
public:
	Texture(const std::filesystem::path& p_Path);
	~Texture();

	void Bind(unsigned int p_Slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
    inline const std::filesystem::path& GetPath() const { return m_FilePath; }
private:
    unsigned int m_RendererID;
    const std::filesystem::path m_FilePath;
    int m_Width, m_Height, m_BPP;
};

