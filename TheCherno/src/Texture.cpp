#include "Texture.h"
#include "std_image/stb_image.h"

Texture::Texture(const std::string& path)
	:m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
	// Loading image
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4); // Now we have all of the texure data inside m_LocalBuffer

	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	// Setting up some parameters for texture. These must be setup!
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)); // Linearly resample everything --> GL_NEAREST is other option
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)); // Horizontal wrap
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE)); // Vertical wrap

	// Sending pixel data to openGL
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	// RGBA8 --> 8 == number of bits per channel --> Only in internalFormat param
	// Format param doesnt use number --> just RGBA
	// Type is type of data --> in out cases its unsigned byte or char
	// Pointer param is the pointer to texture data

	GLCall(glBindTexture(GL_TEXTURE_2D, 0)); // When we create texture we unbind it until we want to use it!

	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer); // We put the data in the texture buffer so we can now free the local buffer! ( But we dont have to do this )
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot /* = 0*/) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
