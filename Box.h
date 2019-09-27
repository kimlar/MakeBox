#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

class Box
{
public:
	Box(std::string fileName);
	~Box();

	void Read();
	void Compile();

	std::string fileName;
	std::vector<char> boxBuffer; // temp
	std::vector<char> buffer;
	std::streamsize size;
};
