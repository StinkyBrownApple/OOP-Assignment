#include "stdafx.h"
#include "Maths.h"
#include <algorithm>

float Maths::Mean(std::vector<float> numbers)
{
	float total = 0.0;
	for each (float n in numbers)
	{
		total += n;
	}
	return total / numbers.size();
}

float Maths::Median(std::vector<float> numbers)
{
	std::sort(numbers.begin(), numbers.end());
	if ((numbers.size() - 1) % 2 == 0)
		return numbers[(numbers.size() - 1) / 2];
	else
		return numbers[(int)((numbers.size() - 1) / 2)] + numbers[(int)((numbers.size() - 1) / 2) + 1] / 2;
}

float Maths::StandardDeviation(std::vector<float> numbers)
{
	if (numbers.size() < 2) //Make sure we won't get /0 errors later
		return 0;
	float numerator = 0;
	float mean = Mean(numbers);
	for (int i = 0; i < numbers.size(); i++)
	{
		numerator += (numbers[i] - mean) * (numbers[i] - mean);
	}
	return sqrt(numerator / (numbers.size() - 1));
}

float Maths::SigmaClippedMean(std::vector<float> numbers, float sigma)
{
	float total = 0.0;
	float size = 0;
	float sd = StandardDeviation(numbers);
	float median = Median(numbers);
	for each (float n in numbers)
	{
		if (n < median + (sd * sigma) && n > median - (sd * sigma))
		{
			total += n;
			size++;
		}
		
	}
	return total / size;
}

int Maths::TwoDToOneD(int x, int y, int width)
{
	return (y*width) + x;
}

void Maths::OneDToTwoD(int i, int width, int* outX, int* outY)
{
	*outX = i % width;
	*outY = i / width;
}


