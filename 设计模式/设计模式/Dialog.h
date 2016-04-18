#pragma once
#include "Widget.h"

class Dialog: public Widget
{
public:
	Dialog(HelpHandler * = nullptr, Topic = NO_HELP_TOPIC);
	~Dialog();
};

