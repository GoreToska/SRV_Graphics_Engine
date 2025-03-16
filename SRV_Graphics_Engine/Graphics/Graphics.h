#pragma once

#pragma region Includes
#include "AdapterReader.h"
#include "ShadersClass/Shaders.h"
#include "../DataTypes/Vertex.h"
#include "../ComponentSystem/GameObject.h"
#include "Camera.h"
#include "../ComponentSystem/Components/Render/MeshRendererComponent.h"

#include<d3d11.h>
#include<wrl/client.h>
#include<vector>
#include <WICTextureLoader.h>
#include "Grid/Grid.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")  
#pragma endregion


class Graphics
{
public:
	bool Initialize(HWND hwnd, int width, int height);
	void RenderFrame();
	void AddObjectToRenderPool(IRenderComponent* object);

	const DirectX::XMMATRIX GetWorldMatrix() const;
	Camera* GetCamera() const;
	float GetClientWidth() const;
	float GetClientHeight() const;

private:
	bool InitializeDirectX(HWND hwnd);
	bool CreateDeviceAndSwapChain(HWND hwnd);
	bool CreateRenderTargetView();
	bool CreateRasterizerState();
	bool CreateSamplerState();
	bool CreateDepthStencilState();
	bool CreateDepthStencilBuffer();
	void CreateViewport();
	bool InitializeShaders();
	bool InitializeScene();

	void DrawGrid();

	int clientWidth;
	int clientHeight;

	Camera* camera;
	DirectX::XMMATRIX worldMatrix;

	Grid* grid;

	std::vector<IRenderComponent*> objectRenderPool = {};

	Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;

	Microsoft::WRL::ComPtr <ID3D11DepthStencilView> depthStencilView;
	Microsoft::WRL::ComPtr <ID3D11Texture2D> depthStencilBuffer;
	Microsoft::WRL::ComPtr <ID3D11DepthStencilState> depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
	Microsoft::WRL::ComPtr <ID3D11SamplerState> samplerState;
};

