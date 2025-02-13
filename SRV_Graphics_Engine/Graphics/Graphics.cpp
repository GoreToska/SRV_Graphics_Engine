#include "Graphics.h"
#include "RenderObjects/RenderComponent.h"
#include "./Device/GraphicsDevice.h"




bool Graphics::Initialize(HWND hwnd, int width, int height)
{
	if (!InitializeDirectX(hwnd, width, height))
		return false;

	if (!InitializeShaders())
		return false;

	if (!InitializeScene())
		return false;

	return true;
}

// ALL DRAWING IS HERE BETWEEN ClearRenderTargetView AND Present
void Graphics::RenderFrame()
{
	DeviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());

	float bgcolor[] = { 0.0f, 0.0, 0.0f, 1.0f }; // background color
	DeviceContext->ClearRenderTargetView(renderTargetView.Get(), bgcolor);
	DeviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// set input layout, topology, rasterizer state
	DeviceContext->IASetInputLayout(vertexShader.GetInputLayout());
	DeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DeviceContext->RSSetState(rasterizerState.Get());
	DeviceContext->OMSetDepthStencilState(depthStencilState.Get(), 0);

	// set shaders
	DeviceContext->VSSetShader(vertexShader.GetShader(), NULL, 0);
	DeviceContext->PSSetShader(pixelShader.GetShader(), NULL, 0);

	// TODO: where should i define this? 
	// inside RenderObject or here and pass it inside RenderObject?
	UINT stride = sizeof(Vertex3D);
	UINT offset = 0;

	for (RenderComponent* item : renderComponents)
	{
		item->Render();
	}

	swapchain->Present(1, NULL);
}

bool Graphics::InitializeDirectX(HWND hwnd, int width, int height)
{
	if (!CreateDeviceAndSwapChain(hwnd, width, height))
		return false;

	if (!CreateRenderTargetView())
		return false;

	if (!CreateDepthStencilBuffer(width, height))
		return false;

	GraphicsDevice::GetInstance().GetContext()->
		OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());

	if (!CreateDepthStencilState())
		return false;

	CreateViewport(width, height);

	if (!CreateRasterizerState())
		return false;

	if (!CreateSamplerState())
		return false;

	return true;
}

bool Graphics::CreateDeviceAndSwapChain(HWND hwnd, int width, int height)
{
	std::vector<AdapterData> adapters = AdapterReader::GetAdapters();

	if (adapters.size() < 1)
	{
		Logger::LogError("No DXGI Adapters found.");
		return false;
	}

	// TODO: decide wich device is better to use
	// just using first for now

	DXGI_SWAP_CHAIN_DESC swapchain_desc{};
	swapchain_desc.BufferDesc.Width = width;
	swapchain_desc.BufferDesc.Height = height;
	swapchain_desc.BufferDesc.RefreshRate.Numerator = 60;
	swapchain_desc.BufferDesc.RefreshRate.Denominator = 1;
	swapchain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapchain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapchain_desc.SampleDesc.Count = 1;
	swapchain_desc.SampleDesc.Quality = 0;
	swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchain_desc.BufferCount = 2;
	swapchain_desc.OutputWindow = hwnd;
	swapchain_desc.Windowed = TRUE;
	swapchain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapchain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_1 };

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		adapters[0].pAdapter,
		D3D_DRIVER_TYPE_UNKNOWN, // if we do not specify adapter (1st param) we can use D3D_DRIVER_TYPE_HARDWARE
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		featureLevels, // feature array
		ARRAYSIZE(featureLevels), // features size
		D3D11_SDK_VERSION,
		&swapchain_desc,
		swapchain.GetAddressOf(),
		DeviceAddress,
		NULL, // supported feature level
		DeviceContextAddress);

	if (FAILED(hr))
	{
		Logger::LogError(hr, "Failed to create device and swapchain.");
		return false;
	}

	return true;
}

