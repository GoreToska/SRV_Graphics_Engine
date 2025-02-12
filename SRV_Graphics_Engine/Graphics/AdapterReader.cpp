#include "AdapterReader.h"

std::vector<AdapterData> AdapterReader::adapters;

AdapterData::AdapterData(IDXGIAdapter* pAdapter)
{
	this->pAdapter = pAdapter;
	HRESULT hr = pAdapter->GetDesc(&desctiption);

	if (FAILED(hr))
	{
		Logger::LogError(hr, "Failed to get decription for IDXGIAdapter.");
	}
}

std::vector<AdapterData> AdapterReader::GetAdapters()
{
	if (adapters.size() > 0)
		return adapters;

	Microsoft::WRL::ComPtr<IDXGIFactory> pFactory;
	HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::LogError(hr, "Failed to create IDXGIFactory for enumerating adapters.");
		exit(-1);
	}

	IDXGIAdapter* pAdapter;
	UINT index = 0;

	while (SUCCEEDED(pFactory->EnumAdapters(index, &pAdapter)))
	{
		adapters.push_back(AdapterData(pAdapter));
		index += 1;
	}

	return adapters;
}
