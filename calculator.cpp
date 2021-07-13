#include <vector>
#include <string>
#include <iostream>

#include "symbol.hpp"
#include "calculator.hpp"

namespace calc {

	Calculator::Calculator() {

		m_syntaxError = false;
		m_inputExpression = std::string();
		m_expression = std::vector<Symbol>();
		m_result = 0;
	}




	
	Calculator::~Calculator() {
		m_expression.~vector();
		m_inputExpression.~basic_string();
	}




	
	void Calculator::calculateExpression() {

		m_syntaxError = false;
		m_inputExpression = std::string();
		m_expression = std::vector<Symbol>();
		m_result = 0;

		getExpression();
		tokenizeIntoSymbols();
		preprocessSymbols();
		calculateSymbols();
	}




	
	void Calculator::getExpression() {
		std::getline(std::cin, m_inputExpression);
	}




	
	void Calculator::tokenizeIntoSymbols() {

		std::string tokenBuffer;
		unsigned char tokenType = Symbol::Null;

		for (const char& chr : m_inputExpression) {

			if (isblank(chr)) {
				flushSymbol(tokenBuffer);
				continue;
			}

			if (isdigit(chr) || chr == '.') {
				if (tokenType != Symbol::isNumber) {
					tokenType = Symbol::isNumber;
					flushSymbol(tokenBuffer);
				}
				tokenBuffer += chr;
				continue;
			}

			if (chr == '(' || chr == ')') {
				tokenType = Symbol::isBrakets;
				flushSymbol(tokenBuffer);
				tokenBuffer += chr;
				continue;
			}

			if (tokenType != Symbol::isOperator) {
				tokenType = Symbol::isOperator;
				flushSymbol(tokenBuffer);
			}
			tokenBuffer += chr;
		}
		flushSymbol(tokenBuffer);
	}




	
	void Calculator::flushSymbol(std::string &tokenStr) {

		if (tokenStr.empty())
			return;

		if (isdigit(tokenStr[0]) || tokenStr[0] == '.') {
			m_expression.push_back(Symbol(std::stold(tokenStr)));
			tokenStr.clear();
			return;
		}

		if (tokenStr == "+") {
			m_expression.push_back(Symbol(Symbol::Addition));
			tokenStr.clear();
			return;
		}

		if (tokenStr == "-") {
			m_expression.push_back(Symbol(Symbol::Subtraction));
			tokenStr.clear();
			return;
		}

		if (tokenStr == "*") {
			m_expression.push_back(Symbol(Symbol::Multiplication));
			tokenStr.clear();
			return;
		}

		if (tokenStr == "/") {
			m_expression.push_back(Symbol(Symbol::Division));
			tokenStr.clear();
			return;
		}

		if (tokenStr == "^") {
			m_expression.push_back(Symbol(Symbol::Power));
			tokenStr.clear();
			return;
		}

		if (tokenStr == "sqrt") {
			m_expression.push_back(Symbol(Symbol::SquareRoot));
			tokenStr.clear();
			return;
		}

		if (tokenStr == "sin") {
			m_expression.push_back(Symbol(Symbol::Sin));
			tokenStr.clear();
			return;
		}

		if (tokenStr == "cos") {
			m_expression.push_back(Symbol(Symbol::Cos));
			tokenStr.clear();
			return;
		}

		if (tokenStr == "tan") {
			m_expression.push_back(Symbol(Symbol::Tan));
			tokenStr.clear();
			return;
		}

		if (tokenStr == "(") {
			m_expression.push_back(Symbol(Symbol::OpenBrakets));
			tokenStr.clear();
			return;
		}

		if (tokenStr == ")") {
			m_expression.push_back(Symbol(Symbol::CloseBrakets));
			tokenStr.clear();
			return;
		}
	}




	
	void Calculator::preprocessSymbols() {}


	void Calculator::calculateSymbols() {

		std::vector<Symbol> symbolBuffer = m_expression;

		while (symbolBuffer.size() > 1) {

			//get next operation to do;
			Symbol todo = Symbol();
			int ptr = 0;
			int nesting = 0;
			int priv = 0;
			
			for (int i = 0; i < symbolBuffer.size(); i++) {
				Symbol sym = symbolBuffer[i];

				if (sym.sType == Symbol::isBrakets) {
					if (sym.oType == Symbol::OpenBrakets)
						nesting += 10;
					else
						nesting -= 10;
				}
				else if (sym.sType == Symbol::isOperator) {
					if ((sym.priv + nesting) > todo.priv) {
						todo = sym;
						todo.priv += nesting;
						ptr = i;
					}
				}
			}

			long double result;

			switch(todo.inputs) {
			case 1:
				//do operations with 1 number;
				result = todo.function(Args(symbolBuffer[ptr + 1].value));
				symbolBuffer[ptr] = result;
				symbolBuffer.erase(symbolBuffer.begin() + ptr + 1);

				//remove brakets
				if (symbolBuffer.size() == 1)
					break;
				if (symbolBuffer[ptr - 1].oType == Symbol::OpenBrakets && symbolBuffer[ptr + 1].oType == Symbol::CloseBrakets) {
					symbolBuffer.erase(symbolBuffer.begin() + ptr + 1);
					symbolBuffer.erase(symbolBuffer.begin() + ptr - 1);
				}
				break;
			
			case 2:
				//do operations with 2 numbers
				result = todo.function(Args(symbolBuffer[ptr - 1].value, symbolBuffer[ptr + 1].value));
				symbolBuffer[ptr - 1] = Symbol(result);
				ptr--;
				symbolBuffer.erase(symbolBuffer.begin() + ptr + 1);
				symbolBuffer.erase(symbolBuffer.begin() + ptr + 1);

				//remove brakets
				if (symbolBuffer.size() == 1)
					break;
				if (symbolBuffer[ptr - 1].oType == Symbol::OpenBrakets && symbolBuffer[ptr + 1].oType == Symbol::CloseBrakets) {
					symbolBuffer.erase(symbolBuffer.begin() + ptr + 1);
					symbolBuffer.erase(symbolBuffer.begin() + ptr - 1);
				}
				break;
			}

			
			
		}
		std::cout << symbolBuffer.front().value << std::endl;
		m_result = symbolBuffer.front().value;
	}
}