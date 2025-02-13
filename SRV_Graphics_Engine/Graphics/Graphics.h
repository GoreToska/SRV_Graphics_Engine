#pragma once

#include<d3d11.h>
#include<wrl/client.h>
#include<vector>

#include "AdapterReader.h"
#include "ShadersClass/Shaders.h"
#include "../DataTypes/Vertex.h"
#include "RenderObjects/RenderComponent.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")


class Graphics
{
public:
	bool Initialize(HWND hwnd, int width, int height);
	void RenderFrame();

private:
	bool InitializeDirectX(HWND hwnd, int width, int height);
	bool CreateDeviceAndSwapChain(HWND hwnd, int width, int height);
	bool CreateRenderTargetView();
	bool CreateRasterizerState();
	bool CreateSamplerState();
	bool CreateDepthStencilState();
	bool CreateDepthStencilBuffer(int width, int height);
	void CreateViewport(int width, int height);
	bool InitializeShaders();
	bool InitializeScene();

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;

	VertexShader vertexShader;
	PixelShader pixelShader;

	std::vector<RenderComponent*> renderComponents{};

	Microsoft::WRL::ComPtr <ID3D11DepthStencilView> depthStencilView;
	Microsoft::WRL::ComPtr <ID3D11Texture2D> depthStencilBuffer;
	Microsoft::WRL::ComPtr <ID3D11DepthStencilState> depthStencilState;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;

	// TODO: font

	// Just for test
	Microsoft::WRL::ComPtr <ID3D11SamplerState> samplerState;
};

