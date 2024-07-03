#include "OptionsWindow.h"


#include <algorithm>
#include <array>
#include <map>


namespace
{
	constexpr SDL_Rect BgRect{ 0, 0, 256, 256 };
	constexpr SDL_Rect CheckedBox{ 261, 13, 14, 12 };

	enum class CheckBox
	{
		AutoBudget,
		AutoBulldoze,
		AutoGoto,
		DisastersEnabled,
		PlayMusic,
		PlaySound
	};

	std::array CheckBoxes =
	{
		std::make_tuple(CheckBox::AutoBudget, SDL_Rect{ 145, 172, 12, 12 }, false),
		std::make_tuple(CheckBox::AutoBulldoze, SDL_Rect{ 13, 172, 12, 12 }, false),
		std::make_tuple(CheckBox::AutoGoto, SDL_Rect{ 13, 188, 12, 12 }, false),
		std::make_tuple(CheckBox::DisastersEnabled, SDL_Rect{ 13, 204, 12, 12 }, false),
		std::make_tuple(CheckBox::PlayMusic, SDL_Rect{ 145, 204, 12, 12 }, false),
		std::make_tuple(CheckBox::PlaySound, SDL_Rect{ 145, 188, 12, 12 }, false)
	};

	
	void setOptionsFromCheckboxValues(OptionsWindow::Options& options)
	{
		options.autoBudget = std::get<bool>(CheckBoxes[0]);
		options.autoBulldoze = std::get<bool>(CheckBoxes[1]);
		options.autoGoto = std::get<bool>(CheckBoxes[2]);
		options.disastersEnabled = std::get<bool>(CheckBoxes[3]);
		options.playMusic = std::get<bool>(CheckBoxes[4]);
		options.playSound = std::get<bool>(CheckBoxes[5]);
	}


	template <typename BaseType>
	SDL_Point pointToSdlPoint(const Point<BaseType>& point)
	{
		return { point.x, point.y };
	}


	template <typename BaseType>
	SDL_Rect adjustedRect(const SDL_Rect& source, const Point<BaseType>& offset)
	{
		return { source.x + offset.x, source.y + offset.y, source.w, source.h };
	}


	void postQuit()
	{
		SDL_Event event{};
		event.type = SDL_QUIT;
		SDL_PushEvent(&event);
	}
};


OptionsWindow::OptionsWindow(SDL_Renderer* renderer):
	mRenderer{ renderer },
	mTexture(loadTexture(renderer, "images/OptionsWindow.png")),
	mCheckTexture(newTexture(renderer, mTexture.dimensions))
{
    size({ BgRect.w, BgRect.h });
    closeButtonActive(false);
	anchor();

	SDL_SetTextureBlendMode(mCheckTexture.texture, SDL_BLENDMODE_BLEND);

	initButtons();
}


void OptionsWindow::initButtons()
{
	/*
	std::make_tuple(OptionsWindow::Button::New, SDL_Rect{ 61, 63, 142, 20 }),
	std::make_tuple(OptionsWindow::Button::Open, SDL_Rect{ 61, 87, 142, 20 }),
	std::make_tuple(OptionsWindow::Button::Save, SDL_Rect{ 61, 111, 142, 20 }),
	*/

	mButtons.emplace_back(Button{ std::bind(&OptionsWindow::buttonReturnClicked, this), SDL_Rect{61, 31, 142, 20} });
	mButtons.emplace_back(Button{ std::bind(&OptionsWindow::buttonQuitClicked, this), SDL_Rect{61, 145, 142, 20} });
	mButtons.emplace_back(Button{ std::bind(&OptionsWindow::buttonAcceptClicked, this), SDL_Rect{61, 224, 142, 20} });
}


void OptionsWindow::optionsChangedConnect(CallbackOptionsChanged callback)
{
	mOptionsChangedCallback = callback;
}


void OptionsWindow::optionsChangedDisconnect(CallbackOptionsChanged callback)
{
	mOptionsChangedCallback = nullptr;
}


void OptionsWindow::setOptions(const Options& options)
{
	mOptions = options;

	/**
	 * These options are arranged in alphabetical order
	 * both in the options struct and in the array for
	 * options.
	 */
	std::get<bool>(CheckBoxes[0]) = options.autoBudget;
	std::get<bool>(CheckBoxes[1]) = options.autoBulldoze;
	std::get<bool>(CheckBoxes[2]) = options.autoGoto;
	std::get<bool>(CheckBoxes[3]) = options.disastersEnabled;
	std::get<bool>(CheckBoxes[4]) = options.playMusic;
	std::get<bool>(CheckBoxes[5]) = options.playSound;

	drawChecks();
}


void OptionsWindow::draw()
{
	const SDL_Rect rect{ area().x, area().y, area().width, area().height };
	SDL_RenderCopy(mRenderer, mTexture.texture, &BgRect, &rect);
	SDL_RenderCopy(mRenderer, mCheckTexture.texture, &BgRect, &rect);
}


void OptionsWindow::onMouseDown(const Point<int>& point)
{
	checkCheckboxesForClick(point);
	
	drawChecks();
	setOptionsFromCheckboxValues(mOptions);
	
	checkButtonsForClick(point);
}


void OptionsWindow::onKeyDown(int32_t key)
{
	switch (key)
	{
	case SDLK_ESCAPE:
		hide();
		break;

	case SDLK_RETURN:
		optionsChangedTrigger();
		hide();
		break;

	default:
		break;
	}
}


void OptionsWindow::onShow()
{
	drawChecks();
}


void OptionsWindow::checkCheckboxesForClick(const Point<int>& point)
{
	for (auto& [checkbox, rect, checked] : CheckBoxes)
	{
		const auto clickPoint = pointToSdlPoint(point);
		const auto adjustedButtonRect = adjustedRect(rect, area().startPoint());

		if (SDL_PointInRect(&clickPoint, &adjustedButtonRect))
		{
			checked = !checked;
		}
	}
}


void OptionsWindow::checkButtonsForClick(const Point<int>& point)
{
	for (auto& button : mButtons)
	{
		const auto clickPoint = pointToSdlPoint(point);
		const auto adjustedButtonRect = adjustedRect(button.area(), area().startPoint());
		if (SDL_PointInRect(&clickPoint, &adjustedButtonRect))
		{
			button.click();
		}
	}
}


void OptionsWindow::buttonReturnClicked()
{
	hide();
}


void OptionsWindow::buttonAcceptClicked()
{
	optionsChangedTrigger();
	hide();
}


void OptionsWindow::buttonQuitClicked()
{
	postQuit();
}


void OptionsWindow::drawChecks()
{
	flushTexture(mRenderer, mCheckTexture);

	for (const auto& [checkbox, rect, checked] : CheckBoxes)
	{
		if (checked)
		{
			const SDL_Rect adjustedRect{ rect.x, rect.y, CheckedBox.w, CheckedBox.h };
			SDL_RenderCopy(mRenderer, mTexture.texture, &CheckedBox, &adjustedRect);
		}
	}

	SDL_SetRenderTarget(mRenderer, nullptr);
}


void OptionsWindow::optionsChangedTrigger()
{
	mOptionsChangedCallback(mOptions);
}
