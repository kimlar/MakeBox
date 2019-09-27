#include <iostream>
#include <string>
#include <vector>

#include "BoxEngine.h"

int main(int argc, char *argv[])
{	
	// Check for flags
	if (argc == 3)
	{
		// List files in the box container
		if (argv[1][0] == '-' && argv[1][1] == 'l')
		{
			BoxEngine* engine = new BoxEngine(argv[2]);
			engine->List();
			return 0;
		}

		// Unpack files in the box container
		if (argv[1][0] == '-' && argv[1][1] == 'u')
		{
			BoxEngine* engine = new BoxEngine(argv[2]);
			engine->Unpack();
			return 0;
		}
	}

	// Display help info?
	if (argc < 3)
	{
		std::cout << "Packing files:\tUsage: makebox.exe myfile.box file1.txt file2.txt ..." << std::endl;
		std::cout << "Listing files:\tUsage: makebox.exe -l myfile.box" << std::endl;
		std::cout << "Unpacking:\tUsage: makebox.exe -u myfile.box" << std::endl;
		return 1;
	}

	// Store parameters
	std::string boxFileName = argv[1];
	std::vector<std::string> vFiles;
	for (int i = 2; i < argc; i++)
		vFiles.push_back(argv[i]);

	// Run the engine
	BoxEngine* engine = new BoxEngine(boxFileName, vFiles);
	engine->Compile();
	
	return 0;
}
