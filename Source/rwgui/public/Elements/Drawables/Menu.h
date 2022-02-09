#ifndef MENU_H
#define MENU_H

#include <Elements/Drawables/Background.h>
#include <Elements/Drawables/Button.h>

class RWGUI_API MenuEntry
{
public:
	MenuEntry() : Name("") {};
	MenuEntry(String inName) : Name(inName) {};

	bool operator==(const MenuEntry& Other) const { return Name == Other.Name; };

	MenuEntry& operator[](String Entry);
	void operator=(std::function<void()> inFunc);
	String GetName() const { return Name; };
	void SetActive(bool bIsActive) { bActive = bIsActive; };
	bool IsActive() const { return bActive; };

	void FilLLayout(class Layout* inLayout);

	bool Execute() { if (OnSelected) { OnSelected(); return true; } return false; };
private:

	class Layout* ContentLayout = nullptr;
	bool bActive = false;
	std::function<void()> OnSelected;
	String Name;
	List<MenuEntry> Subentries;
};

class RWGUI_API Menu : public Drawable
{
	using Drawable::Drawable;
public:
	virtual void Init() override;
	virtual void Update(float DeltaTime) override;

	MenuEntry& operator[](String Entry);
private:
	List<MenuEntry> Entries;
	class Layout* MenuLayout;
};

#endif