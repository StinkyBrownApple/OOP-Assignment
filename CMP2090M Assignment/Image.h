#pragma once
#include <vector>
#include <string>
#include "Maths.h"
#include <iostream>

#ifndef image
#define image


//*********************************************
//Image class to hold and allow manipulation of images once read into the code
//from https://www.scratchapixel.com/
//********************************************* 

class Image
{
public:
	// Rgb structure, i.e. a pixel 
	struct Rgb
	{
		Rgb() : r(0), g(0), b(0) {}
		Rgb(float c) : r(c), g(c), b(c) {}
		Rgb(float _r, float _g, float _b) : r(_r), g(_g), b(_b) {}
		bool operator != (const Rgb &c) const
		{
			return c.r != r || c.g != g || c.b != b;
		}
		Rgb& operator *= (const Rgb &rgb)
		{
			r *= rgb.r, g *= rgb.g, b *= rgb.b; return *this;
		}
		Rgb& operator += (const Rgb &rgb)
		{
			r += rgb.r, g += rgb.g, b += rgb.b; return *this;
		}
		friend float& operator += (float &f, const Rgb rgb)
		{
			f += (rgb.r + rgb.g + rgb.b) / 3.f; return f;
		}
		float r, g, b;
	};

	Image() : w(0), h(0), pixels(nullptr), depth(0) { /* empty image */ }
	Image(const unsigned int &_w, const unsigned int &_h, const unsigned int &_depth, const std::string &_path, const Rgb &c = kBlack);
	const Rgb& operator [] (const unsigned int &i) const
	{
		return pixels[i];
	}
	Rgb& operator [] (const unsigned int &i)
	{
		return pixels[i];
	}
	~Image();
	Rgb *pixels; // 1D array of pixels 
	static const Rgb kBlack, kWhite, kRed, kGreen, kBlue; // Preset colors
	virtual void OutputDetails(std::string);

	unsigned int Width() const;
	unsigned int Height() const;
	unsigned int Depth() const;
	std::string Path() const;
private:
	const unsigned int w, h; // Image resolution 
	const unsigned int depth; //Image colour depth
	const std::string path; //Image filepath
};

class BlendedImage : public Image {
public:
	enum BlendMode {
		Mean, Median, SigmaClippedMean
	};
	BlendedImage(std::vector<Image*> images, BlendMode mode, bool useMultiThread = true, std::string path = "OutputImages/output.ppm");
	void OutputDetails(std::string);
	BlendMode Mode() const;
private:
	void Blend(std::vector<Image*>);
	void BlendRed(std::vector<Image*>);
	void BlendGreen(std::vector<Image*>);
	void BlendBlue(std::vector<Image*>);
	void MultiThreadBlend(std::vector<Image*>);
	const BlendMode blendMode;
};

class ZoomedImage : public Image {
public:
	enum ZoomMode {
		NearestNeighbour, Bilinear
	};
	ZoomedImage(Image* imageToZoom, int scale, ZoomMode mode, std::string path = "OutputImages/output.ppm");
	ZoomedImage(Image* imageToZoom, int scale, ZoomMode mode, int x1, int y1, int x2, int y2, std::string path = "OutputImages/output.ppm");

	void OutputDetails(std::string);
	ZoomMode Mode() const;
private:
	void NearestNeighbourZoom();
	void BilinearZoom();
	void InterpolateRed();
	void InterpolateGreen();
	void InterpolateBlue();
	const Image* originalImage;
	const int scaleFactor; 
	const ZoomMode zoomMode;
	const Image ROI;
};

#endif // !image
