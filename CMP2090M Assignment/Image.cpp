#pragma once
#include "stdafx.h"
#include "Image.h"
#include "IO.h"
#include <thread>

///////////////////////////////								//////////////////////////
///////////////////////////////    Image class functions	//////////////////////////
//////////////////////////////								//////////////////////////

//Constructor
Image::Image(const unsigned int & _w, const unsigned int & _h, const unsigned int & _depth, const std::string & _path, const Rgb & c) :
	w(_w), h(_h), pixels(NULL), depth(_depth), path(_path)
{
	pixels = new Rgb[w * h];
	for (unsigned int i = 0; i < w * h; ++i)
		pixels[i] = c;
}
//Destructor
Image::~Image()
{
	if (pixels != NULL) delete[] pixels;
	//delete[];
}
//Output function
void Image::OutputDetails(std::string filePath)
{
	IO::writeTXT(filePath, "Regular PPM image\n" + 
		std::string("File path: ") + Path() + "\n" +
		"Image width: " + std::to_string(Width()) + "\n" +
		"Image height: " + std::to_string(Height()) + "\n" +
		"Image depth: " + std::to_string(Depth()) + "\n");
}
//Member variable getters
unsigned int Image::Width() const
{
	return w;
}
unsigned int Image::Height() const
{
	return h;
}
unsigned int Image::Depth() const
{
	return depth;
}
std::string Image::Path() const
{
	return path;
}
//Constant colours
const Image::Rgb Image::kBlack = Image::Rgb(0);
const Image::Rgb Image::kWhite = Image::Rgb(1);
const Image::Rgb Image::kRed = Image::Rgb(1, 0, 0);
const Image::Rgb Image::kGreen = Image::Rgb(0, 1, 0);
const Image::Rgb Image::kBlue = Image::Rgb(0, 0, 1);

///////////////////////////////									//////////////////////////
///////////////////////////////    BlendedImage class functions	//////////////////////////
//////////////////////////////									//////////////////////////

