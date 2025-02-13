#include "GraphicsDevice.h"

GraphicsDevice& GraphicsDevice::GetInstance()
{
	static GraphicsDevice instance;

	return instance;
}

ID3D11Device* GraphicsDevice::GetDevice()
{
	return device.Get();
}

ID3D11Device** GraphicsDevice::GetDeviceAddress()
{
	return device.GetAddressOf();
}

ID3D11DeviceContext* GraphicsDevice::GetContext()
{
	return deviceContext.Get();
}

ID3D11DeviceContext** GraphicsDevice::GetContextAddress()
{
	return deviceContext.GetAddressOf();
}
