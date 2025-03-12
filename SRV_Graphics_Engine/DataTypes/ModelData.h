#pragma once
#include <string>

struct ModelData
{
public:
	ModelData(std::string modelPath, std::wstring texturePath)
		:modelPath(modelPath), texturePath(texturePath) {}

	std::string modelPath;
	std::wstring texturePath;
};