#include "stdafx.h"
#include "Blending.h"
#include "Maths.h"
#include "IO.h"
#include <thread>

Image Blending::MeanBlend(std::vector<Image*> images, bool multiThread)
{
	if (!multiThread)
		return Blend(images, Mean);
	else
		return MultiThreadBlend(images, Mean);
}

Image Blending::MedianBlend(std::vector<Image*> images, bool multiThread)
{
	if (!multiThread)
		return Blend(images, Median);
	else
		return MultiThreadBlend(images, Median);
}

Image Blending::SigmaClippedMeanBlend(std::vector<Image*> images, bool multiThread)
{
	if (!multiThread)
		return Blend(images, SigmaClippedMean);
	else
		return MultiThreadBlend(images, SigmaClippedMean);
}

Image Blending::Blend(std::vector<Image*> images, BlendMode mode)
{
	int width = images[0]->w;
	int height = images[0]->h;

	IO::println(std::to_string(width));
	IO::println(std::to_string(height));
	IO::println(std::to_string(width*height));
	Image *blendedImage = new Image(width, height);
	std::vector<float> rs;
	std::vector<float> gs;
	std::vector<float> bs;

	for (int i = 0; i < width * height; i++)
	{
		for (unsigned int x = 0; x < images.size(); x++)
		{
			rs.push_back(images[x]->pixels[i].r);
			gs.push_back(images[x]->pixels[i].g);
			bs.push_back(images[x]->pixels[i].b);
		}
		if (mode == Mean)
			blendedImage->pixels[i] = Image::Rgb(Maths::Mean(rs), Maths::Mean(gs), Maths::Mean(bs));
		else if (mode == Median)
			blendedImage->pixels[i] = Image::Rgb(Maths::Median(rs), Maths::Median(gs), Maths::Median(bs));
		else
			blendedImage->pixels[i] = Image::Rgb(Maths::SigmaClippedMean(rs), Maths::SigmaClippedMean(gs), Maths::SigmaClippedMean(bs));
		//IO::println("Wrote pixel: " + std::to_string(blendedImage->pixels[i].r * 255) + ", " + std::to_string(blendedImage->pixels[i].g * 255) + ", " + std::to_string(blendedImage->pixels[i].b * 255) + "... " + std::to_string(i / width * height) + "% complete");
		//IO::println(std::to_string(i) + " / " + std::to_string(width * height));
		rs.clear();
		gs.clear();
		bs.clear();
	}
	return *blendedImage;
}

Image Blending::MultiThreadBlend(std::vector<Image*> images, BlendMode mode)
{
	Image *mergedImage = new Image(images[0]->w, images[0]->h);
	std::thread *redThread = new std::thread(BlendRed, images, mergedImage, mode);
	std::thread *greenThread = new std::thread(BlendGreen, images, mergedImage, mode);
	BlendBlue(images, mergedImage, mode);
	redThread->join();
	greenThread->join();

	return *mergedImage;
}

void Blending::BlendRed(std::vector<Image*> images, Image* outputImage, BlendMode mode)
{
	int width = images[0]->w;
	int height = images[0]->h;
	std::vector<float> rs;

	for (int i = 0; i < width * height; i++)
	{
		for (unsigned int x = 0; x < images.size(); x++)
		{
			rs.push_back(images[x]->pixels[i].r);
		}
		if (mode == Mean)
			outputImage->pixels[i] = Image::Rgb(Maths::Mean(rs), outputImage->pixels[i].g, outputImage->pixels[i].b);
		else if (mode == Median)
			outputImage->pixels[i] = Image::Rgb(Maths::Median(rs), 0, 0);
		else
			outputImage->pixels[i] = Image::Rgb(Maths::SigmaClippedMean(rs), 0, 0);
		rs.clear();
	}
}

void Blending::BlendGreen(std::vector<Image*> images, Image* outputImage, BlendMode mode)
{
	int width = images[0]->w;
	int height = images[0]->h;

	Image *blendedImage = new Image(width, height);
	std::vector<float> gs;

	for (int i = 0; i < width * height; i++)
	{
		for (unsigned int x = 0; x < images.size(); x++)
		{
			gs.push_back(images[x]->pixels[i].g);
		}
		if (mode == Mean)
			outputImage->pixels[i] = Image::Rgb(outputImage->pixels[i].r, Maths::Mean(gs), outputImage->pixels[i].b);
		else if (mode == Median)
			outputImage->pixels[i] = Image::Rgb(0, Maths::Median(gs), 0);
		else
			outputImage->pixels[i] = Image::Rgb(0, Maths::SigmaClippedMean(gs), 0);
		gs.clear();
	}
}

void Blending::BlendBlue(std::vector<Image*> images, Image* outputImage, BlendMode mode)
{
	int width = images[0]->w;
	int height = images[0]->h;

	Image *blendedImage = new Image(width, height);
	std::vector<float> bs;

	for (int i = 0; i < width * height; i++)
	{
		for (unsigned int x = 0; x < images.size(); x++)
		{
			bs.push_back(images[x]->pixels[i].b);
		}
		if (mode == Mean)
			outputImage->pixels[i] = Image::Rgb(outputImage->pixels[i].r, outputImage->pixels[i].g, Maths::Mean(bs));
		else if (mode == Median)
			outputImage->pixels[i] = Image::Rgb(0, 0, Maths::Median(bs));
		else
			outputImage->pixels[i] = Image::Rgb(0, 0, Maths::SigmaClippedMean(bs));
		bs.clear();
	}
}


