#pragma once
#include "Widget.h"
class Button: public Widget
{
public:
	Button(Widget * = nullptr, Topic = NO_HELP_TOPIC);
	~Button();
	virtual void HandleHelp();
};

