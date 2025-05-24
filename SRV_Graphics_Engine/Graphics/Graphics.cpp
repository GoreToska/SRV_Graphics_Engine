#include <direct.h>   
#include <iostream>

#include "Graphics.h"
#include "./Device/GraphicsDevice.h"
#include "ShaderManager/ShaderManager.h"
#include "../Engine/Asserter.h"
#include "Buffers/GBuffer.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"


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

	gBuffer = new GBuffer();

	deferred_objectMatrixBuffer.Initialize();

	InitImGui(hwnd);

	return true;
}

void Graphics::InitImGui(HWND hwnd)
{ 
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& IO = ImGui::GetIO();
	ImGui_ImplWin32_Init(hwnd);

	if (!ImGui_ImplDX11_Init(SRVDevice, SRVDeviceContext))
	{
		MessageBox(nullptr, L"Failed to initialize ImGui DX11", L"Error", MB_OK);
		return;
	}

	ImGui::StyleColorsDark();
}

void Graphics::RenderFrame()
{
	RenderShadows();

	float bgcolor[] = { 0.0f, 0.0, 0.0f, 1.0f }; // background color
	SRVDeviceContext->ClearRenderTargetView(renderTargetView.Get(), bgcolor);
	SRVDeviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	FillGBuffer();

	SRVDeviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), nullptr);
	
	SRVDeviceContext->RSSetState(rasterizerState.Get());
	SRVDeviceContext->OMSetDepthStencilState(depthStencilState.Get(), 0);

	SRVDeviceContext->PSSetSamplers(0, 1, this->samplerState.GetAddressOf());
	SRVDeviceContext->PSSetSamplers(1, 1, this->shadowSamplerState.GetAddressOf());
	
	worldMatrix = DirectX::XMMatrixIdentity();

	SetConstBufferForLight();
	SRVDeviceContext->PSSetConstantBuffers(2, 1, deferred_objectMatrixBuffer.GetAddressOf());
	gBuffer->PSBindResourceViews(2);

	for (auto& light : GetAllLights())
	{
		SRVDeviceContext->PSSetConstantBuffers(0, 1, light->UpdateLightConstBuffer().GetAddressOf());
		
		if (light->GetSourceType() == Directional)
		{
			DrawDeferredScreenQuad();
		}
	}


	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	swapchain->Present(1, NULL);
}

void Graphics::FillGBuffer()
{
	SRVDeviceContext->IASetInputLayout(ShaderManager::GetInstance().GetVS(ShaderManager::Deferred_Opaque)->GetInputLayout());
	SRVDeviceContext->VSSetShader(ShaderManager::GetInstance().GetVS(ShaderManager::Deferred_Opaque)->GetShader(), NULL, 0);
	SRVDeviceContext->PSSetShader(ShaderManager::GetInstance().GetPS(ShaderManager::Deferred_Opaque)->GetShader(), NULL, 0);
	SRVDeviceContext->RSSetViewports(1, &viewport);
	SRVDeviceContext->PSSetSamplers(0, 1, this->samplerState.GetAddressOf());
	SRVDeviceContext->RSSetState(rasterizerState.Get());
	SRVDeviceContext->OMSetDepthStencilState(depthStencilState.Get(), 0);
	SRVDeviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	gBuffer->SetRenderTargets();
	gBuffer->ClearRenderTargets();

	for (IRenderComponent* item : objectRenderPool)
	{
		item->Render(false);
	}
}

void Graphics::DrawDeferredScreenQuad()
{
	SRVDeviceContext->IASetInputLayout(ShaderManager::GetInstance().GetVS(ShaderManager::Deferred_Light)->GetInputLayout());
	SRVDeviceContext->VSSetShader(ShaderManager::GetInstance().GetVS(ShaderManager::Deferred_Light)->GetShader(), NULL, 0);
	SRVDeviceContext->PSSetShader(ShaderManager::GetInstance().GetPS(ShaderManager::Deferred_Light)->GetShader(), NULL, 0);


	std::vector<Vector4D> verts = { Vector4D() };
	std::vector<DWORD> idcs = { 0 };
	std::vector<UINT> mockOffsets = { 0 };
	VertexBuffer<Vector4D> vertexBuffer = {};
	IndexBuffer indexBuffer = {};
	vertexBuffer.Initialize(verts.data(), verts.size());
	indexBuffer.Initialize(idcs.data(), idcs.size());
	std::vector<UINT> mockStrides = { 0 };

	SRVDeviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), mockStrides.data(), mockOffsets.data());
	SRVDeviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	SRVDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	SRVDeviceContext->Draw(4, 0);
}

