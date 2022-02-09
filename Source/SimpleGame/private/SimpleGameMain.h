#pragma once
#include <rwgui.h>

class SimpleGame : public Application
{
private:
	Vector2D Input;
	class PlayerCharacter* LocalPlayerCharacter;
public:

	class World* pWorld = nullptr;
	class Viewport* pViewport = nullptr;

	String GetApplicationName() const override { return "SimpleGame"; };
	virtual void OnInit() override;
	void Update(float DeltaTime) override;
	virtual void OnStop() override;
		
	virtual void OnKeyStateChanged(char key, bool bPressed) override;


	virtual void BuildPages() override;
};
