#ifndef PULSARTEXTIRE_HPP
#define PULSARTEXTIRE_HPP

#include <PulsarMath.hpp>

#include <iostream>
#include <GL/glew.h>

namespace Pulsar
{
using namespace std;
//Now only support PNG files
class Image
{
public:
	Image();
	Image(string path);
	virtual ~Image();
	void load(string path);
	void save(string path);
	vec4* getRaw();
	int getWidth();
	int getHeight();
	void unload();

	bool good();

protected:
	vec4* buffer = NULL;
	int width, height;
};


class Texture
{
public:
	Texture();
	virtual ~Texture();

	bool load(Image* img);
	void unload();
	void bind();
	void unbind();

	void enableMipmap(bool enable);
protected:
	GLuint id = 0;
};
}

#endif