void Graphics::AddObjectToRenderPool(IRenderComponent* object)
{
	objectRenderPool.push_back(object);

	auto light = dynamic_cast<LightComponent*>(object);
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

std::vector<LightComponent*> Graphics::GetAllLights() const
{
	return lightPool;
}

ID3D11DepthStencilView* Graphics::GetDepthStencilView()
{
	return depthStencilView.Get();
}

ID3D11ShaderResourceView* Graphics::GetDepthStencilSRV()
{
	return depthStencilSRV.Get();
}

void Graphics::RenderShadows()
{
	for (LightComponent* item : lightPool)
	{
		item->RenderShadowPass(objectRenderPool);
	}
}

void Graphics::SetConstBufferForLight()
{
	deferred_objectMatrixBuffer.GetData()->world = Matrix::Identity.Transpose();
	deferred_objectMatrixBuffer.GetData()->view = camera->GetViewMatrix().Transpose();
	deferred_objectMatrixBuffer.GetData()->projection = camera->GetProjectionMatrix().Transpose();
	deferred_objectMatrixBuffer.GetData()->inverseView = camera->GetViewMatrix().Invert().Transpose();
	deferred_objectMatrixBuffer.GetData()->inverseProjection = camera->GetProjectionMatrix().Invert().Transpose();

	deferred_objectMatrixBuffer.ApplyChanges();
}

bool Graphics::InitializeDirectX(HWND hwnd)
{
	if (!CreateDeviceAndSwapChain(hwnd))
		return false;

	if (!CreateRenderTargetView())
		return false;

	if (!CreateDepthStencilBuffer())
		return false;

	SRVDeviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());

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
		SRVDeviceAddress,
		NULL, // supported feature level
		SRVDeviceContextAddress);

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

	hr = SRVDevice->CreateRenderTargetView(backBuffer.Get(), NULL, renderTargetView.GetAddressOf());

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

	HRESULT hr = SRVDevice->CreateRasterizerState(&rasterizerDesc, rasterizerState.GetAddressOf());

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

	ThrowIfFailed(SRVDevice->CreateSamplerState(&samplerDesc, samplerState.GetAddressOf()), "Failed to create sampler state.");


	D3D11_SAMPLER_DESC shadowSamplerDesc = {};
	shadowSamplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	shadowSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	shadowSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	shadowSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	shadowSamplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
	shadowSamplerDesc.MinLOD = 0;
	shadowSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	shadowSamplerDesc.MipLODBias = 0;
	shadowSamplerDesc.BorderColor[0] = 1.0f;
	shadowSamplerDesc.BorderColor[1] = 1.0f;
	shadowSamplerDesc.BorderColor[2] = 1.0f;
	shadowSamplerDesc.BorderColor[3] = 1.0f;


	ThrowIfFailed(SRVDevice->CreateSamplerState(&shadowSamplerDesc, shadowSamplerState.GetAddressOf()),
		"Failed to create sampler state.");

	return true;
}

bool Graphics::CreateDepthStencilState()
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	HRESULT hr = SRVDevice->CreateDepthStencilState(&depthStencilDesc, depthStencilState.GetAddressOf());

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
	depthStencilDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	depthStencilDesc.SampleDesc = { 1, 0 };

	HRESULT hr = SRVDevice->CreateTexture2D(&depthStencilDesc, NULL, depthStencilTexture.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::LogError(hr, "Failed to create depth stencil buffer.");
		return false;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	hr = SRVDevice->CreateDepthStencilView(depthStencilTexture.Get(), &dsvDesc, depthStencilView.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::LogError(hr, "Failed to create depth stencil view.");
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	ThrowIfFailed(SRVDevice->CreateShaderResourceView(depthStencilTexture.Get(), &srvDesc, depthStencilSRV.GetAddressOf()), "Failed.");

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

	SRVDeviceContext->RSSetViewports(1, &viewport);
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
