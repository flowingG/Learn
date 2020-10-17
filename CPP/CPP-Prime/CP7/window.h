#ifndef __WINDOW_H
#define __WINDOW_H

#include <vector>
#include "screen.h"

class Window_mgr{
public:
	using screenIndex = std::vector<Screen>::size_type;
	void clear(screenIndex);
private:
	std::vector<Screen> screens{Screen(24, 80, ' ')};
};

void Window_mgr::clear(screenIndex i)
{
	Screen &s = screens[i];
	s.contents = string(s.height * s.width, ' ');
}

#endif
