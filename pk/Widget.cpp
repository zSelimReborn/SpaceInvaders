#include "Widget.h"

#include <iostream>

#include "AssetManager.h"

Widget::Widget()
	: bActive(false), Id(-1)
{
}

int Widget::GetId() const
{
	return Id;
}

void Widget::Construct()
{
}

void Widget::Render()
{
}

void Widget::Input(const InputHandler& Handler, const float Delta)
{
}

void Widget::Activate()
{
	bActive = true;
}

void Widget::Deactivate()
{
	bActive = false;
}

bool Widget::IsActive() const
{
	return bActive;
}

void Widget::SetScene(const SceneWeakPtr& InScene)
{
	ScenePtr = InScene;
}

Widget::SceneSharedPtr Widget::GetScene() const
{
	return ScenePtr.lock();
}

void Widget::RenderText(const std::string& InFontName, 
	const glm::vec2& StartLocation, const std::string& InText, 
	TextOrient Orient, float Scale, const glm::vec4& InColor,
	float& OutWidth, float& OutHeight
) {
	Font::SharedPtr Font = AssetManager::Get().GetFont(InFontName);
	if (Font == nullptr)
	{
		std::cout << "Unable to render text, no font with name " << InFontName << "\n";
		return;
	}


	Font->GetTextSize(InText, Scale, OutWidth, OutHeight);
	glm::vec2 TextLocation = StartLocation;
	if (Orient == TextOrient::Center)
	{
		TextLocation.x -= (OutWidth / 2);
	}
	else if (Orient == TextOrient::Right)
	{
		TextLocation.x -= OutWidth;
	}

	Font->Render(InText, TextLocation, Scale, InColor);
}
