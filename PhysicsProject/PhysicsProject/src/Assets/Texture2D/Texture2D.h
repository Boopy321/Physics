#pragma once
#include <string>

enum class TexFormat
{
	NONE		= 0, // Not loaded
	GREY		= 1,
	GREY_ALPHA	= 2,
	RGB			= 3,
	RGBA		= 4
};

class Texture2D
{
private:
	unsigned int m_id;
	int m_width;
	int m_height;
	TexFormat m_format;

public:
	Texture2D();
	Texture2D(const std::string& a_filePath);
	Texture2D(const Texture2D& a_copy);
	~Texture2D();

	void Bind(const unsigned int& a_unit);

	Texture2D& operator=(const Texture2D& a_copy);

	inline int GetWidth() const { return m_width; }
	inline int GetHeight() const { return m_height; }
	inline TexFormat GetFormat() const { return m_format; }
};