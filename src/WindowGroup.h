#pragma once

#include <vector>

class WindowBase;

/**
 * Thin wrapper class for convenience of working with groups of
 * Windows.
 */
class WindowGroup
{
public:
	bool windowVisible() const;

	void addWindow(WindowBase*);

	void hide();

	void draw();
	void update();

private:
	std::vector<WindowBase*> mWindows;
};
