#pragma once

#include<stb_image/stb_image.h>
#include<string>
#include<assert.h>
#include"opengl_assert.h"

class Texture
{
public:
	virtual unsigned int getWidth() = 0;
	virtual unsigned int getHeight() = 0;
	virtual void bind(unsigned int slot = 0) = 0;
	unsigned int width;
	unsigned int height;
};

class Texture2D : public Texture
{
public:
	Texture2D();
	~Texture2D();
	unsigned int getWidth() override;
	unsigned int getHeight() override;
	void bind(unsigned int slot) override;

	void setUp(const std::string& path);
	std::string path;
	unsigned int width, height;
	unsigned int ID;
}; //destruktor