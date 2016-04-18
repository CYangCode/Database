#include "HelpHandler.h"

HelpHandler::HelpHandler(HelpHandler * h, Topic t) :_successor(h), _topic(t)
{

}


HelpHandler::~HelpHandler()
{
	if (_successor != nullptr)
		delete _successor;
}

bool HelpHandler::HasHelp()
{
	return _topic != NO_HELP_TOPIC;
}

void HelpHandler::SetHandler(HelpHandler * h, Topic t)
{
	_successor = h;
	_topic = t;
}

void HelpHandler::HandleHelp()
{
	if (_successor != nullptr) {
		_successor->HandleHelp();
	}
}
