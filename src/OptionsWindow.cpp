#include "OptionsWindow.h"


#include <array>
#include <map>


namespace
{
	constexpr SDL_Rect BgRect{ 0, 0, 256, 256 };
	constexpr SDL_Rect CheckedBox{ 231, 13, 14, 12 };

	/*
	constexpr SDL_Rect BrnReturn{ 61, 31, 142, 20 };
	constexpr SDL_Rect BtnNew{ 61, 63, 142, 20 };
	constexpr SDL_Rect BtnOpen{ 61, 87, 142, 20 };
	constexpr SDL_Rect BtnSave{ 61, 111, 142, 20 };
	constexpr SDL_Rect BtnQuit{ 61, 145, 142, 20 };
	constexpr SDL_Rect BtnAccept{ 61, 224, 142, 20 };
	*/

	enum class Button
	{
		Return,
		New,
		Open,
		Save,
		Quit,
		Accept
	};


	constexpr std::array Buttons =
	{
		std::make_tuple(Button::Return, SDL_Rect{ 61, 31, 142, 20 }),
		std::make_tuple(Button::New, SDL_Rect{ 61, 63, 142, 20 }),
		std::make_tuple(Button::Open, SDL_Rect{ 61, 87, 142, 20 }),
		std::make_tuple(Button::Save, SDL_Rect{ 61, 111, 142, 20 }),
		std::make_tuple(Button::Quit, SDL_Rect{ 61, 145, 142, 20 }),
		std::make_tuple(Button::Accept, SDL_Rect{ 61, 224, 142, 20 })
	};

	enum class CheckBox
	{
		AutoBudget,
		AutoBulldoze,
		AutoGoto,
		DisastersEnabled,
		PlayMusic,
		PlaySound
	};

	constexpr std::array CheckBoxes =
	{
		std::make_tuple(CheckBox::AutoBudget, SDL_Rect{ 145, 172, 12, 12 }),
		std::make_tuple(CheckBox::AutoBulldoze, SDL_Rect{ 13, 172, 12, 12 }),
		std::make_tuple(CheckBox::AutoGoto, SDL_Rect{ 13, 188, 12, 12 }),
		std::make_tuple(CheckBox::DisastersEnabled, SDL_Rect{ 13, 204, 12, 12 }),
		std::make_tuple(CheckBox::PlayMusic, SDL_Rect{ 145, 204, 12, 12 }),
		std::make_tuple(CheckBox::PlaySound, SDL_Rect{ 145, 188, 12, 12 })
	};
};


OptionsWindow::OptionsWindow(SDL_Renderer* renderer):
	mRenderer{ renderer },
	mTexture(loadTexture(renderer, "images/OptionsWindow.png")),
	mCheckTexture(newTexture(renderer, mTexture.dimensions))
{
    size({ BgRect.w, BgRect.h });
    closeButtonActive(false);
}


void OptionsWindow::draw()
{
	const SDL_Rect rect{ area().x, area().y, area().width, area().height };
	SDL_RenderCopy(mRenderer, mTexture.texture, &BgRect, &rect);
	SDL_RenderCopy(mRenderer, mCheckTexture.texture, &BgRect, &rect);
}


void OptionsWindow::onMouseDown(const Point<int>&)
{
}


void OptionsWindow::onShow()
{
}
