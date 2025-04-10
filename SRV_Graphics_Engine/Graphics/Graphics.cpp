#include <direct.h>   
#include <iostream>

#include "Graphics.h"
#include "./Device/GraphicsDevice.h"
#include "ShaderManager/ShaderManager.h"
#include "../Engine/Asserter.h"


bool Graphics::Initialize(HWND hwnd, int width, int height)
{
	clientWidth = width;
	clientHeight = height;

	if (!InitializeDirectX(hwnd))
		return false;

	if (!InitializeShaders())
		return false;

	if (!InitializeScene())
		return false;

	camera = new Camera();
	camera->SetPosition(0.0f, 0.0f, -2.0f);
	camera->SetPerspectiveProjection(90, static_cast<float>(clientWidth) / static_cast<float>(clientHeight), 0.1f, 1000.0f);

	return true;
}

// ALL DRAWING IS HERE BETWEEN ClearRenderTargetView AND Present
void Graphics::RenderFrame()
{
	ID3D11ShaderResourceView* nullSRV[2] = { NULL, NULL }; 
	DeviceContext->PSSetShaderResources(0, 2, nullSRV);
	DeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	RenderShadows();

	DeviceContext->RSSetViewports(1, &viewport);

	DeviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());
	float bgcolor[] = { 0.0f, 0.0, 0.0f, 1.0f }; // background color
	DeviceContext->ClearRenderTargetView(renderTargetView.Get(), bgcolor);
	DeviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// set input layout, topology, rasterizer state
	DeviceContext->RSSetState(rasterizerState.Get());
	DeviceContext->OMSetDepthStencilState(depthStencilState.Get(), 0);

	// set shaders and samplers
	DeviceContext->PSSetSamplers(0, 1, this->samplerState.GetAddressOf());
	DeviceContext->PSSetSamplers(1, 1, this->shadowSamplerState.GetAddressOf());

	// View matrix
	worldMatrix = DirectX::XMMatrixIdentity();


	for (IRenderComponent* item : objectRenderPool)
	{
		item->Render();
	}

	swapchain->Present(1, NULL);
}

void Graphics::AddObjectToRenderPool(IRenderComponent* object)
{
	objectRenderPool.push_back(object);

	auto light = dynamic_cast<DirectionalLightComponent*>(object);
	if (light)
	{
		lightPool.push_back(light);
	}
}

const DirectX::XMMATRIX Graphics::GetWorldMatrix() const
{
	return worldMatrix;
}

Camera* Graphics::GetCamera() const
{
	return camera;
}

float Graphics::GetClientWidth() const
{
	return clientWidth;
}

float Graphics::GetClientHeight() const
{
	return clientHeight;
}

std::vector<DirectionalLightComponent*> Graphics::GetAllLights() const
{
	return lightPool;
}

void Graphics::RenderShadows()
{
	for (DirectionalLightComponent* item : lightPool)
	{
		item->RenderShadowPass(objectRenderPool);
	}
}

bool Graphics::InitializeDirectX(HWND hwnd)
{
	if (!CreateDeviceAndSwapChain(hwnd))
		return false;

	if (!CreateRenderTargetView())
		return false;

	if (!CreateDepthStencilBuffer())
		return false;

	DeviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());

	if (!CreateDepthStencilState())
		return false;

	CreateViewport();

	if (!CreateRasterizerState())
		return false;

	if (!CreateSamplerState())
		return false;

	return true;
}

bool Graphics::CreateDeviceAndSwapChain(HWND hwnd)
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
	swapchain_desc.BufferDesc.Width = clientWidth;
	swapchain_desc.BufferDesc.Height = clientHeight;
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
	HRESULT hr = swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(backBuffer.GetAddressOf()));

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
	rasterizerDesc.CullMode = D3D11_CULL_NONE;

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

	ThrowIfFailed(Device->CreateSamplerState(&samplerDesc, samplerState.GetAddressOf()), "Failed to create sampler state.");


	D3D11_SAMPLER_DESC shadowSamplerDesc = {};
	shadowSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	shadowSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP; 
	shadowSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP; 
	shadowSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP; 
	shadowSamplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
	shadowSamplerDesc.MinLOD = 0;
	shadowSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	shadowSamplerDesc.MipLODBias = 0; 

	ThrowIfFailed(Device->CreateSamplerState(&shadowSamplerDesc, shadowSamplerState.GetAddressOf()), 
		"Failed to create sampler state.");


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

bool Graphics::CreateDepthStencilBuffer()
{
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = clientWidth;
	depthStencilDesc.Height = clientHeight;
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

void Graphics::CreateViewport()
{
	viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = clientWidth;
	viewport.Height = clientHeight;
	viewport.MinDepth = 0.0;
	viewport.MaxDepth = 1.0;

	DeviceContext->RSSetViewports(1, &viewport);
}

bool Graphics::InitializeShaders()
{
	return ShaderManager::GetInstance().Initialize();
}

bool Graphics::InitializeScene()
{
	return true;
}

void Graphics::DrawGrid()
{

}
