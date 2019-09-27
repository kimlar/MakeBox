#include "BoxEngine.h"

BoxEngine::BoxEngine(std::string boxFile, std::vector<std::string> vFiles)
{
	this->boxFile = boxFile;
	this->vFiles = vFiles;
}

BoxEngine::BoxEngine(std::string boxFile)
{
	this->boxFile = boxFile;
}

void BoxEngine::Compile()
{
	std::streamsize totalSize = 0;
	std::vector<char> boxFileData;

	for each (std::string item in vFiles)
		vBoxes.push_back(Box(item));

	for each (Box item in vBoxes)
	{
		item.Read();

		std::vector<char> temp;
		temp.clear();
		item.Compile();
		for (unsigned int i = 0; i < item.buffer.size(); i++)
			temp.push_back(item.buffer[i]);

		for (unsigned int i = 0; i < temp.size(); i++)
			boxFileData.push_back(temp[i]);

		totalSize += temp.size();
	}

	std::ofstream outfile(this->boxFile, std::ofstream::binary);

	// allocate memory for file content
	char* buffer = new char[totalSize];

	for (unsigned int i = 0; i < totalSize; i++)
		buffer[i] = boxFileData[i];

	// write to outfile
	outfile.write(buffer, totalSize);

	// release dynamically-allocated memory
	delete[] buffer;

	outfile.close();

	std::cout << std::endl << "Compiled box file: " << this->boxFile << std::endl;
}

void BoxEngine::List()
{
	//std::cout << "Listing!" << std::endl;

	this->ReadBoxFile();

	this->ReadBoxArchive(false);
}

void BoxEngine::Unpack()
{
	//std::cout << "Unpacking!" << std::endl;

	this->ReadBoxFile();

	this->ReadBoxArchive(true);
}

void BoxEngine::ReadBoxFile()
{
	std::ifstream file(this->boxFile, std::ios::binary | std::ios::ate);
	this->size = file.tellg();
	file.seekg(0, std::ios::beg);

	boxBuffer.clear();

	std::vector<char> buffer(this->size);
	if (file.read(buffer.data(), this->size))
	{
		boxBuffer = buffer;
		//std::cout << boxFile.c_str() << std::endl;
	}
	else
	{
		std::cout << "ERROR: Box::Read(): " << boxFile.c_str() << std::endl;
	}
}

void BoxEngine::ReadBoxArchive(bool unpackFile)
{
	std::vector<std::string> fileNames;
	std::vector<unsigned int> fileSizes;
	std::vector<char> tempHeader;

	enum Mode { FileHeader = 0, FileBody, FileEnd };
	int mode = Mode::FileHeader;

	for (unsigned int i = 0; i < boxBuffer.size(); i++)
	{
		if (mode == Mode::FileHeader)
		{
			if (boxBuffer[i] == '{')
			{
				mode = Mode::FileBody;

				std::string header(tempHeader.begin(), tempHeader.end() - 1); // -1 to skip \n character
				tempHeader.clear();

				//std::cout << "#" << header << "#" << std::endl;

				if(header[0] == '\"')
					header = header.substr(1);

				std::string tempFileName;
				std::string tempFileSize;
				unsigned int j;
				// get file name
				for (j = 0; j < header.size(); j++)
				{
					if (header[j] == '\"')
						break;
										
					tempFileName += header[j];
					//std::cout << "#" << header[j] << "#";
				}
				j++; //skip '\"' character
				j++; //skip ',' character
					 // get file size
				for (j; j < header.size(); j++)
				{
					if (header[j] == '\n')
						break;

					tempFileSize += header[j];
				}
				// save it
				fileNames.push_back(tempFileName);
				fileSizes.push_back(atoi(tempFileSize.c_str()));

				//std::cout << "#" << fileNames.back() << "#" << std::endl;
				//std::cout << "#" << fileSizes.back() << "#" << std::endl;

				i++; // skip '\n' character

					 // display the file name
				std::cout << fileNames.back() << std::endl;
			}
			else
			{
				tempHeader.push_back(boxBuffer[i]);
			}
		}
		else if (mode == Mode::FileBody)
		{
			// unpack the file?
			if (unpackFile)
			{
				std::ofstream outfile(fileNames.back(), std::ofstream::binary);

				// allocate memory for file content
				char* buffer = new char[fileSizes.back()];

				for (unsigned int k = 0; k < fileSizes.back(); k++)
					buffer[k] = boxBuffer[i+k];

				// write to outfile
				outfile.write(buffer, fileSizes.back());

				// release dynamically-allocated memory
				delete[] buffer;

				outfile.close();
			}

			// skip file data
			i += fileSizes.back();

			// skip '\n'
			i++;

			// skip '}'
			i++;

			// skip '\n'
			i++;

			// set mode
			mode = Mode::FileHeader;
		}
	}
}
