#include "GBuffer.h"
#include "../Device/GraphicsDevice.h"
#include "../../Engine/Engine.h"

GBuffer::GBuffer()
{
	Initialize();
}

void GBuffer::SetRenderTargets() const
{
	ID3D11RenderTargetView* rtvs[] = {
		depthRTV.Get(),
		normalRTV.Get(),
		diffuseRTV.Get(),
		specularRTV.Get()
	};

	SRVDeviceContext->OMSetRenderTargets(4, rtvs, SRVEngine.GetGraphics().GetDepthStencilView());
}

void GBuffer::PSBindResourceViews(int startIndex) const
{
	ID3D11ShaderResourceView* rsvs[] = {
		depthSRV.Get(),
		normalSRV.Get(),
		diffuseSRV.Get(),
		specularSRV.Get(),
		SRVEngine.GetGraphics().GetDepthStencilSRV()
	};

	SRVDeviceContext->PSSetShaderResources(startIndex, 5, rsvs);
}

void GBuffer::ClearRenderTargets() const
{
	const float clearColorWhite[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	const float clearColorBlack[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	SRVDeviceContext->ClearRenderTargetView(depthRTV.Get(), clearColorWhite);
	SRVDeviceContext->ClearRenderTargetView(normalRTV.Get(), clearColorBlack);
	SRVDeviceContext->ClearRenderTargetView(diffuseRTV.Get(), clearColorBlack);
	SRVDeviceContext->ClearRenderTargetView(specularRTV.Get(), clearColorBlack);
}

void GBuffer::Initialize()
{
	D3D11_TEXTURE2D_DESC depthTexDesc = {};
	depthTexDesc.Width = SRVEngine.GetGraphics().GetClientWidth();
	depthTexDesc.Height = SRVEngine.GetGraphics().GetClientHeight();
	depthTexDesc.MipLevels = 1;
	depthTexDesc.ArraySize = 1;
	depthTexDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
	depthTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	depthTexDesc.CPUAccessFlags = 0;
	depthTexDesc.MiscFlags = 0;
	depthTexDesc.SampleDesc = { 1, 0 };

	ThrowIfFailed(SRVDevice->CreateTexture2D(&depthTexDesc, nullptr, &depth), "Failed to create depth"); ;


	D3D11_SHADER_RESOURCE_VIEW_DESC depthSrvDesc = {};
	depthSrvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	depthSrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	depthSrvDesc.Texture2D.MipLevels = 1;
	depthSrvDesc.Texture2D.MostDetailedMip = 0;

	D3D11_RENDER_TARGET_VIEW_DESC depthRtvDesc = {};
	depthRtvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	depthRtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	depthRtvDesc.Texture2D.MipSlice = 0;

	ThrowIfFailed(SRVDevice->CreateShaderResourceView(depth.Get(), &depthSrvDesc, &depthSRV), "Failed.");
	ThrowIfFailed(SRVDevice->CreateRenderTargetView(depth.Get(), &depthRtvDesc, &depthRTV), "Failed.");


	D3D11_TEXTURE2D_DESC normalTexDesc = {};
	normalTexDesc.Width = SRVEngine.GetGraphics().GetClientWidth();
	normalTexDesc.Height = SRVEngine.GetGraphics().GetClientHeight();
	normalTexDesc.MipLevels = 1;
	normalTexDesc.ArraySize = 1;
	normalTexDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	normalTexDesc.Usage = D3D11_USAGE_DEFAULT;
	normalTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	normalTexDesc.CPUAccessFlags = 0;
	normalTexDesc.MiscFlags = 0;
	normalTexDesc.SampleDesc = { 1, 0 };

	ThrowIfFailed(SRVDevice->CreateTexture2D(&normalTexDesc, nullptr, &normal), "Failed.");


	D3D11_SHADER_RESOURCE_VIEW_DESC normalSrvDesc = {};
	normalSrvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	normalSrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	normalSrvDesc.Texture2D.MipLevels = 1;
	normalSrvDesc.Texture2D.MostDetailedMip = 0;
	D3D11_RENDER_TARGET_VIEW_DESC normalRtvDesc = {};
	normalRtvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	normalRtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	normalRtvDesc.Texture2D.MipSlice = 0;

	ThrowIfFailed(SRVDevice->CreateShaderResourceView(normal.Get(), &normalSrvDesc, &normalSRV), "Failed");
	ThrowIfFailed(SRVDevice->CreateRenderTargetView(normal.Get(), &normalRtvDesc, &normalRTV), "Failed");


	D3D11_TEXTURE2D_DESC diffuseTexDesc = {};
	diffuseTexDesc.Width = SRVEngine.GetGraphics().GetClientWidth();
	diffuseTexDesc.Height = SRVEngine.GetGraphics().GetClientHeight();
	diffuseTexDesc.MipLevels = 1;
	diffuseTexDesc.ArraySize = 1;
	diffuseTexDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	diffuseTexDesc.Usage = D3D11_USAGE_DEFAULT;
	diffuseTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	diffuseTexDesc.CPUAccessFlags = 0;
	diffuseTexDesc.MiscFlags = 0;
	diffuseTexDesc.SampleDesc = { 1, 0 };

	ThrowIfFailed(SRVDevice->CreateTexture2D(&diffuseTexDesc, nullptr, &diffuse), "Failed.");


	D3D11_SHADER_RESOURCE_VIEW_DESC diffuseSrvDesc = {};
	diffuseSrvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	diffuseSrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	diffuseSrvDesc.Texture2D.MipLevels = 1;
	diffuseSrvDesc.Texture2D.MostDetailedMip = 0;
	D3D11_RENDER_TARGET_VIEW_DESC diffuseRtvDesc = {};
	diffuseRtvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	diffuseRtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	diffuseRtvDesc.Texture2D.MipSlice = 0;

	ThrowIfFailed(SRVDevice->CreateShaderResourceView(diffuse.Get(), &diffuseSrvDesc, &diffuseSRV), "Failed.");
	ThrowIfFailed(SRVDevice->CreateRenderTargetView(diffuse.Get(), &diffuseRtvDesc, &diffuseRTV), "Failed");

	D3D11_TEXTURE2D_DESC specTexDesc = {};
	specTexDesc.Width = SRVEngine.GetGraphics().GetClientWidth();
	specTexDesc.Height = SRVEngine.GetGraphics().GetClientHeight();
	specTexDesc.MipLevels = 1;
	specTexDesc.ArraySize = 1;
	specTexDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	specTexDesc.Usage = D3D11_USAGE_DEFAULT;
	specTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	specTexDesc.CPUAccessFlags = 0;
	specTexDesc.MiscFlags = 0;
	specTexDesc.SampleDesc = { 1, 0 };

	ThrowIfFailed(SRVDevice->CreateTexture2D(&specTexDesc, nullptr, &specular), "Failed.");

	D3D11_SHADER_RESOURCE_VIEW_DESC specSrvDesc = {};
	specSrvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	specSrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	specSrvDesc.Texture2D.MipLevels = 1;
	specSrvDesc.Texture2D.MostDetailedMip = 0;
	D3D11_RENDER_TARGET_VIEW_DESC specRtvDesc = {};
	specRtvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	specRtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	specRtvDesc.Texture2D.MipSlice = 0;

	ThrowIfFailed(SRVDevice->CreateShaderResourceView(specular.Get(), &specSrvDesc, &specularSRV), "Failed.");
	ThrowIfFailed(SRVDevice->CreateRenderTargetView(specular.Get(), &specRtvDesc, &specularRTV), "Failed.");
}
