#pragma once


class IComponent
{
public:
	virtual void Update(const float& deltaTime) = 0;
	bool IsEnabled();
	void SetEnabled(bool value);

private:

	bool isEnabled = true;
};