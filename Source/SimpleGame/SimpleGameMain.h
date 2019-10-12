#pragma once
#include <rwgui.h>
#include <rwupd.h>

class SimpleGame : public Application
{
public:

	class World* pWorld = nullptr;
	class Viewport* pViewport = nullptr;

	String GetApplicationName() const override { return "SimpleGame"; };
	virtual void OnInit() override;
	void Update(float DeltaTime) override;
	virtual void OnStop() override;

	virtual void BuildPages() override;
};
