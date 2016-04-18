#include "Widget.h"

Widget::Widget(Widget * h, Topic t) : HelpHandler(h, t)
{
	_parent = h;
}


Widget::~Widget()
{
}
