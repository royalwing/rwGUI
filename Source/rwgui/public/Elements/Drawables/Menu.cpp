#include "Menu.h"
#include <Elements/Drawables/Layout.h>

void Menu::Init()
{
	const Color ActiveColor(0.5f, 0.5f, 0.5f, 1.0f);
	const Color DefaultColor(0.3f, 0.3f, 0.3f, 1.0f);
	const Color SubitemColor(0.4f, 0.4f, 0.4f, 1.0f);

	MenuLayout = new Layout("HeaderLayout", ELayoutType::LayoutType_HORIZONTAL);
	MenuLayout->SetContentPadding(0, 0, 0, 0);
	AddChild(MenuLayout);

	int entryId = 0;
	for(auto& Entry : Entries)
	{
		++entryId;
		{
			Button* btn = new Button(String("Button"), [&Entry, ActiveColor, DefaultColor, SubitemColor](Button* btn)->void {
				if(!Entry.Execute())
				{
					Entry.SetActive(!Entry.IsActive());
					btn->SetBackgroundColor(Entry.IsActive() ? ActiveColor : DefaultColor);
				}
			});
			btn->SetCaption(Entry.GetName());
			btn->SetSize(100, 24);
			btn->SetBackgroundColor(DefaultColor);
			btn->SetFontSize(12);
			btn->bIgnoreLayoutScaling = true;
			MenuLayout->AddChild(btn);
		}
	}

}

void Menu::Update(float DeltaTime)
{
	Drawable::Update(DeltaTime);
	SetSize(this->GetOuterBounds().Size.x, 32);
}


MenuEntry& Menu::operator[](String Entry)
{
	return Entries.FindOrAdd(MenuEntry(Entry));
}

MenuEntry& MenuEntry::operator[](String Entry)
{
	return Subentries.FindOrAdd(MenuEntry(Entry));
}

void MenuEntry::operator=(std::function<void()> inFunc)
{
	OnSelected = inFunc;
}

void MenuEntry::FilLLayout(Layout* inLayout)
{

	const Color ActiveColor(0.5f, 0.5f, 0.5f, 1.0f);
	const Color DefaultColor(0.3f, 0.3f, 0.3f, 1.0f);
	const Color SubitemColor(0.4f, 0.4f, 0.4f, 1.0f);

	for(auto& Entry : Subentries)
	{
		Button* btn = new Button(String("Button"), [&Entry, ActiveColor, DefaultColor, SubitemColor](Button* btn)->void {
			if (!Entry.Execute())
			{
				Entry.SetActive(!Entry.IsActive());
				btn->SetBackgroundColor(Entry.IsActive() ? ActiveColor : SubitemColor);
			}
		});
		btn->SetCaption(Entry.GetName());
		btn->SetSize(72, 24);
		btn->SetFontSize(12);
		btn->SetBackgroundColor(SubitemColor);
		btn->bIgnoreLayoutScaling = true;
		btn->SetVisilityLambda([this]() {
			return bActive;
		});
		inLayout->AddChild(btn);

		Entry.FilLLayout(inLayout);
	}
}


