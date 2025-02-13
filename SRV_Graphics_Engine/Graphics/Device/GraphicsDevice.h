#pragma once

#include <wrl/client.h>
#include <d3d11.h>

#define DeviceContext GraphicsDevice::GetInstance().GetContext()
#define DeviceContextAddress GraphicsDevice::GetInstance().GetContextAddress()
#define Device GraphicsDevice::GetInstance().GetDevice()
#define DeviceAddress GraphicsDevice::GetInstance().GetDeviceAddress()


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

