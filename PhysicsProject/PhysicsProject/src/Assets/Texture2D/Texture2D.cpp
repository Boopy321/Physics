#include "Texture2D.h"
#include "stb\stb_image.h"
#include <gl_core_4_4.h>
#include <iostream>


Texture2D::Texture2D()
{
	m_id		= 0;
	m_width		= 0;
	m_height	= 0;
	m_format	= TexFormat::NONE;
}

Texture2D::Texture2D(const Texture2D& a_copy)
{
	m_id		= a_copy.m_id;
	m_width		= a_copy.m_width;
	m_height	= a_copy.m_height;
	m_format	= a_copy.m_format;
}

Texture2D::Texture2D(const std::string& a_filePath)
{
	m_width = 0;
	m_height = 0;
	int format = 0;

	// Load file data
	unsigned char* data = stbi_load(a_filePath.c_str(), &m_width, &m_height, 
		&format, STBI_default);

	unsigned int glFormat = GL_RGB;
	switch (format)
	{
	case STBI_grey:
		m_format = TexFormat::GREY;
		break;
	case STBI_grey_alpha:
		m_format = TexFormat::GREY_ALPHA;
		break;

	case STBI_rgb:
		m_format = TexFormat::RGB;
		break;

	case STBI_rgb_alpha:
		m_format = TexFormat::RGBA;
		glFormat = GL_RGBA;
		break;
	}

	// Generate OpenGL texture
	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);
	glTexImage2D(GL_TEXTURE_2D, 0, glFormat, m_width, m_height, 0,
		glFormat, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	stbi_image_free(data);
}

Texture2D::~Texture2D()
{
	glDeleteTextures(1, &m_id);
}

Texture2D& Texture2D::operator=(const Texture2D& a_copy)
{
	m_id = a_copy.m_id;
	m_width = a_copy.m_width;
	m_height = a_copy.m_height;
	m_format = a_copy.m_format;

	return *this;
}

void Texture2D::Bind(const unsigned int& a_unit)
{
	if (a_unit > 31)
	{
		std::cerr << "Texture Unit Bind out of range..." << std::endl;
		return; // Texture Unit out of range
	}
	glActiveTexture(GL_TEXTURE0 + a_unit);
	glBindTexture(GL_TEXTURE_2D, m_id);
}