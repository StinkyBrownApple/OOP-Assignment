//main.cpp

#include "stdafx.h"
#include "IO.h"
#include <vector>
#include <ctime>

void Blend()
{
	while (true)
	{
		IO::ClearConsole();
		IO::println("Select a blending mode:");
		IO::println("1: Mean");
		IO::println("2: Median");
		IO::println("3: Sigma Clipped Mean");
		IO::println("4: Return to main menu\n");
		int option = IO::GetOption();
		while (option != 1 && option != 2 && option != 3 && option != 4)
		{
			IO::println("\nInvalid input, please try again\n");
			option = IO::GetOption();
		}

		bool multithread;
		BlendedImage::BlendMode mode;

		if (option == 1)
		{
			mode = BlendedImage::Mean;
		}

		if (option == 2)
		{
			mode = BlendedImage::Median;
		}

		if (option == 3)
		{
			mode = BlendedImage::SigmaClippedMean;
		}
		if (option == 4)
		{
			return;
		}

		IO::ClearConsole();
		IO::println("Would you like to use muli-threading? (Can significantly speed up the operation but will use increased CPU resources)");
		IO::println("Enter 1 for yes, 2 for no or 3 to return");
		option = IO::GetOption();
		while (option != 1 && option != 2 && option != 3)
		{
			IO::println("\nInvalid input, please try again\n");
			option = IO::GetOption();
		}
		if (option == 1)
		{
			multithread = true;
		}
		else if (option == 2)
		{
			multithread = false;
		}
		else
		{
			continue;
		}

		IO::ClearConsole();
		IO::println("Enter a name for the output image:");
		std::string outputName = IO::GetString();

		std::vector<Image*> images;

		IO::ClearConsole();
		IO::println("Loading images. This may take a while... (0/13)");
		Image *img0 = IO::readPPM("Images/BlendSet1/IMG_1.ppm");
		images.push_back(img0);

		IO::ClearConsole();
		IO::println("Loading images. This may take a while... (1/13)");
		Image *img1 = IO::readPPM("Images/BlendSet1/IMG_2.ppm");
		images.push_back(img1);

		IO::ClearConsole();
		IO::println("Loading images. This may take a while... (2/13)");
		Image *img2 = IO::readPPM("Images/BlendSet1/IMG_3.ppm");
		images.push_back(img2);

		IO::ClearConsole();
		IO::println("Loading images. This may take a while... (3/13)");
		Image *img3 = IO::readPPM("Images/BlendSet1/IMG_4.ppm");
		images.push_back(img3);

		IO::ClearConsole();
		IO::println("Loading images. This may take a while... (4/13)");
		Image *img4 = IO::readPPM("Images/BlendSet1/IMG_5.ppm");
		images.push_back(img4);

		IO::ClearConsole();
		IO::println("Loading images. This may take a while... (5/13)");
		Image *img5 = IO::readPPM("Images/BlendSet1/IMG_6.ppm");
		images.push_back(img5);

		IO::ClearConsole();
		IO::println("Loading images. This may take a while... (6/13)");
		Image *img6 = IO::readPPM("Images/BlendSet1/IMG_7.ppm");
		images.push_back(img6);

		IO::ClearConsole();
		IO::println("Loading images. This may take a while... (7/13)");
		Image *img7 = IO::readPPM("Images/BlendSet1/IMG_8.ppm");
		images.push_back(img7);

		IO::ClearConsole();
		IO::println("Loading images. This may take a while... (8/13)");
		Image *img8 = IO::readPPM("Images/BlendSet1/IMG_9.ppm");
		images.push_back(img8);

		IO::ClearConsole();
		IO::println("Loading images. This may take a while... (9/13)");
		Image *img9 = IO::readPPM("Images/BlendSet1/IMG_10.ppm");
		images.push_back(img9);

		IO::ClearConsole();
		IO::println("Loading images. This may take a while... (10/13)");
		Image *img10 = IO::readPPM("Images/BlendSet1/IMG_11.ppm");
		images.push_back(img10);

		IO::ClearConsole();
		IO::println("Loading images. This may take a while... (11/13)");
		Image *img11 = IO::readPPM("Images/BlendSet1/IMG_12.ppm");
		images.push_back(img11);

		IO::ClearConsole();
		IO::println("Loading images. This may take a while... (12/13)");
		Image *img12 = IO::readPPM("Images/BlendSet1/IMG_13.ppm");
		images.push_back(img12);

		IO::ClearConsole();
		IO::println("Images loaded. Blending images...");
		clock_t startClock = clock();
		BlendedImage *blend = new BlendedImage(images, mode, multithread, "OutputImages/" + outputName + ".ppm");
		clock_t stopClock = clock();
		clock_t tickTime = stopClock - startClock;
		IO::println("Blending took " + std::to_string((tickTime/(double)CLOCKS_PER_SEC)/60.0) + " minutes");
		IO::Wait();
		IO::ClearConsole();
		IO::println("Blend complete. Writing image...");
		IO::writePPM(*blend);
		images.push_back(blend);
		IO::println("Write complete.");
		IO::println("Writing info files...");
		for (int i = 0; i < images.size(); i++)
		{
			images[i]->OutputDetails("InfoOutput/image " + std::to_string(i) + std::string(".txt"));
			delete images[i];
		}
		IO::println("Write complete");
		IO::Wait();
		return;
	}
}

