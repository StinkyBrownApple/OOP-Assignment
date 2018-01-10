#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "Image.h"

class IO {
public:
	static void println(std::string);
	static void print(std::string);
	static void ClearConsole();
	static int GetOption();
	static std::string GetString();
	static void Wait();
	static Image* readPPM(const char* file);
	static void writePPM(const Image &img);
	static void writeTXT(std::string, std::string);
};