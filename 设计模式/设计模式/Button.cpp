#include "Button.h"


Button::Button()
{
}

Button::Button(Widget * h, Topic t) : Widget(h, t)
{

}


Button::~Button()
{
}

void Button::HandleHelp()
{
	if (HasHelp()) {
		//Ã·π©∞Ô÷˙
	}
	else {
		HelpHandler::HandleHelp();
	}
}