void Zoom()
{
	ZoomedImage *zoom = NULL;
	Image *original = NULL;
	while (true)
	{
		IO::ClearConsole();
		IO::println("Select a zoom mode:");
		IO::println("1: Nearest Neighbour");
		IO::println("2: Bilinear");
		IO::println("3: Return to main menu\n");
		int option = IO::GetOption();
		while (option != 1 && option != 2 && option != 3)
		{
			IO::println("\nInvalid input, please try again\n");
			option = IO::GetOption();
		}

		ZoomedImage::ZoomMode mode;

		if (option == 1)
		{
			mode = ZoomedImage::NearestNeighbour;
		}

		else if (option == 2)
		{
			mode = ZoomedImage::Bilinear;
		}

		else if (option == 3)
		{
			delete original;
			delete zoom;
			return;
		}

		IO::ClearConsole();
		IO::println("Enter a scale factor (whole number between 1 and 8, or 0 to return): ");
		option = IO::GetOption();
		while (option < 0 && option > 8)
		{
			IO::println("\nInvalid input, please try again\n");
			option = IO::GetOption();
		}

		if (option == 0)
		{
			continue;
		}

		int scale = option;

		IO::ClearConsole();
		IO::println("Loading image...");
		original = IO::readPPM("Images/Zoom/zIMG_1.ppm");
		
		IO::ClearConsole();
		IO::println("Select an option:");
		IO::println("1: Zoom whole image");
		IO::println("2: Zoom a region of interest");
		IO::println("3: Return\n");
		option = IO::GetOption();
		while (option != 1 && option != 2 && option != 3)
		{
			IO::println("\nInvalid input, please try again\n");
			option = IO::GetOption();
		}

		if (option == 1)
		{
			IO::ClearConsole();
			IO::println("Enter a name for the output image:");
			std::string outputName = IO::GetString();
			IO::ClearConsole();
			IO::println("Applying zoom...");
			zoom = new ZoomedImage(original, scale, mode, "OutputImages/" + outputName + ".ppm");
		}

		else if (option == 2)
		{
			IO::ClearConsole();
			IO::println("Enter the starting x coordinate (between 0 and " + std::to_string(original->Width() - 1) + ")");
			int x1 = IO::GetOption();
			while (x1 < 0 && x1 > original->Width() - 1)
			{
				IO::println("\nInvalid input, please try again\n");
				x1 = IO::GetOption();
			}

			IO::ClearConsole();
			IO::println("Enter the starting y coordinate (between 0 and " + std::to_string(original->Height() - 1) + ")");
			int y1 = IO::GetOption();
			while (y1 < 0 && y1 > original->Height() - 1)
			{
				IO::println("\nInvalid input, please try again\n");
				y1 = IO::GetOption();
			}

			IO::ClearConsole();
			IO::println("Enter the ending x coordinate (between 0 and " + std::to_string(original->Width() - 1) + ")");
			int x2 = IO::GetOption();
			while (x2 < 0 && x2 > original->Width() - 1)
			{
				IO::println("\nInvalid input, please try again\n");
				x2 = IO::GetOption();
			}

			IO::ClearConsole();
			IO::println("Enter the ending y coordinate (between 0 and " + std::to_string(original->Height() - 1) + ")");
			int y2 = IO::GetOption();
			while (y2 < 0 && y2 > original->Height() - 1)
			{
				IO::println("\nInvalid input, please try again\n");
				y2 = IO::GetOption();
			}

			IO::ClearConsole();
			IO::println("Enter a name for the output image:");
			std::string outputName = IO::GetString();

			zoom = new ZoomedImage(original, scale, mode, x1, y1, x2, y2, "OutputImages/" + outputName + ".ppm");
		}

		else if (option == 3)
		{
			continue;
		}

		IO::ClearConsole();
		IO::println("Zoom complete. Writing image...");
		IO::writePPM(*zoom);
		IO::ClearConsole();
		IO::println("Write complete. Writing info files...");
		original->OutputDetails("InfoOutput/original.txt");
		zoom->OutputDetails("InfoOutput/zoom.txt");
		delete original;
		delete zoom;
		IO::println("Write complete");
		IO::Wait();


		return;
	}
}

int main()
{
	while (true)
	{
		IO::ClearConsole();
		IO::println("Enter a number to select an option:");
		IO::println("1: Blend");
		IO::println("2: Zoom");
		IO::println("3: Exit\n");
		int option = IO::GetOption();
		while (option != 1 && option != 2 && option != 3)
		{
			IO::println("\nInvalid input, please try again\n");
			option = IO::GetOption();
		}

		if (option == 1)
		{
			Blend();
		}
		if (option == 2)
		{
			Zoom();
		}
		if (option == 3)
		{
			break;
		}
	}

	return 0;
}