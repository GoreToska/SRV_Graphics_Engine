#pragma once

#pragma region Includes
#include "AdapterReader.h"
#include "ShadersClass/Shaders.h"
#include "../DataTypes/Vertex.h"
#include "../ComponentSystem/GameObject.h"
#include "Camera.h"
#include "../ComponentSystem/Components/Render/MeshRendererComponent.h"
#include "../ComponentSystem/Components/Light/LightComponent.h"
#include "Grid/Grid.h"

#include<d3d11.h>
#include<wrl/client.h>
#include<vector>
#include <WICTextureLoader.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")  
#pragma endregion

class GBuffer;
class AABB;

class Graphics
{
public:
	bool Initialize(HWND hwnd, int width, int height);
	void RenderFrame();

	void DrawDeferredLight();

	void AddObjectToRenderPool(IRenderComponent* object);

	const DirectX::XMMATRIX GetWorldMatrix() const;
	Camera* GetCamera() const;
	float GetClientWidth() const;
	float GetClientHeight() const;
	//std::vector<LightComponent*> GetAllLights() const;
	LightComponent* GetDirectionalLight() const;
	ID3D11DepthStencilView* GetDepthStencilView();
	ID3D11ShaderResourceView* GetDepthStencilSRV();


private:
	void RenderShadows();

	void DrawDeferredOpaque();
	void DrawDeferredScreenQuad();
	void DrawDeferredAABB(const AABB& box, LightComponent& lightSource);

	void SetConstBufferForLight();

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
	void InitImGui(HWND hwnd);

	void DrawGrid();

	int clientWidth;
	int clientHeight;

	Camera* camera;
	LightComponent* directionalLight;
	DirectX::XMMATRIX worldMatrix;

	GBuffer* gBuffer;

	std::vector<IRenderComponent*> objectRenderPool = {};
	std::vector<LightComponent*> lightPool = {};

	Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;

	Microsoft::WRL::ComPtr <ID3D11DepthStencilView> depthStencilView;
	Microsoft::WRL::ComPtr <ID3D11ShaderResourceView> depthStencilSRV;
	Microsoft::WRL::ComPtr <ID3D11Texture2D> depthStencilTexture;
	Microsoft::WRL::ComPtr <ID3D11DepthStencilState> depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
	Microsoft::WRL::ComPtr <ID3D11SamplerState> samplerState;
	Microsoft::WRL::ComPtr <ID3D11SamplerState> shadowSamplerState;

	ConstantBuffer<VS_ObjectMatrixBuffer> deferred_objectMatrixBuffer = {};

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> NoWriteNoCheckDSS;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> NoWriteGreaterDSS;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> NoWriteLessDSS;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rastStateCullFront;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rastStateCullBack;

	Microsoft::WRL::ComPtr<ID3D11BlendState> additiveBlendState;

	Microsoft::WRL::ComPtr <ID3D11ShaderResourceView> decal;


	D3D11_VIEWPORT viewport;
};

