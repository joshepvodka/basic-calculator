#ifndef __LEXER_H__
#define __LEXER_H__

#include <ctype.h>

bool isOperator(const char& chr) {
	if (chr == '+' ||
		chr == '-' ||
		chr == '*' ||
		chr == '/' ||
		chr == '^') {
		return true;
	}
	return false;
}

static bool isSign(const char& chr) {
	if (chr == '+' ||
		chr == '-') {
		return true;
	}
	return false;
}

static bool isBrakets(const char& chr) {
	if (chr == '(' ||
		chr == ')') {
		return true;
	}
	return false;
}

static bool isNumber(const char& chr) {
	if (isdigit(chr) || chr == '.')
		return true;
	return false;
}

#endif // __LEXER_H__