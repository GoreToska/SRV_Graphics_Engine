#pragma once

#include<d3d11.h>
#include<wrl/client.h>
#include<vector>

#include"../Utils/Logger.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")


class AdapterData
{
public:
	AdapterData(IDXGIAdapter* pAdapter);
	IDXGIAdapter* pAdapter = nullptr;
	DXGI_ADAPTER_DESC desctiption;
};

class AdapterReader
{
public:
	static std::vector<AdapterData> GetAdapters();

private:
	static std::vector<AdapterData> adapters;
};

