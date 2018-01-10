#pragma once
#include "stdafx.h"
#include "IO.h"

void IO::println(std::string s)
{
	std::cout << s << std::endl;
}

void IO::print(std::string s)
{
	std::cout << s;
}

void IO::ClearConsole()
{
	system("cls");
	println("************************************");
	println("Image Stacker / Image Scaler");
	println("************************************");
	println("");
}

int IO::GetOption()
{
	int option;
	try 
	{
		std::string input;
		std::cin >> input;
		option = std::stoi(input);
	}
	catch (...)
	{
		option = 0;
	}
	return option;
}

std::string IO::GetString()
{
	std::string input;
	std::cin >> input;
	return input;
}

void IO::Wait()
{
	system("pause");
}

//Read ppm files into the code
//They need to be in 'binary' format (P6) with no comments in the header
//The first line is the 'P'number - P6 indicates it is a binary file, then the image dimensions and finally the colour range
//This header is then followed by the pixel colour data
//eg:	P6
//		3264 2448
//		255
//Open a .ppm file in notepad++ to see this header (caution: they are large files!)
Image* IO::readPPM(const char *filename)
{
	std::ifstream ifs;
	ifs.open(filename, std::ios::binary);
	Image *src;
	try {
		if (ifs.fail()) {
			throw("Can't open the input file - is it named correctly/is it in the right directory?");
		}
		std::string header;
		int w, h, b;
		ifs >> header;
		if (strcmp(header.c_str(), "P6") != 0) throw("Can't read the input file - is it in binary format (Has P6 in the header)?");
		ifs >> w >> h >> b;
		src = new Image(w, h, b, filename);
		ifs.ignore(256, '\n'); // skip empty lines if necessary until we get to the binary data 
		unsigned char pix[3]; // read each pixel one by one and convert bytes to floats 
		for (int i = 0; i < w * h; ++i) {
			ifs.read(reinterpret_cast<char *>(pix), 3);
			src->pixels[i].r = pix[0] / 255.f;
			src->pixels[i].g = pix[1] / 255.f;
			src->pixels[i].b = pix[2] / 255.f;
		}
		ifs.close();
	}
	catch (const char *err) {
		fprintf(stderr, "%s\n", err);
		ifs.close();
		src = new Image();
	}
	return src;
}

//Write data out to a ppm file
//Constructs the header as above
void IO::writePPM(const Image &img)
{
	if (img.Width() == 0 || img.Height() == 0) { fprintf(stderr, "Can't save an empty image\n"); return; }
	std::ofstream ofs;
	try {
		ofs.open(img.Path(), std::ios::binary); // need to spec. binary mode for Windows users
		if (ofs.fail()) throw("Can't open output file");
		ofs << "P6\n" << img.Width() << " " << img.Height() << "\n255\n";
		unsigned char r, g, b;
		// loop over each pixel in the image, clamp and convert to byte format
		for (unsigned int i = 0; i < img.Width() * img.Height(); ++i) {
			r = static_cast<unsigned char>(std::min(1.f, img.pixels[i].r) * 255);
			g = static_cast<unsigned char>(std::min(1.f, img.pixels[i].g) * 255);
			b = static_cast<unsigned char>(std::min(1.f, img.pixels[i].b) * 255);
			ofs << r << g << b;
		}
		ofs.close();
	}
	catch (const char *err) {
		fprintf(stderr, "%s\n", err);
		ofs.close();
	}
}

void IO::writeTXT(std::string filePath, std::string text)
{
	std::ofstream ofs;
	try 
	{
		ofs.open(filePath);
		if (ofs.fail()) throw("Cant open output file");
		ofs << text;
	}
	catch (const char *err)
	{
		fprintf(stderr, "%s\n", err);
		ofs.close();
	}
}
