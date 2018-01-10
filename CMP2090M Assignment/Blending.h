#pragma once
#include <vector>
#include "Image.h"

class Blending
{
public:
	static Image MeanBlend(std::vector<Image*>, bool);
	static Image MedianBlend(std::vector<Image*>, bool);
	static Image SigmaClippedMeanBlend(std::vector<Image*>, bool);
private:
	enum BlendMode {
		Mean, Median, SigmaClippedMean
	};
	static Image Blend(std::vector<Image*>, BlendMode);
	static void BlendRed(std::vector<Image*>, Image*, BlendMode);
	static void BlendGreen(std::vector<Image*>, Image*, BlendMode);
	static void BlendBlue(std::vector<Image*>, Image*, BlendMode);
	static Image MultiThreadBlend(std::vector<Image*>, BlendMode);
};
