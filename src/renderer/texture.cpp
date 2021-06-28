#include "texture.h"
unsigned int Texture2D::getHeight()
{
	return height;
}
unsigned int Texture2D::getWidth()
{
	return width;
}
void Texture2D::setUp(const std::string& path)
{
	int width1, height1, channels1;
	stbi_uc* data = stbi_load(path.c_str(), &width1, &height1, &channels1, 0);
	stbi_set_flip_vertically_on_load(1);
	if (!data) {
		std::cout << "ERROR TEXTURE FAILED TO LOAD: " << path << std::endl;
		assert(false);
	}
	else {
		std::cout << "Loaded texture " << path << std::endl;
	}
	width = width1;
	height = height1;

	GLenum internalFormat = 0, dataFormat = 0;
	if (channels1 == 4)
	{
		internalFormat = GL_RGBA8;
		dataFormat = GL_RGBA;
	}
	else if (channels1 == 3)
	{
		internalFormat = GL_RGB8;
		dataFormat = GL_RGB;
	}
	if (!(internalFormat & dataFormat))
	{
		std::cout << "ERROR Texture format not supported\n";
		assert(false);
	}


	GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &ID));
	GLCall(glTextureStorage2D(ID, 1, internalFormat, width, height));

	GLCall(glTextureParameteri(ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTextureParameteri(ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));

	GLCall(glTextureSubImage2D(ID, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data));

	stbi_image_free(data);

}
Texture2D::Texture2D()
{
}
Texture2D::~Texture2D()
{
	GLCall(glDeleteTextures(1, &ID));
}
void Texture2D::bind(unsigned int slot)
{
	GLCall(glBindTextureUnit(slot, ID));
}