bool Graphics::CreateRenderTargetView()
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	HRESULT hr = swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));

	if (FAILED(hr))
	{
		Logger::LogError(hr, "GetBuffer failed.");
		return false;
	}

	hr = Device->CreateRenderTargetView(backBuffer.Get(), NULL, renderTargetView.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::LogError(hr, "Failed to create render target view.");
		return false;
	}

	return true;
}

bool Graphics::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC rasterizerDesc{};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;

	// This can help if you draw objects counter clockwise (if you can't see them)
	// rasterizerDesc.FrontCounterClockwise = FALSE;

	HRESULT hr = Device->CreateRasterizerState(&rasterizerDesc, rasterizerState.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::LogError(hr, "Failed to create rasterizer state.");
		return false;
	}

	return true;
}

bool Graphics::CreateSamplerState()
{
	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT hr = Device->CreateSamplerState(&samplerDesc, samplerState.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::LogError(hr, "Failed to create sampler state.");
		return false;
	}

	return true;
}

bool Graphics::CreateDepthStencilState()
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	HRESULT hr = Device->CreateDepthStencilState(&depthStencilDesc, depthStencilState.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::LogError(hr, "Failed to create depth stencil state.");
		return false;
	}

	return true;
}

bool Graphics::CreateDepthStencilBuffer(int width, int height)
{
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	HRESULT hr = Device->CreateTexture2D(&depthStencilDesc, NULL, depthStencilBuffer.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::LogError(hr, "Failed to create depth stencil buffer.");
		return false;
	}

	hr = Device->CreateDepthStencilView(depthStencilBuffer.Get(), NULL, depthStencilView.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::LogError(hr, "Failed to create depth stencil view.");
		return false;
	}

	return true;
}

void Graphics::CreateViewport(int width, int height)
{
	D3D11_VIEWPORT viewport{};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinDepth = 0.0;
	viewport.MaxDepth = 1.0;

	DeviceContext->RSSetViewports(1, &viewport);
}

bool Graphics::InitializeShaders()
{
	std::wstring shaderFolder = L"";

#pragma region ShaderPathMacro
	if (IsDebuggerPresent() == TRUE)
	{
#ifdef _DEBUG
#ifdef _WIN64
		shaderFolder = L"../x64/Debug/";
#else
		shaderFolder = L"../Debug/";
#endif // _WIN64 or _WIN32

#else // RELEASE
#ifdef _WIN64
		shaderFolder = L"../x64/Release/";
#else
		shaderFolder = L"../Release/";
#endif
#endif // _DEBUG
	}
#pragma endregion

	D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	UINT numElements = ARRAYSIZE(layoutDesc);

	if (!vertexShader.Initialize(shaderFolder + L"VertexShader.cso", layoutDesc, numElements))
		return false;

	if (!pixelShader.Initialize(shaderFolder + L"PixelShader.cso"))
		return false;

	return true;
}

bool Graphics::InitializeScene()
{
	RenderComponent* triangle = new RenderComponent
	{
		{
		Vertex3D({-0.5f, -0.5f, 1.0f}, {1.0f, 0.0f, 0.0f}),
		Vertex3D({-0.5f, 0.5f, 1.0f}, {1.0f, 0.0f, 0.0f}),
		Vertex3D({0.5f, 0.5f, 1.0f}, {1.0f, 0.0f, 0.0f}),
		}
	};

	RenderComponent* triangle2 = new RenderComponent
	{
		{
		Vertex3D({0.5f, 0.5f, 1.0f}, {0.0f, 1.0f, 0.0f}),
		Vertex3D({0.5f, -0.5f, 1.0f}, {0.0f, 0.0f, 1.0f}),
		Vertex3D({-0.5f, -0.5f, 1.0f}, {1.0f, 0.0f, 0.0f}),
		},
	};

	renderComponents.push_back(triangle);
	renderComponents.push_back(triangle2);

	return true;
}