//Constructor
BlendedImage::BlendedImage(std::vector<Image*> images, BlendMode mode, bool useMultiThread, std::string path) : Image(images[0]->Width(), images[0]->Height(), images[0]->Depth(), path), blendMode(mode) {
	if (useMultiThread)
		MultiThreadBlend(images);
	else
		Blend(images);
}
//Single thread blend
void BlendedImage::Blend(std::vector<Image*> images)
{
	std::vector<float> rs;
	std::vector<float> gs;
	std::vector<float> bs;

	for (unsigned int i = 0; i < Width() * Height(); i++)
	{
		for (unsigned int x = 0; x < images.size(); x++)
		{
			rs.push_back(images[x]->pixels[i].r);
			gs.push_back(images[x]->pixels[i].g);
			bs.push_back(images[x]->pixels[i].b);
		}
		if (Mode() == Mean)
			pixels[i] = Image::Rgb(Maths::Mean(rs), Maths::Mean(gs), Maths::Mean(bs));
		else if (Mode() == Median)
			pixels[i] = Image::Rgb(Maths::Median(rs), Maths::Median(gs), Maths::Median(bs));
		else
			pixels[i] = Image::Rgb(Maths::SigmaClippedMean(rs), Maths::SigmaClippedMean(gs), Maths::SigmaClippedMean(bs));
		rs.clear();
		gs.clear();
		bs.clear();
	}
}
//Multithread blend
void BlendedImage::MultiThreadBlend(std::vector<Image*> images)
{
	std::thread *redThread = new std::thread(&BlendedImage::BlendRed, this, images);
	std::thread *greenThread = new std::thread(&BlendedImage::BlendGreen, this, images);
	BlendBlue(images);
	redThread->join();
	greenThread->join();
}
void BlendedImage::BlendRed(std::vector<Image*> images)
{
	std::vector<float> rs;

	for (unsigned int i = 0; i < Width() * Height(); i++)
	{
		for (unsigned int x = 0; x < images.size(); x++)
		{
			rs.push_back(images[x]->pixels[i].r);
		}
		if (Mode() == Mean)
			pixels[i] = Image::Rgb(Maths::Mean(rs), pixels[i].g, pixels[i].b);
		else if (Mode() == Median)
			pixels[i] = Image::Rgb(Maths::Median(rs), pixels[i].g, pixels[i].b);
		else
			pixels[i] = Image::Rgb(Maths::SigmaClippedMean(rs), pixels[i].g, pixels[i].b);
		rs.clear();
	}
}
void BlendedImage::BlendGreen(std::vector<Image*> images)
{
	std::vector<float> gs;

	for (unsigned int i = 0; i < Width() * Height(); i++)
	{
		for (unsigned int x = 0; x < images.size(); x++)
		{
			gs.push_back(images[x]->pixels[i].g);
		}
		if (Mode() == Mean)
			pixels[i] = Image::Rgb(pixels[i].r, Maths::Mean(gs), pixels[i].b);
		else if (Mode() == Median)
			pixels[i] = Image::Rgb(pixels[i].r, Maths::Median(gs), pixels[i].b);
		else
			pixels[i] = Image::Rgb(pixels[i].r, Maths::SigmaClippedMean(gs), pixels[i].b);
		gs.clear();
	}
}
void BlendedImage::BlendBlue(std::vector<Image*> images)
{
	std::vector<float> bs;

	for (unsigned int i = 0; i < Width() * Height(); i++)
	{
		for (unsigned int x = 0; x < images.size(); x++)
		{
			bs.push_back(images[x]->pixels[i].b);
		}
		if (Mode() == Mean)
			pixels[i] = Image::Rgb(pixels[i].r, pixels[i].g, Maths::Mean(bs));
		else if (Mode() == Median)
			pixels[i] = Image::Rgb(pixels[i].r, pixels[i].g, Maths::Median(bs));
		else
			pixels[i] = Image::Rgb(pixels[i].r, pixels[i].g, Maths::SigmaClippedMean(bs));
		bs.clear();
	}
}
//Output function
void BlendedImage::OutputDetails(std::string filePath)
{
	std::string method = (Mode() == Mean) ? "Mean" : (Mode() == Median) ? "Median" : "Sigma Clipped Mean";
	IO::writeTXT(filePath, "Blended PPM image\n" +
		std::string("File path: ") + Path() + "\n" +
		"Image width: " + std::to_string(Width()) + "\n" +
		"Image height: " + std::to_string(Height()) + "\n" +
		"Image depth: " + std::to_string(Depth()) + "\n" +
		"Blend method: " + method + "\n");
}
//Blend mode getter
BlendedImage::BlendMode BlendedImage::Mode() const
{
	return blendMode;
}

///////////////////////////////									//////////////////////////
///////////////////////////////    ZoomedImage class functions	//////////////////////////
//////////////////////////////									//////////////////////////

