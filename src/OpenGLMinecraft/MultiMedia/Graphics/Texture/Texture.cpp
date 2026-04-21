#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "OpenGLMinecraft/MultiMedia/Graphics/GLCommon.h"
#include "OpenGLMinecraft/Debug.h"
#include "OpenGLMinecraft/Utility/Log.h"

Texture::Texture(const std::filesystem::path& p_Path)
	:m_FilePath(p_Path), m_Width(0), m_Height(0), m_BPP(0) {

	//stbi_set_flip_vertically_on_load(1); // idk why this isnt needed but it works
	unsigned char* LocalBuffer = stbi_load(m_FilePath.string().c_str(), &m_Width, &m_Height, &m_BPP, 4);
    if(!LocalBuffer)
    {
        LOG_ERROR("Cannot load {}: {}", m_FilePath.string(), stbi_failure_reason());
        ASSERT(false);
    }

	glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, LocalBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(LocalBuffer);

    LOG_INFO("Loaded texture {}, image res: {}x{} BPP: {}", m_FilePath.string(), m_Width, m_Height, m_BPP);
}

Texture::~Texture() 
{
	glDeleteTextures(1, &m_RendererID);
}

void Texture::Bind(const unsigned int p_Slot) const 
{
	glActiveTexture(GL_TEXTURE0 + p_Slot);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture::Unbind() const 
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
