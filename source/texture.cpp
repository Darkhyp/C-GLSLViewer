#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>


Texture::Texture(string imageName, GLenum textureType, GLuint slot, GLenum pixelType)
{
	type = textureType;
	unit = slot;

	// store width, height and number of chanels in the image
	int image_width, image_height, image_numColorChanels;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* image_bytes = stbi_load(imageName.c_str(), &image_width, &image_height, &image_numColorChanels, 0);

	// Generate openGL texture object
	glGenTextures(1, &ID);
	// Assign texture to a texture Unit
	glActiveTexture(GL_TEXTURE0 + slot);
	Bind();

	// configure the algorithm that us used to make image smaller or bigger 
	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// configure the way to image repeat (if it does at all)
	glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// for boarder creation (GL_CLAMP_TO_BOARDER)
	//	float flatColors[] = { 1., 1., 1., 1. };
	//	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColors);

	// Assign image to OpenGL texture object
	if(image_numColorChanels == 3)
		glTexImage2D(type, 0, GL_RGB, image_width, image_height, 0, GL_RGB, pixelType, image_bytes);
	else if (image_numColorChanels == 4)
		glTexImage2D(type, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, pixelType, image_bytes);
	glGenerateMipmap(type);

	// delete image
	stbi_image_free(image_bytes);

}

Texture::~Texture()
{
	glDeleteTextures(1, &ID);
}

void Texture::textureInit(Shader* shader, const char* uniform, GLuint unit)
{
	// shader should be activated before!
	GLuint tex0 = glGetUniformLocation(shader->ID, uniform);
	glUniform1i(tex0, unit);

}

void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(type, ID);
}

void Texture::Unbind()
{
	glBindTexture(type, 0);
}
