#pragma once
typedef int Topic;

const Topic NO_HELP_TOPIC = -1;
const Topic PRINT_TOPIC = 1;
const Topic PAPER_ORIENTATION_TOPIC = 2;
const Topic APPLICATION_TOPIC = 3;

class HelpHandler
{
public:
	HelpHandler(HelpHandler * = nullptr, Topic = NO_HELP_TOPIC);
	~HelpHandler();
	virtual bool HasHelp();
	virtual void SetHandler(HelpHandler *, Topic);
	virtual void HandleHelp();
private:
	HelpHandler * _successor;
	Topic _topic;
};

