/*
 * SynTreeNode.h
 *
 *  Created on: 2016年3月11日
 *      Author: cyang
 */

#ifndef SYNTREENODE_H_
#define SYNTREENODE_H_

#include "MyElangLexer.h"
#include "MyElangParser.h"
#include <cassert>
#include <string>
#include <iostream>

class SqlTool {
public:
	SqlTool(const char * filename);
	SqlTool * getNextChild();
	SqlTool * getChild(int i);
	int getChildCount();
	bool hasNextChild();
	const char* getText();
	int getTokenType();
	void displayTree();
private:
	pANTLR3_INPUT_STREAM input;
	pMyElangLexer lex;
	pANTLR3_COMMON_TOKEN_STREAM tokens;
	pMyElangParser parser;
	pANTLR3_BASE_TREE tree;
	int childNum;
	SqlTool(pANTLR3_BASE_TREE t);
};

#endif
