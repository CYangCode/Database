/*
 * SynTreeNode.cpp
 *
 *  Created on: 2016年3月11日
 *      Author: cyang
 */

#include "SynTreeNode.h"


SqlTool::SqlTool(pANTLR3_BASE_TREE t) :
		input(NULL), lex(NULL), tokens(NULL), parser(NULL), tree(NULL), childNum(0) {
	tree = t;
}

SqlTool::SqlTool(const char * filename) :
	input(NULL), lex(NULL), tokens(NULL), parser(NULL), tree(NULL), childNum(0) {
	input = antlr3FileStreamNew((pANTLR3_UINT8) filename, ANTLR3_ENC_8BIT);
	lex = MyElangLexerNew(input);

	tokens = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT,
			TOKENSOURCE(lex));
	parser = MyElangParserNew(tokens);

	MyElangParser_start_rule_return_struct r = parser->start_rule(parser);

	tree = r.tree;
}

SqlTool * SqlTool::getChild(int i) {
	pANTLR3_BASE_TREE child = (pANTLR3_BASE_TREE)tree->getChild(tree, i);
	if (child)
		return new SqlTool(child);
	return NULL;
}

int SqlTool::getChildCount() {
	return tree->getChildCount(tree);
}

bool SqlTool::hasNextChild() {
	if (tree->getChild(tree, childNum)) {
		return true;
	}
	return false;
}

const char * SqlTool::getText() {
	return (const char*) tree->getText(tree)->chars;
}

int SqlTool::getTokenType() {
	pANTLR3_COMMON_TOKEN token = tree->getToken(tree);
	return token->type;
}

void SqlTool::displayTree() {
	pANTLR3_STRING s = (tree)->toStringTree((tree));
	printf(" Tree %s\n", s->chars);
}
/**
 * 得到第i个子节点
 */
SqlTool * SqlTool::getNextChild() {
	return new SqlTool((pANTLR3_BASE_TREE)tree->getChild(tree, childNum++));
}

