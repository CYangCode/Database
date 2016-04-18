#include "antlr3.h"

#include "MyElangLexer.h"
#include "MyElangParser.h"
#include <cassert>
#include <string>
#include <iostream>
#include "SynTreeNode.h"

using std::string;
using std::cout;
using std::endl;

int main(int argc, char **argv) {
	SqlTool * head = new SqlTool(argv[1]);
//	head->displayTree();
	while (head->hasNextChild()) {
		SqlTool * temp = head->getNextChild();
		temp->displayTree();
	}
//	head->getChild(0)->displayTree();
//	head->getChild(1)->displayTree();
//	return 0;
//	head = head->getNextChild();
//	while (head->hasNextChild()) {
//		SynTreeNode * temp = head->getNextChild();
//		if (temp->getTokenType() == 38)
//			while (temp->hasNextChild()) {
//				cout << temp->getNextChild()->getText() << endl;
//			}
//	}
}
