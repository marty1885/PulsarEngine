#include <PulsarTexture.hpp>
using namespace Pulsar;

#include <png++/png.hpp>

Image::Image()
{
}

Image::Image(string path)
{
	load(path);
}

void Image::load(string path, float gamma)
{
	//Clear any previous data. Although in any good programming. This shouldn't happen
	if(buffer != NULL)
		unload();

	//NOTE: I Hate exceptions. Use them as least as possible
	png::image<png::rgba_pixel> image;
	try
	{
		image.read(path);
	}
	catch(png::std_error error)
	{
		cout << "Filed to load image " << path << endl;
	}
	png::rgba_pixel pixel;
	height = image.get_height();
	width = image.get_width();

	buffer = new vec4[width*height];
	for(int i=0;i<height;i++)
		for(int j=0;j<width;j++)
		{
			pixel = image.get_pixel(j,i);
			vec4 colorVec(pixel.red/255.0f,pixel.green/255.0f,pixel.blue/255.0f,pixel.alpha/255.0f);
			const vec4 gammaVec(gamma,gamma,gamma,1);
			colorVec = glm::pow(colorVec,gammaVec);
			buffer[i*width+j] = colorVec;
		}
}

void Image::save(string path)
{
	if(buffer == NULL)
	{
		cout << "No image is not the buffer! can't save image." << endl;
		return;
	}
	png::image<png::rgba_pixel> image;
	try
	{
		png::image<png::rgba_pixel> image(width,height);
		image.set_interlace_type(png::interlace_adam7);
		for(int i=0;i<height;i++)
			for(int j=0;j<width;j++)
			{
				image[i][j] = png::rgba_pixel(buffer[i*width+j].r*255,buffer[i*width+j].g*255
					,buffer[i*width+j].b*255,buffer[i*width+j].w*255);
			}
		image.write(path);
	}
	catch(...)
	{
		cout << "Failed to write to image " << path << endl;
	}
}

void Image::unload()
{
	delete [] buffer;
	width = 0;
	height = 0;
	buffer = NULL;
}

Image::~Image()
{
	unload();
}

vec4* Image::getRaw()
{
	return buffer;
}

bool Image::good()
{
	if(buffer != NULL)
		return true;
	return false;
}

int Image::getWidth()
{
	return width;
}

int Image::getHeight()
{
	return height;
}

Texture::Texture()
{
}

Texture::~Texture()
{
	unload();
}

bool Texture::load(Image* image)
{
	if(id != 0)
	{
		cout << "Warrning : There is a texture currently valid but trying to load a new texture on. It will be overrided" << endl;
		unload();
	}

	if(image == NULL)
		return false;
	if(image->good() == false)
		return false;

	glGenTextures(1,&id);
	glBindTexture(GL_TEXTURE_2D, id);

	//Try to not use the old APIs
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, image->getWidth(), image->getHeight(), 0, GL_RGBA, GL_FLOAT, image->getRaw());

	//NOTE : The "levels" argument must be n^2 on AMD GPU + Mesa Driver. not sure if required on AMD priority drivers.
	//FIXME : The following commented code is for OpenGL 4.2+. Use it if we want 4.2+ support
	// glTexStorage2D(GL_TEXTURE_2D, 4, GL_RGBA8, image->getWidth(), image->getHeight());
	// glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image->getWidth(), image->getHeight(), GL_RGBA, GL_FLOAT, image->getRaw());

	glGenerateMipmap(GL_TEXTURE_2D);
	enableMipmap(false);

	//Texture flags
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);
	return true;
}

void Texture::unload()
{
	glDeleteTextures(1,&id);
	id = 0;
}

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::enableMipmap(bool enable)
{
	bind();

	if(enable == true)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.47);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	}
	unbind();
}