//Constructors
ZoomedImage::ZoomedImage(Image * imageToZoom, int scale, ZoomMode mode, std::string path) : Image(imageToZoom->Width() * scale, imageToZoom->Height() * scale, imageToZoom->Depth(), path), originalImage(imageToZoom), scaleFactor(scale), zoomMode(mode)
{
	if (zoomMode == NearestNeighbour)
		NearestNeighbourZoom();
	else
		BilinearZoom();
}
ZoomedImage::ZoomedImage(Image * imageToZoom, int scale, ZoomMode mode, int x1, int y1, int x2, int y2, std::string path) : Image((x2 - x1) * scale, (y2 - y1) * scale, imageToZoom->Depth(), path), ROI(Image(x2 - x1, y2 - y1, imageToZoom->Depth(), path)), originalImage(&ROI), scaleFactor(scale), zoomMode(mode)
{
	if (x2 < x1)
	{
		int temp = x1;
		x1 = x2;
		x2 = temp;
	}
	if (y2 < y1)
	{
		int temp = y1;
		y1 = y2;
		y2 = temp;
	}

	for (int x = 0; x < ROI.Width(); x++)
	{
		for (int y = 0; y < ROI.Height(); y++)
		{
			ROI.pixels[Maths::TwoDToOneD(x, y, ROI.Width())] = imageToZoom->pixels[Maths::TwoDToOneD(x1 + x, y1 + y, imageToZoom->Width())];
		}
	}

	if (zoomMode == NearestNeighbour)
		NearestNeighbourZoom();
	else
		BilinearZoom();
}
//Nearest neighbour zoom algorithm
void ZoomedImage::NearestNeighbourZoom()
{
	for (int i = 0; i < originalImage->Width() * originalImage->Height(); i++)
	{
		int originalX;
		int originalY;
		Maths::OneDToTwoD(i, originalImage->Width(), &originalX, &originalY);
		for (int y = 0; y < scaleFactor; y++)
		{
			for (int x = 0; x < scaleFactor; x++)
			{
				int newX = (originalX * scaleFactor) + x;
				int newY = (originalY * scaleFactor) + y;
				int oneDCoord = Maths::TwoDToOneD(newX, newY, this->Width());
				pixels[oneDCoord] = originalImage->pixels[i];
			}
		}
	}
}
//Bilinear Zoom Algorithm
void ZoomedImage::BilinearZoom()
{
	
	int originalX, originalY;
	for (int i = 0; i < originalImage->Width() * originalImage->Height(); i++)
	{
		Maths::OneDToTwoD(i, originalImage->Width(), &originalX, &originalY);
		pixels[Maths::TwoDToOneD(originalX * scaleFactor, originalY * scaleFactor, Width())] = originalImage->pixels[i];
	}

	InterpolateRed();
	InterpolateGreen();
	InterpolateBlue();
}
void ZoomedImage::InterpolateRed()
{
	int x, y;
	for (int i = 0; i < Width() * Height(); i++)
	{
		Maths::OneDToTwoD(i, Width(), &x, &y);
		if (x % scaleFactor == 0 && y % scaleFactor == 0)
			continue;

		int x1 = x - (x % scaleFactor);
		int x2 = x + (scaleFactor - (x % scaleFactor));
		int y2 = y - (y % scaleFactor);
		int y1 = y + (scaleFactor - (y % scaleFactor));

		bool rightEdge = (x2 >= Width()) ? true : false;
		bool bottomEdge = (y1 >= Height()) ? true : false;

		float Q12, Q11, Q21, Q22;

		Q12 = originalImage->pixels[Maths::TwoDToOneD(x1 / scaleFactor, y2 / scaleFactor, originalImage->Width())].r;

		if (!rightEdge)
		{
			Q22 = originalImage->pixels[Maths::TwoDToOneD(x2 / scaleFactor, y2 / scaleFactor, originalImage->Width())].r;
			if (bottomEdge)
			{
				Q21 = Q22;
				Q11 = Q12;
			}
			else
			{
				Q21 = originalImage->pixels[Maths::TwoDToOneD(x2 / scaleFactor, y1 / scaleFactor, originalImage->Width())].r;
				Q11 = originalImage->pixels[Maths::TwoDToOneD(x1 / scaleFactor, y1 / scaleFactor, originalImage->Width())].r;
			}
		}
		else
		{
			Q22 = Q12;
			if (bottomEdge)
			{
				Q21 = Q12;
				Q11 = Q12;
			}
			else
			{
				Q11 = originalImage->pixels[Maths::TwoDToOneD(x1 / scaleFactor, y1 / scaleFactor, originalImage->Width())].r;;
				Q21 = Q11;
			}
		}		

		float R1 = ((x2 - x) / (float)(x2 - x1))*Q11 + ((x - x1) / (float)(x2 - x1))*Q21;
		float R2 = ((x2 - x) / (float)(x2 - x1))*Q12 + ((x - x1) / (float)(x2 - x1))*Q22;

		pixels[i].r = ((y2 - y) / (float)(y2 - y1))*R1 + ((y - y1) / (float)(y2 - y1))*R2;
	}
}
void ZoomedImage::InterpolateGreen()
{
	int x, y;
	for (int i = 0; i < Width() * Height(); i++)
	{
		Maths::OneDToTwoD(i, Width(), &x, &y);
		if (x % scaleFactor == 0 && y % scaleFactor == 0)
			continue;

		int x1 = x - (x % scaleFactor);
		int x2 = x + (scaleFactor - (x % scaleFactor));
		int y2 = y - (y % scaleFactor);
		int y1 = y + (scaleFactor - (y % scaleFactor));

		bool rightEdge = (x2 >= Width()) ? true : false;
		bool bottomEdge = (y1 >= Height()) ? true : false;

		float Q12, Q11, Q21, Q22;

		Q12 = originalImage->pixels[Maths::TwoDToOneD(x1 / scaleFactor, y2 / scaleFactor, originalImage->Width())].g;

		if (!rightEdge)
		{
			Q22 = originalImage->pixels[Maths::TwoDToOneD(x2 / scaleFactor, y2 / scaleFactor, originalImage->Width())].g;
			if (bottomEdge)
			{
				Q21 = Q22;
				Q11 = Q12;
			}
			else
			{
				Q21 = originalImage->pixels[Maths::TwoDToOneD(x2 / scaleFactor, y1 / scaleFactor, originalImage->Width())].g;
				Q11 = originalImage->pixels[Maths::TwoDToOneD(x1 / scaleFactor, y1 / scaleFactor, originalImage->Width())].g;
			}
		}
		else
		{
			Q22 = Q12;
			if (bottomEdge)
			{
				Q21 = Q12;
				Q11 = Q12;
			}
			else
			{
				Q11 = originalImage->pixels[Maths::TwoDToOneD(x1 / scaleFactor, y1 / scaleFactor, originalImage->Width())].g;
				Q21 = Q11;
			}
		}

		float R1 = ((x2 - x) / (float)(x2 - x1))*Q11 + ((x - x1) / (float)(x2 - x1))*Q21;
		float R2 = ((x2 - x) / (float)(x2 - x1))*Q12 + ((x - x1) / (float)(x2 - x1))*Q22;

		pixels[i].g = ((y2 - y) / (float)(y2 - y1))*R1 + ((y - y1) / (float)(y2 - y1))*R2;
	}
}
void ZoomedImage::InterpolateBlue()
{
	int x, y;
	for (int i = 0; i < Width() * Height(); i++)
	{
		Maths::OneDToTwoD(i, Width(), &x, &y);
		if (x % scaleFactor == 0 && y % scaleFactor == 0)
			continue;

		int x1 = x - (x % scaleFactor);
		int x2 = x + (scaleFactor - (x % scaleFactor));
		int y2 = y - (y % scaleFactor);
		int y1 = y + (scaleFactor - (y % scaleFactor));

		bool rightEdge = (x2 >= Width()) ? true : false;
		bool bottomEdge = (y1 >= Height()) ? true : false;

		float Q12, Q11, Q21, Q22;

		Q12 = originalImage->pixels[Maths::TwoDToOneD(x1 / scaleFactor, y2 / scaleFactor, originalImage->Width())].b;

		if (!rightEdge)
		{
			Q22 = originalImage->pixels[Maths::TwoDToOneD(x2 / scaleFactor, y2 / scaleFactor, originalImage->Width())].b;
			if (bottomEdge)
			{
				Q21 = Q22;
				Q11 = Q12;
			}
			else
			{
				Q21 = originalImage->pixels[Maths::TwoDToOneD(x2 / scaleFactor, y1 / scaleFactor, originalImage->Width())].b;
				Q11 = originalImage->pixels[Maths::TwoDToOneD(x1 / scaleFactor, y1 / scaleFactor, originalImage->Width())].b;
			}
		}
		else
		{
			Q22 = Q12;
			if (bottomEdge)
			{
				Q21 = Q12;
				Q11 = Q12;
			}
			else
			{
				Q11 = originalImage->pixels[Maths::TwoDToOneD(x1 / scaleFactor, y1 / scaleFactor, originalImage->Width())].b;
				Q21 = Q11;
			}
		}

		float R1 = ((x2 - x) / (float)(x2 - x1))*Q11 + ((x - x1) / (float)(x2 - x1))*Q21;
		float R2 = ((x2 - x) / (float)(x2 - x1))*Q12 + ((x - x1) / (float)(x2 - x1))*Q22;

		pixels[i].b = ((y2 - y) / (float)(y2 - y1))*R1 + ((y - y1) / (float)(y2 - y1))*R2;
	}
}
//Ouput function
void ZoomedImage::OutputDetails(std::string filePath)
{
	std::string method = (Mode() == NearestNeighbour) ? "Nearest Neighobour" : "Bilinear";

	IO::writeTXT(filePath, "Zoomed PPM image\n" +
		std::string("File path: ") + Path() + "\n" +
		"Image width: " + std::to_string(Width()) + "\n" +
		"Image height: " + std::to_string(Height()) + "\n" +
		"Image depth: " + std::to_string(Depth()) + "\n" +
		"Zoom method: " + method + "\n");
}
//Zoom mode getter
ZoomedImage::ZoomMode ZoomedImage::Mode() const
{
	return zoomMode;
}
