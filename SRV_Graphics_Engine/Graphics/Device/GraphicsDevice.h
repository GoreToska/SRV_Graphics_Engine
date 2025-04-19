#pragma once

#include <wrl/client.h>
#include <d3d11.h>

#define SRVDeviceContext GraphicsDevice::GetInstance().GetContext()
#define SRVDeviceContextAddress GraphicsDevice::GetInstance().GetContextAddress()
#define SRVDevice GraphicsDevice::GetInstance().GetDevice()
#define SRVDeviceAddress GraphicsDevice::GetInstance().GetDeviceAddress()


class GraphicsDevice
{
public:
	static GraphicsDevice& GetInstance();

	ID3D11Device* GetDevice();
	ID3D11Device** GetDeviceAddress();
	ID3D11DeviceContext* GetContext();
	ID3D11DeviceContext** GetContextAddress();

private:
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
};

