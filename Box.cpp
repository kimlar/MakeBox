#include "Box.h"

Box::Box(std::string fileName)
{
	this->fileName = fileName;
}

Box::~Box()
{
}

void Box::Read()
{
	std::ifstream file(this->fileName, std::ios::binary | std::ios::ate);
	this->size = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<char> buffer(this->size);
	if (file.read(buffer.data(), this->size))
	{
		boxBuffer = buffer;
		std::cout << fileName.c_str() << std::endl;
	}
	else
	{
		std::cout << "ERROR: Box::Read(): " << fileName.c_str() << std::endl;
	}
}

void Box::Compile()
{
	std::string fileTag;
	fileTag = "\"" + this->fileName + "\"," + std::to_string(this->size) + "\n{\n";

	for(unsigned int i = 0; i < fileTag.size(); i++)
		buffer.push_back(fileTag[i]);
	
	for (unsigned int i = 0; i < this->size; i++)
		buffer.push_back(this->boxBuffer[i]);

	buffer.push_back('\n');
	buffer.push_back('}');
	buffer.push_back('\n');
}
