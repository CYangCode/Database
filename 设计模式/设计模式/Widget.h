#pragma once
#include "HelpHandler.h"

class Widget: public HelpHandler
{
public:
	Widget(Widget * = nullptr, Topic = NO_HELP_TOPIC);
	~Widget();
private:
	Widget * _parent;
};

