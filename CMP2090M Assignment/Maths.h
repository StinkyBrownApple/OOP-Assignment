#pragma once
#include <vector>
class Maths
{
public:
	static float Mean(std::vector<float>);
	static float Median(std::vector<float>);
	static float StandardDeviation(std::vector<float>);
	static float SigmaClippedMean(std::vector<float>, float sigma = 0.5f);
	static int TwoDToOneD(int, int, int);
	static void OneDToTwoD(int, int, int*, int*);
};
