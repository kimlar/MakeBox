#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "Box.h"

class BoxEngine
{
public:
	BoxEngine(std::string boxFile, std::vector<std::string> vFiles);
	BoxEngine(std::string boxFile);

	void Compile();
	void List();
	void Unpack();

	std::string boxFile;
	std::vector<std::string> vFiles;
	std::vector<Box> vBoxes;
	std::vector<char> boxBuffer; // temp

private:
	void ReadBoxFile(); // read raw box file into memory
	void ReadBoxArchive(bool unpackFile); // translate from raw box format to get file names, file sizes and file data.
	
	std::streamsize size;
};
