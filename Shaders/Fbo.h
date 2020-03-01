#pragma once

#include <gl/glew.h>

class Fbo
{
public:
	GLuint fboTextureId;
	GLuint fboId;
	int width, height;


	Fbo(int width, int height);



	void bind();
	void unbind();
	void use();

};